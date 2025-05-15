INCLUDE Irvine32.inc
INCLUDE macros.inc
.386
.model flat, stdcall
.stack 4096

includelib Winmm.lib

buffsize = 2000 ; for file reading/writing

; Constants for menu navigation
MENU_HOME = 0
MENU_LEVELS = 1
MENU_INSTR = 2
MENU_SCORES = 3
MENU_GAME = 4

; Constants for directions
UP    = 0
RIGHT = 1
DOWN  = 2
LEFT  = 3

; Sound playback flags
SND_ASYNC = 0001h    ; Play asynchronously
SND_LOOP = 0008h    ; Loop the sound
SND_FILENAME = 00020000h ; Name is a file name
SND_NODEFAULT = 0002h ; avoid playing df sound

GRID_WIDTH = 45
GRID_HEIGHT = 22
GRID_SIZE = GRID_WIDTH * GRID_HEIGHT

.data
    ; sound files
    lobbyMusic     BYTE "lobby_music.wav", 0
    deathMusic     BYTE "pacman_death.wav", 0
    levelMusic     BYTE "pacman_playing.wav", 0

    deathSoundPlaying BYTE 0
    deathSoundTimer   DWORD 0
    deathSoundLength  DWORD 65 

    ; Ghost data
    ghosts         DWORD 20 DUP (?) ; 10 max
    numGhosts      BYTE ?
    minValue       DWORD ?
    maxValue       DWORD ?
    randomNum      DWORD ?
    ghostRow       DWORD ?
    ghostCol       DWORD ?
    currDist       DWORD 99999
    ghostSpeed     DWORD 5
    speedCounter   DWORD 0
    ghostCollision BYTE 0
    originalTiles  BYTE 10 DUP(0)
    ghostDirection BYTE 10 DUP(0)
    possibleDirs   BYTE 4 DUP(0)


    ; Grid data
    PACMAN         BYTE 'P'
    GHOST          BYTE 'G'
    wallChar       BYTE 177 ; Solid block character
    dotChar        BYTE '.'
    pacmanRow      DWORD 8
    pacmanCol      DWORD 22
    gridVal        BYTE ?
    fruits         DWORD 20 DUP(?)
    fruitRow       DWORD ?
    fruitCol       DWORD ?
    numFruits      BYTE 2
    FRUIT          BYTE '+'
    
    ; 1300
    GRID           BYTE GRID_WIDTH * GRID_HEIGHT DUP(?)

    ; HighScore File
    scoreFile      BYTE "highscore.txt", 0
    handler        HANDLE ?
    buffer         BYTE buffsize DUP(?)
    tempBuffer     BYTE 256 DUP(0)
    bytesRead      DWORD 0
    crlfString     BYTE 13, 10, 0

    ; Player data
    playerName     BYTE 31 DUP(0)
    currentScore   DWORD 0
    lives          DWORD 3
    scoreStr       BYTE "Current Score: ", 0
    livesStr       BYTE "Lives: ", 0
    clearingStr    BYTE "       ", 0
    pauseStr       BYTE "Paused!", 0

    ; Menu state
    currentMenu    DWORD MENU_HOME
    selectedLevel  DWORD 0

    ; Menu prompts and strings
    titleArt       BYTE "=================================================", 0Dh, 0Ah
                   BYTE "                   P A C M A N                   ", 0Dh, 0Ah
                   BYTE "=================================================", 0Dh, 0Ah, 0

    namePrompt     BYTE "Enter your name: ", 0
    levelPrompt    BYTE "Select level (1-3): ", 0
    instructTitle  BYTE "INSTRUCTIONS", 0Dh, 0Ah, 0
    instructText   BYTE "Use arrow keys to move Pacman.", 0Dh, 0Ah
                   BYTE "Eat all dots while avoiding ghosts.", 0Dh, 0Ah
                   BYTE "Power pellets let you eat ghosts temporarily.", 0Dh, 0Ah, 0
    scoreTitle     BYTE "HIGH SCORES", 0Dh, 0Ah, 0
    scoreFormat    BYTE "%d. %s: %d", 0Dh, 0Ah, 0
    menuPrompt     BYTE "Press: 1-Play, 2-Instructions, 3-High Scores, ESC-Exit", 0

    startGameMsg   BYTE "Starting game at level ", 0
    keyPressMsg    BYTE "Press any key to return to menu...", 0
    returnMsg      BYTE "Press any key to return...", 0
    
    ; debug msgs
    debugStr       BYTE "Ghost directions: ", 0
    debugStr2      BYTE "Possible directions: ", 0
    debugStr3      BYTE "Debug before readKey: ", 0
    comma          BYTE ", ", 0

