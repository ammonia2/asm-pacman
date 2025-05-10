MenuLoop PROC
    menuLoop_start:
        call Clrscr
        
        ; Display current menu
        mov eax, currentMenu
        cmp eax, MENU_HOME
        jne check_menu_levels
        call DisplayHomeScreen
        jmp menu_input
        
        check_menu_levels:
            cmp eax, MENU_LEVELS
            jne check_menu_instr
            call DisplayLevelSelect
            jmp menu_input
        
        check_menu_instr:
            cmp eax, MENU_INSTR
            jne check_menu_scores
            call DisplayInstructions
            jmp menu_input
        
        check_menu_scores:
            cmp eax, MENU_SCORES
            jne check_menu_game
            call DisplayHighScores
            jmp menu_input
        
        check_menu_game:
            cmp eax, MENU_GAME
            jne menu_input
            call StartGame
        
        menu_input:
            ; menu navigation
            mov al, 0
            call ReadChar
        
            mov ebx, currentMenu
            cmp ebx, MENU_HOME
            jne check_input_levels
            call HandleHomeInput
            jmp check_exit
        
        check_input_levels:
            cmp ebx, MENU_LEVELS
            jne check_input_others
            call HandleLevelInput
            jmp check_exit
        
        check_input_others:
            cmp ebx, MENU_INSTR
            je set_home_menu
            cmp ebx, MENU_SCORES
            jne check_exit
        
        set_home_menu:
            mov currentMenu, MENU_HOME ; any key to get back to menu
        
        check_exit:
            cmp al, 27    ; ESC to exit
            jne menuLoop_start
    
    ret
MenuLoop ENDP

StartGame PROC
    call Clrscr
    
    mov edx, OFFSET startGameMsg
    call WriteString
    mov eax, selectedLevel
    call WriteDec
    call Crlf
    
    call startLevel1
    mov edx, OFFSET keyPressMsg
    call WriteString
    call ReadChar
    
    ; TODO: level init
    mov currentMenu, MENU_HOME
    ret
StartGame ENDP

DisplayHomeScreen PROC
    mov edx, OFFSET titleArt
    call WriteString
    call Crlf
    
    ; Check if player name is empty
    mov al, playerName
    cmp al, 0
    jne display_welcome
    
    ; Get player name
    mov edx, OFFSET namePrompt
    call WriteString
    
    mov edx, OFFSET playerName
    mov ecx, 30      ; Max name chars
    call ReadString
    jmp display_menu
    
    display_welcome:
        ; Display welcome back message
        mov edx, OFFSET playerName
        call WriteString
        call Crlf
    
    display_menu:
        call Crlf
        mov edx, OFFSET menuPrompt
        call WriteString
    
    ret
DisplayHomeScreen ENDP

HandleHomeInput PROC
    cmp al, '1'
    jne check_option2
    mov currentMenu, MENU_LEVELS
    jmp handle_home_done
    
    check_option2:
        cmp al, '2'
        jne check_option3
        mov currentMenu, MENU_INSTR
        jmp handle_home_done
    
    check_option3:
        cmp al, '3'
        jne handle_home_done
        mov currentMenu, MENU_SCORES
    
    handle_home_done:
        ret
HandleHomeInput ENDP

DisplayLevelSelect PROC
    mov edx, OFFSET titleArt
    call WriteString
    call Crlf
    
    mov edx, OFFSET levelPrompt
    call WriteString
    
    ret
DisplayLevelSelect ENDP

HandleLevelInput PROC
    ; Check if input is between '1' and '5'
    cmp al, '1'
    jb check_esc_key
    cmp al, '5'
    ja check_esc_key
    
    ; Convert ASCII to number (subtract '0')
    movzx eax, al
    sub eax, '0'
    mov selectedLevel, eax
    
    ; Redirect to game with selected level
    mov currentMenu, MENU_GAME
    jmp handle_level_done
    
    check_esc_key:
        cmp al, 27    ; ESC
        jne handle_level_done
        ; Return to home menu
        mov currentMenu, MENU_HOME
    
    handle_level_done:
        ret
HandleLevelInput ENDP

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
