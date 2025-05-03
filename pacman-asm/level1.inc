initialiseLevel1 PROC
    mov ecx, GRID_HEIGHT
    mov esi, -1 ; row counter
    mov ebx, 0 ; grid index
    
    OuterLoopL1:
        inc esi
        push ecx ; save OuterLoop counter
        mov ecx, GRID_WIDTH
        mov edi, -1  ; Column counter
    
        InnerLoopL1:
            inc edi
    
            cmp esi, 0
            je StoreWall_L1
            cmp esi, GRID_HEIGHT-1
            je StoreWall_L1
            cmp edi, 0
            je StoreWall_L1
            cmp edi, GRID_WIDTH-1
            je StoreWall_L1
    
            cmp edi, 16
            jne CheckWall2_L1
            cmp esi, 4
            jl StoreDot_L1
            cmp esi, 16
            jg StoreDot_L1
            jmp StoreWall_L1
    
        CheckWall2_L1:
            ; Top horizontal of 2
            cmp esi, 6
            jne CheckMiddlePart_L1
            cmp edi, 28
            jl CheckWall3_L1
            cmp edi, 36
            jg CheckWall3_L1
            jmp StoreWall_L1
            
        CheckMiddlePart_L1:
            ; Right vertical of 2
            cmp edi, 36
            jne CheckBottomPart_L1
            cmp esi, 6
            jl CheckWall3_L1
            cmp esi, 10
            jg CheckWall3_L1
            jmp StoreWall_L1
            
        CheckBottomPart_L1:
            ; Bottom horizontal of 2
            cmp esi, 10
            jne CheckLeftPart_L1
            cmp edi, 28
            jl CheckWall3_L1
            cmp edi, 36
            jg CheckWall3_L1
            jmp StoreWall_L1
            
        CheckLeftPart_L1:
            ; Left vertical of 2
            cmp edi, 28
            jne CheckHorizontalBottom_L1
            cmp esi, 10
            jl CheckWall3_L1
            cmp esi, 14
            jg CheckWall3_L1
            jmp StoreWall_L1
            
        CheckHorizontalBottom_L1:
            ; bottom most horizontal of 2
            cmp esi, 14
            jne CheckWall3_L1
            cmp edi, 28
            jl CheckWall3_L1
            cmp edi, 36
            jg CheckWall3_L1
            jmp StoreWall_L1
    
        CheckWall3_L1:
            cmp edi, 48
            jne CheckPacmanPos_L1
            cmp esi, 4
            jl StoreDot_L1
            cmp esi, 16
            jg StoreDot_L1
            jmp StoreWall_L1

        CheckPacmanPos_L1:
            cmp esi, pacmanRow
            jne StoreDot_L1
            cmp edi, pacmanCol
            jne StoreDot_L1
            jmp StorePacman_L1
    
        StoreWall_L1:
            mov al, wallChar
            jmp MoveCharacter_L1
    
        StoreDot_L1:
            mov al, dotChar
            jmp MoveCharacter_L1
        StorePacman_L1:
            mov al, 50h
            jmp MoveCharacter_L1
    
        MoveCharacter_L1:
            mov [GRID + ebx * type GRID], al
            
            inc ebx
            dec ecx
            jnz InnerLoopL1
    
        pop ecx
        dec ecx
        jnz OuterLoopL1
    ret
initialiseLevel1 ENDP


getGridVal PROC uses eax ebx ecx
    mov ebx, pacmanRow
    imul ebx, GRID_WIDTH
    mov ecx, pacmanCol
    add ebx, ecx
    mov al, GRID[ebx]
    mov gridVal, al

    ret
getGridVal ENDP

storePacmanL1 PROC uses ebx ecx
    mov ebx, pacmanRow
    imul ebx, GRID_WIDTH
    mov ecx, pacmanCol
    add ebx, ecx
    mov GRID[ebx], 50h

    ret
storePacmanL1 ENDP

storeEmptyCharL1 PROC uses ebx ecx
    mov ebx, pacmanRow
    imul ebx, GRID_WIDTH
    mov ecx, pacmanCol
    add ebx, ecx
    mov GRID[ebx], 20h

    ret
storeEmptyCharL1 ENDP


drawLevel1 PROC
    

    ; curr score
    mov edx, OFFSET scoreStr
    call writestring
    mov eax, currentScore
    call writeint
    call Crlf

    mov ecx, GRID_HEIGHT * GRID_WIDTH
    mov ebx, 0
    mov esi, 1 ; col counter
    level1Draw:
        mov al, [GRID + ebx* type GRID]
        call writechar

        cmp esi, GRID_WIDTH
        jnz skipCrlf_L1
        call Crlf
        mov esi, 0

        skipCrlf_L1:
        inc esi
        inc ebx
        loop level1Draw

    ret
drawLevel1 ENDP

playLevel1 PROC
    call Clrscr
    call drawLevel1

    inputLoop_L1:
        call ReadChar

        cmp al, 77h ; lowercase 'w'
        jnz inputCheck2_L1

        call storeEmptyCharL1
        sub pacmanRow, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveUp_L1

        cmp pacmanRow, 0
        jl revertMoveUp_L1

        jmp inputDone_L1

    revertMoveUp_L1:
        add pacmanRow, 1
        call storePacmanL1
        jmp invalidInput_L1

    inputCheck2_L1:
        cmp al, 61h ; lowercase 'a'
        jnz inputCheck3_L1

        call storeEmptyCharL1
        sub pacmanCol, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveLeft_L1

        cmp pacmanCol, 0
        jl revertMoveLeft_L1

        jmp inputDone_L1

    revertMoveLeft_L1:
        add pacmanCol, 1
        call storePacmanL1
        jmp invalidInput_L1

    inputCheck3_L1:
        cmp al, 73h ; lowercase 's'
        jnz inputCheck4_L1

        call storeEmptyCharL1
        add pacmanRow, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveDown_L1

        cmp pacmanRow, GRID_HEIGHT-1
        ja revertMoveDown_L1

        jmp inputDone_L1

    revertMoveDown_L1:
        sub pacmanRow, 1
        call storePacmanL1
        jmp invalidInput_L1

    inputCheck4_L1:
        cmp al, 64h ; lowercase 'd'
        jnz invalidInput_L1

        call storeEmptyCharL1
        add pacmanCol, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveRight_L1

        cmp pacmanCol, GRID_WIDTH-1
        ja revertMoveRight_L1

        jmp inputDone_L1

    revertMoveRight_L1:
        sub pacmanCol, 1
        call storePacmanL1
        jmp invalidInput_L1

        ; input processing
    inputDone_L1:
        ; updating grid

        cmp gridVal, '.'
        jnz skipScoreUpdate_L1
        inc currentScore

    skipScoreUpdate_L1:
        call storePacmanL1
        
        call Clrscr
        call drawLevel1 ; using updated grid

    invalidInput_L1:
        jmp inputLoop_L1

    ret

playLevel1 ENDP