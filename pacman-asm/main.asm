INCLUDE Irvine32.inc
.386
.model flat, stdcall
.stack 4096

buffsize = 300 ; for file reading/writing

; Constants for menu navigation
MENU_HOME      EQU 0
MENU_LEVELS    EQU 1
MENU_INSTR     EQU 2
MENU_SCORES    EQU 3
MENU_GAME      EQU 4

.data
    ; Global Variables
    PACMAN         BYTE ?, ?
    GRID           BYTE 60 DUP(?)

    ; HighScore File
    scoreFile      BYTE "highscore.txt", 0
    handler        HANDLE ?
    buffer         BYTE buffsize DUP(?)

    ; Player data
    playerName     BYTE 31 DUP(0)    ; Player name (30 chars max + null)
    currentScore   DWORD 0
    highScores     DWORD 5 DUP(0)    ; Store top 5 scores
    highScoreNames BYTE 5 DUP(31 DUP(0)) ; Names for high scores

    ; Menu state
    currentMenu    DWORD MENU_HOME
    selectedLevel  DWORD 0

    ; Menu prompts and strings
    titleArt       BYTE "=================================", 0Dh, 0Ah
                   BYTE "           P A C M A N           ", 0Dh, 0Ah
                   BYTE "=================================", 0Dh, 0Ah, 0

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
    
    
.code
INCLUDE utils.inc 
INCLUDE menuUtils.inc 

main PROC
    call InitializeGame
    call MenuLoop
    exit
main ENDP

;----------------------------------------------------
InitializeGame PROC
    ; Set up initial game state
    mov currentMenu, MENU_HOME
    mov currentScore, 0
    
    ; Set default high scores for testing
    mov highScores[0], 5000
    mov highScores[4], 1000
    mov highScores[8], 800
    mov highScores[12], 500
    mov highScores[16], 200
    
    ; Copy name strings (would use proper string copy in real implementation)
    mov eax, OFFSET highScoreNames
    mov BYTE PTR [eax], "P"
    mov BYTE PTR [eax+1], "r"
    mov BYTE PTR [eax+2], "o"
    
    mov eax, OFFSET highScoreNames+31
    mov BYTE PTR [eax], "A"
    mov BYTE PTR [eax+1], "c"
    mov BYTE PTR [eax+2], "e"
    
    ret
InitializeGame ENDP

END main