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
    
    cmp selectedLevel, 1
    JNZ checkLevel2
    call startLevel1
    jmp doneLevelCheck

    checkLevel2:
        cmp selectedLevel, 2
        JNZ checkLevel3
        call startLevel2
        jmp doneLevelCheck

    checkLevel3:
        cmp selectedLevel, 3
        JNZ doneLevelCheck
        JNZ checkLevel3
        call startLevel3

    doneLevelCheck:
        ; save highscore here!!!
        call saveHighScore
        mov edx, OFFSET keyPressMsg
        call WriteString
        call ReadChar
    
    mov currentMenu, MENU_HOME
    ret
StartGame ENDP

saveHighScore PROC
    mov edx, OFFSET scoreFile
    call openInputFile
    mov handler, eax
    
    cmp eax, INVALID_HANDLE_VALUE
    jne fileOpened
    
    ; If file doesn't exist, create it
    mov edx, OFFSET scoreFile
    call CreateOutputFile
    mov handler, eax
    jmp formatNewScore
    
    fileOpened:
        ; Read the existing scores into buffer
        mov edx, OFFSET buffer
        mov ecx, buffsize
        call readFromFile
        mov bytesRead, eax
    
        ; Close the input file
        mov eax, handler
        call closeFile
    
        ; Reopen file for writing
        mov edx, OFFSET scoreFile
        call CreateOutputFile
        mov handler, eax
    
        ; Write the original data back first
        mov edx, OFFSET buffer
        mov ecx, bytesRead
        call WriteToFile
    
    formatNewScore:
        mov edi, OFFSET tempBuffer
        mov esi, OFFSET playerName
    
    copyPlayerName:
        mov al, [esi]
        cmp al, 0
        je nameEnd
        mov [edi], al
        inc esi
        inc edi
        jmp copyPlayerName
    
    nameEnd:
        ; Add ": " after name
        mov al, ':'
        mov [edi], al
        inc edi
        mov al, ' '
        mov [edi], al
        inc edi
    
        ; Convert score to string
        mov eax, currentScore
        call WriteToStr
    
        ; Add newline
        mov al, 13
        mov [edi], al
        inc edi
        mov al, 10
        mov [edi], al
        inc edi
        mov al, 0
        mov [edi], al
    
        ; Calculate length of formatted string
        mov edi, OFFSET tempBuffer
        mov ecx, 0
    calculateLength:
        mov al, [edi + ecx]
        cmp al, 0
        je lengthFound
        inc ecx
        jmp calculateLength
    
    lengthFound:
        push ecx

        ; mov eax, handler
        ; mov edx, OFFSET crlfString
        ; mov ecx, 2
        ; call WriteToFile

        pop ecx
        mov edx, OFFSET tempBuffer
        mov eax, handler
        call WriteToFile
    
        ; Close the file
        mov eax, handler
        call closeFile
    
    ret
saveHighScore ENDP

WriteToStr PROC uses eax ecx ebx edx  
    mov ebx, 10         ; divisor
    mov ecx, 0          ; count digits
    
    ; Special case for zero
    test eax, eax
    jnz notZero
    mov BYTE PTR [edi], '0'
    inc edi
    jmp writeEnd
    
    notZero:
    convertLoop:
        mov edx, 0 
        div ebx
        push edx            ; save remainder
        inc ecx
        test eax, eax       ; check if done
        jnz convertLoop
    
        ; Now pop digits in correct order
    popLoop:
        pop edx
        add edx, '0'        ; convert to ASCII
        mov [edi], dl       ; store in buffer
        inc edi
        loop popLoop
    
    writeEnd:
        mov BYTE PTR [edi], 0    ; null terminator
    
    ret
WriteToStr ENDP

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
    ; Check if input is between '1' and '3'
    cmp al, '1'
    jb check_esc_key
    cmp al, '3'
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