.code
PlaySound PROTO,
    pszSound:PTR BYTE,
    hmod:DWORD,
    fdwSound:DWORD

INCLUDE level1.inc
INCLUDE menuUtils.inc
INCLUDE initialisations.inc
INCLUDE level2.inc
INCLUDE level3.inc

main PROC
    CALL Randomize

    call InitializeGame
    call MenuLoop
    exit
main ENDP

;------------------------------------------------
PlayBackgroundMusic PROC uses eax edx    
    INVOKE PlaySound, 
           ADDR lobbyMusic,
           NULL,   ; No module handle
           SND_ASYNC OR SND_LOOP OR SND_FILENAME  ; Play asynchronously and loop
    
    ret
PlayBackgroundMusic ENDP

StopBackgroundMusic PROC uses eax edx
    INVOKE PlaySound, 
           NULL,  ; to stop any playing sound
           NULL,
           0
    
    ret
StopBackgroundMusic ENDP

PlayGameMusic PROC uses eax edx    
    INVOKE PlaySound, 
           ADDR levelMusic,
           NULL,
           SND_ASYNC OR SND_LOOP OR SND_FILENAME 
    
    ret
PlayGameMusic ENDP

UpdateAudio PROC uses eax
    cmp deathSoundPlaying, 0
    je done_update
    
    dec deathSoundTimer
    
    ; Check if timer has expired
    cmp deathSoundTimer, 0
    jg done_update
    
    mov deathSoundPlaying, 0
    call PlayGameMusic
    
done_update:
    ret
UpdateAudio ENDP

PlayDeathSound PROC uses eax edx
    call StopBackgroundMusic
    
    ; Play death sound
    INVOKE PlaySound, 
           ADDR deathMusic,
           NULL,
           SND_ASYNC OR SND_FILENAME OR SND_NODEFAULT
    
    ; Set the death sound flag and timer
    mov deathSoundPlaying, 1
    mov eax, deathSoundLength
    mov deathSoundTimer, eax
    
    ret
PlayDeathSound ENDP

;----------------------------------------------------
InitializeGame PROC
    ; Set up initial game state
    mov currentMenu, MENU_HOME
    mov currentScore, 0
    
    call PlayBackgroundMusic
    ret
InitializeGame ENDP

startLevel1 PROC
    call initialiseLevel1
    
    mov currentScore, 0
    mov pacmanRow, 8
    mov pacmanCol, 22
    mov lives, 3

    call initialiseLevel1Ghosts
    mov numFruits, 2  ; 2 fruits
    call initialiseFruits
    call StopBackgroundMusic
    call playLevel1
    call PlayBackgroundMusic

    ret
startLevel1 ENDP

startLevel2 PROC
    call initialiseLevel2
    
    mov currentScore, 0
    mov pacmanRow, 8
    mov pacmanCol, 22
    mov lives, 3

    call initialiseLevel2Ghosts
    mov numFruits, 4
    call initialiseFruits
    call StopBackgroundMusic
    call playLevel2
    call PlayBackgroundMusic

    ret
startLevel2 ENDP

startLevel3 PROC
    call initialiseLevel3
    
    mov currentScore, 0
    mov pacmanRow, 8
    mov pacmanCol, 22
    mov lives, 3

    call initialiseLevel3Ghosts
    mov numFruits, 10
    call initialiseFruits
    call StopBackgroundMusic
    call playLevel3
    call PlayBackgroundMusic

    ret
startLevel3 ENDP

;-------------------------------------------------------
generateRandomNumber PROC uses eax ebx ecx esi edi
    mov ebx, maxValue
    mov ecx, minValue
    sub ebx, ecx
    inc ebx

    mov eax, ebx
    CALL RandomRange

    add eax, ecx
    mov randomNum, eax

    ret
generateRandomNumber ENDP

getGridValGhost PROC uses eax ebx ecx esi
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    mov al, GRID[ebx]
    mov gridVal, al

    ret
