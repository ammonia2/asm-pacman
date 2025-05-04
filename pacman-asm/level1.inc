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
    
            cmp edi, 8
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
            cmp edi, 18
            jl CheckWall3_L1
            cmp edi, 26
            jg CheckWall3_L1
            jmp StoreWall_L1
            
        CheckMiddlePart_L1:
            ; Right vertical of 2
            cmp edi, 26
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
            cmp edi, 18
            jl CheckWall3_L1
            cmp edi, 26
            jg CheckWall3_L1
            jmp StoreWall_L1
            
        CheckLeftPart_L1:
            ; Left vertical of 2
            cmp edi, 18
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
            cmp edi, 18
            jl CheckWall3_L1
            cmp edi, 26
            jg CheckWall3_L1
            jmp StoreWall_L1
    
        CheckWall3_L1:
            cmp edi, 36
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


initialiseLevel1Ghosts PROC uses esi eax ebx ecx
    ; 3 Ghosts for Level 1
    mov esi, 0 ; ghost count
    mov numGhosts, 3
    
    ghostInit:
        ; ghostRow
        mov maxValue, GRID_HEIGHT - 3
        mov minValue, 3
        call generateRandomNumber
        mov ebx, randomNum
        mov ghostRow, ebx
        
        ; ghostCol
        mov maxValue, GRID_WIDTH - 3
        mov minValue, 3
        call generateRandomNumber
        mov ecx, randomNum
        mov ghostCol, ecx

        ; inspect grid to confirm availability
        call getGridVal2
        mov al, gridVal

        cmp al, '.'
        jnz ghostInit

        mov eax, ghostRow
        mov ghosts[esi], eax
        mov ghosts[esi + 1], ecx
        call storeGhost_L1

        add esi, 2

        cmp esi, 6
        jnz ghostInit

    ret
initialiseLevel1Ghosts ENDP

getGridVal2 PROC uses eax ebx ecx esi
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    movzx si, bl
    mov al, GRID[si]
    mov gridVal, al

    ret
getGridVal2 ENDP

storeGhost_L1 PROC uses eax ebx ecx
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    mov al, GHOST
    mov GRID[ebx], al

    ret
storeGhost_L1 ENDP

storeGhost2_L1 PROC uses eax ebx ecx
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    mov al, GHOST
    movzx si, bl
    mov GRID[si], al

    ret
storeGhost2_L1 ENDP

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
    mov al, PACMAN
    mov GRID[ebx], al

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

storeEmptyCharGhostL1 PROC uses ebx ecx
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    movzx si, bl
    mov GRID[si], 20h

    ret
storeEmptyCharGhostL1 ENDP


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

        ; extra char in bw walls
        cmp al, wallChar
        jnz extraWhiteSpace_L1
        call writechar
        jmp skipWhitespace_L1
        extraWhiteSpace_L1:
        mov al, 20h
        call writechar

        skipWhitespace_L1:
        cmp esi, GRID_WIDTH
        jnz skipCrlf_L1
        call Crlf
        mov esi, 0

        skipCrlf_L1:
        inc esi
        inc ebx

        dec ecx
        jnz level1Draw

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
        call moveGhosts_L1
        jmp inputLoop_L1

    ret
playLevel1 ENDP


getDirections_L1 PROC uses edx esi
    mov esi, 0
    mov edx, eax
    ; UP
    cmp edx, 1
    JL downCheck_l1
    mov ghostRow, edx
    dec ghostRow
    mov ghostCol, ebx
    call getGridVal2
    mov al, gridVal
    cmp al, 20h ; space
    je up_valid_l1
    cmp al, '.'  ; dot
    jne downCheck_l1
    up_valid_l1:
        mov possibleDirs[esi], UP
        inc esi

    downCheck_l1:
        ; DOWN
        cmp edx, GRID_HEIGHT
        JA leftCheck_l1
        mov ghostRow, edx
        inc ghostRow
        mov ghostCol, ebx
        call getGridVal2
        mov al, gridVal
        cmp al, 20h ; space
        je down_valid_l1
        cmp al, '.'  ; dot
        jne leftCheck_l1
    down_valid_l1:
        mov possibleDirs[esi], DOWN
        inc esi

    leftCheck_l1:
        ; LEFT
        cmp ebx, 1
        JL rightCheck_l1
        mov ghostRow, edx
        mov ghostCol, ebx
        dec ghostCol
        call getGridVal2
        mov al, gridVal
        cmp al, 20h ; space
        je left_valid_l1
        cmp al, '.'  ; dot
        jne rightCheck_l1
    left_valid_l1:
        mov possibleDirs[esi], LEFT
        inc esi

    rightCheck_l1:
        ; RIGHT
        cmp ebx, GRID_WIDTH-1
        JA doneCheck_l1
        mov ghostRow, edx
        mov ghostCol, ebx
        inc ghostCol
        call getGridVal2
        mov al, gridVal
        cmp al, 20h ; space
        je right_valid_l1
        cmp al, '.'  ; dot
        jne doneCheck_l1
    right_valid_l1:
        mov possibleDirs[esi], RIGHT
        inc esi

    doneCheck_l1:
        mov eax, esi ; num of directions
        ret
getDirections_L1 ENDP


moveGhosts_L1 PROC uses ecx esi eax ebx
    mov ecx, 0
    mov esi, 0
    ghostLoop_L1:
        movzx eax, byte ptr ghosts[esi]
        movzx ebx, byte ptr ghosts[esi + 1]
        call getDirections_L1  ; Stores all possible directions in possibleDirs
        mov edx, eax    ; Number of directions

        ; Calculate distances & select a direction
        mov edi, 0
        selectDirection_l1:
            movzx eax, byte ptr ghosts[esi]
            movzx ebx, byte ptr ghosts[esi + 1]

            .IF possibleDirs[edi] == UP
                sub eax, 1
            .ELSEIF possibleDirs[edi] == DOWN
                add eax, 1
            .ELSEIF possibleDirs[edi] == LEFT
                sub ebx, 1
            .ELSEIF possibleDirs[edi] == RIGHT
                add ebx, 1
            .ENDIF

            CALL calculateDistance
            cmp eax, currDist
            JAE skipUpdateDirn_l1

            mov bl, possibleDirs[edi]
            mov ghostDirection[ecx], bl
            mov currDist, eax

            skipUpdateDirn_l1:
                inc edi
                cmp edi, edx
                jnz selectDirection_l1

        ; Use ghostDirection to now move ghost position

        ; Clear previous position:
        movzx eax, byte ptr ghosts[esi]    ; Reload ghostRow (8-bit) into eax
        movzx ebx, byte ptr ghosts[esi + 1]; Reload ghostCol (8-bit) into ebx
        mov ghostRow, eax
        mov ghostCol, ebx
        CALL storeEmptyCharGhostL1

        .IF ghostDirection[ecx] == UP
            sub byte ptr ghosts[esi], 1
        .ELSEIF ghostDirection[ecx] == DOWN
            add byte ptr ghosts[esi], 1
        .ELSEIF ghostDirection[ecx] == LEFT
            sub byte ptr ghosts[esi + 1], 1
        .ELSEIF ghostDirection[ecx] == RIGHT
            add byte ptr ghosts[esi + 1], 1
        .ENDIF

        CALL storeGhost2_L1

        add esi, 2
        inc ecx
        cmp cl, numGhosts
        jnz ghostLoop_L1

    ret
moveGhosts_L1 ENDP