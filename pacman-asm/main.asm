INCLUDE Irvine32.inc
.386
.model flat, stdcall
.stack 4096

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

GRID_WIDTH = 45
GRID_HEIGHT = 22
GRID_SIZE = GRID_WIDTH * GRID_HEIGHT

.data
    ; Ghost data
    ghosts         DWORD 20 DUP (?) ; 10 max
    numGhosts      BYTE ?
    minValue       DWORD ?
    maxValue       DWORD ?
    randomNum      DWORD ?
    ghostRow       DWORD ?
    ghostCol       DWORD ?
    currDist       DWORD 99999
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
    
    ; 1300
    GRID           BYTE GRID_WIDTH * GRID_HEIGHT DUP(?)

    ; HighScore File
    scoreFile      BYTE "highscore.txt", 0
    handler        HANDLE ?
    buffer         BYTE buffsize DUP(?)

    ; Player data
    playerName     BYTE 31 DUP(0)
    currentScore   DWORD 0
    scoreStr       BYTE "Current Score: ", 0

    ; Menu state
    currentMenu    DWORD MENU_HOME
    selectedLevel  DWORD 0

    ; Menu prompts and strings
    titleArt       BYTE "=================================================", 0Dh, 0Ah
                   BYTE "                   P A C M A N                   ", 0Dh, 0Ah
                   BYTE "=================================================", 0Dh, 0Ah, 0

    namePrompt     BYTE "Enter your name: ", 0
    levelPrompt    BYTE "Select level (1-5): ", 0
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
    comma          BYTE ", ", 0

.code
INCLUDE level1.inc
INCLUDE menuUtils.inc

main PROC
    CALL Randomize

    call InitializeGame
    call MenuLoop
    exit
main ENDP

;----------------------------------------------------
InitializeGame PROC
    ; Set up initial game state
    mov currentMenu, MENU_HOME
    mov currentScore, 0
    
    ret
InitializeGame ENDP

startLevel1 PROC
    call initialiseLevel1
    call initialiseLevel1Ghosts
    call playLevel1

    ret
startLevel1 ENDP

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

END main