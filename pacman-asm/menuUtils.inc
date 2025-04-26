;----------------------------------------------------
MenuLoop PROC
    .REPEAT
        call Clrscr
        
        ; Display current menu
        mov eax, currentMenu
        .IF eax == MENU_HOME
            call DisplayHomeScreen
        .ELSEIF eax == MENU_LEVELS
            call DisplayLevelSelect
        .ELSEIF eax == MENU_INSTR
            call DisplayInstructions
        .ELSEIF eax == MENU_SCORES
            call DisplayHighScores
        .ELSEIF eax == MENU_GAME
            call StartGame
        .ENDIF
        
        ; menu navigation
        mov al, 0
        call ReadChar
        
        .IF currentMenu == MENU_HOME
            call HandleHomeInput
        .ELSEIF currentMenu == MENU_LEVELS
            call HandleLevelInput
        .ELSEIF currentMenu == MENU_INSTR || currentMenu == MENU_SCORES
            mov currentMenu, MENU_HOME ; any key to get back to menu
        .ENDIF
    .UNTIL (al == 27)    ; ESC to exit
    
    ret
MenuLoop ENDP

;----------------------------------------------------
StartGame PROC
    call Clrscr
    
    mov edx, OFFSET startGameMsg
    call WriteString
    mov eax, selectedLevel
    call WriteDec
    call Crlf
    
    mov edx, OFFSET keyPressMsg
    call WriteString
    call ReadChar
    
    ; TODO: level init
    mov currentMenu, MENU_HOME
    ret
StartGame ENDP

;----------------------------------------------------
DisplayHomeScreen PROC
    mov edx, OFFSET titleArt
    call WriteString
    call Crlf
    
    ; Check if player name is empty
    mov al, playerName
    .IF al == 0
        ; Get player name
        mov edx, OFFSET namePrompt
        call WriteString
        
        mov edx, OFFSET playerName
        mov ecx, 30      ; Max name chars
        call ReadString
    .ELSE
        ; Display welcome back message
        mov edx, OFFSET playerName
        call WriteString
        call Crlf
    .ENDIF
    
    call Crlf
    mov edx, OFFSET menuPrompt
    call WriteString
    
    ret
DisplayHomeScreen ENDP

;----------------------------------------------------
HandleHomeInput PROC
    .IF al == '1'
        mov currentMenu, MENU_LEVELS
    .ELSEIF al == '2'
        mov currentMenu, MENU_INSTR
    .ELSEIF al == '3'
        mov currentMenu, MENU_SCORES
    .ENDIF
    
    ret
HandleHomeInput ENDP

;----------------------------------------------------
DisplayLevelSelect PROC
    mov edx, OFFSET titleArt
    call WriteString
    call Crlf
    
    mov edx, OFFSET levelPrompt
    call WriteString
    
    ret
DisplayLevelSelect ENDP

;----------------------------------------------------
HandleLevelInput PROC
    .IF al >= '1' && al <= '5'
        ; Convert ASCII to number (subtract '0')
        movzx eax, al
        sub eax, '0'
        mov selectedLevel, eax
        
        ; Redirect to game with selected level
        mov currentMenu, MENU_GAME
    .ELSEIF al == 27    ; ESC
        ; Return to home menu
        mov currentMenu, MENU_HOME
    .ENDIF
    
    ret
HandleLevelInput ENDP

;----------------------------------------------------
DisplayInstructions PROC
    mov edx, OFFSET titleArt
    call WriteString
    call Crlf
    
    mov edx, OFFSET instructTitle
    call WriteString
    call Crlf
    
    mov edx, OFFSET instructText
    call WriteString
    call Crlf
    
    mov edx, OFFSET returnMsg
    call WriteString
    
    ret
DisplayInstructions ENDP

;----------------------------------------------------
DisplayHighScores PROC
    mov edx, OFFSET titleArt
    call WriteString
    call Crlf
    
    mov edx, OFFSET scoreTitle
    call WriteString
    call Crlf
    
    ; reading highscores
    mov edx, OFFSET scoreFile
    call openInputFile
    mov handler, eax

    mov edx, OFFSET buffer
    mov ecx, buffsize
    call readFromFile       ; length of data read in eax
    
    ; displaying highscores:
    mov ecx, eax
    mov esi, 0
    displayScores:
        mov al, buffer[esi]
        call writechar
        inc esi
        loop displayScores

    ; closing File
    mov eax, handler
    call closeFile
    
    call Crlf
    
    mov edx, OFFSET returnMsg
    call WriteString

    ret
DisplayHighScores ENDP