getGridValGhost ENDP

getGridVal PROC uses eax ebx ecx
    mov ebx, pacmanRow
    imul ebx, GRID_WIDTH
    mov ecx, pacmanCol
    add ebx, ecx
    mov al, GRID[ebx]
    mov gridVal, al

    ret
getGridVal ENDP

displayScore PROC uses edx eax
    mov dh, 0
    mov dl, 0
    call Gotoxy
    mov edx, OFFSET scoreStr
    call writestring

    mov dh, 0
    mov dl, lengthof scoreStr - 1
    CALL Gotoxy
    mov eax, currentScore
    call writeint

    ret
displayScore ENDP

displayLives PROC uses edx eax
    mov dh, 0
    mov dl, GRID_WIDTH*2 - 10
    call Gotoxy

    mov edx, OFFSET livesStr
    call writestring
    mov edx, OFFSET clearingStr
    call writestring

    mov dh, 0
    mov dl, GRID_WIDTH*2 - 11 + lengthof livesStr
    CALL Gotoxy
    mov eax, lives
    call writeint

    ret
displayLives ENDP

; calculates distance ghost -> PACMAN
calculateDistance PROC uses ecx edx esi edi
    ; eax = ghostRow, ebx = ghostCol
    mov ecx, pacmanRow
    mov edx, pacmanCol
    
    ; row distance
    cmp eax, ecx
    jl row_less
    mov esi, eax
    sub esi, ecx
    jmp col_dist

    row_less:
        mov esi, ecx
        sub esi, eax
    
    col_dist:
        ; column distance
        cmp ebx, edx
        jl col_less
        mov edi, ebx
        sub edi, edx
        jmp calc_final
    col_less:
        mov edi, edx
        sub edi, ebx
    
    calc_final:
        ; Manhattan distance (row distance + column distance)
        mov eax, esi
        add eax, edi ; final result
    
    ret
calculateDistance ENDP

PauseGame PROC uses edx
    mov dh, GRID_HEIGHT +3
    mov dl, 0
    call gotoxy
    mov edx, offset pauseStr
    call writestring

    pauseLoop:
        call readchar
        cmp al, 'p'
        jnz pauseLoop

    mov dh, GRID_HEIGHT +3
    mov dl, 0
    call Gotoxy
    mov edx, offset clearingStr
    call writestring

    ret
PauseGame ENDP

getGridValFruits PROC uses eax ecx ebx esi
    mov ebx, fruitRow
    imul ebx, GRID_WIDTH
    mov ecx, fruitCol
    add ebx, ecx
    mov al, GRID[ebx]
    mov gridVal, al

    ret
getGridValFruits ENDP

storeFruit PROC uses eax ebx ecx
    mov ebx, fruitRow
    imul ebx, GRID_WIDTH
    mov ecx, fruitCol
    add ebx, ecx
    mov al, FRUIT
    mov GRID[ebx], al

    ; only redraw what's needed
    mov dh, BYTE PTR fruitRow
    add dh, 1
    mov dl, BYTE PTR fruitCol
    shl dl, 1
    call Gotoxy
    mov al, FRUIT
    call WriteChar

    ret
storeFruit ENDP

initialiseFruits PROC uses esi eax ebx ecx
    LOCAL fruitCounter:DWORD

    mov fruitCounter, 0
    mov esi, 0 ; fruits count
    
    fruitInit:
        ; fruitRow
        mov maxValue, GRID_HEIGHT - 3
        mov minValue, 3
        call generateRandomNumber
        mov ebx, randomNum
        mov fruitRow, ebx
        
        ; fruitCol
        mov maxValue, GRID_WIDTH - 3
        mov minValue, 3
        call generateRandomNumber
        mov ecx, randomNum
        mov fruitCol, ecx

        ; inspect grid to confirm availability
        call getGridValFruits
        mov al, gridVal

        cmp al, '.'
        jnz fruitInit

        mov eax, fruitRow
        mov fruits[esi], eax
        mov eax, fruitCol
        mov fruits[esi + type fruits], eax
        mov edx, fruitCounter
        call storeFruit

        add esi, 2 * type fruits
        inc fruitCounter

        mov eax, fruitCounter
        cmp al, numFruits
        jnz fruitInit

    ret
initialiseFruits ENDP

END main