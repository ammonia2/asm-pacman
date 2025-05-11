initialiseLevel2Ghosts PROC uses esi eax ebx ecx
    LOCAL ghostCounter:DWORD

    mov ghostCollision, 0
    mov ghostCounter, 0
    mov ghostSpeed, 5
    mov numGhosts, 5  ; 5 Ghosts for Level 2
    mov esi, 0 ; ghost counter
    
    ghostInitL2:
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
        call getGridValGhost
        mov al, gridVal

        cmp al, '.'
        jnz ghostInitL2

        mov eax, ghostRow
        mov ghosts[esi], eax
        mov eax, ghostCol
        mov ghosts[esi + type ghosts], eax
        mov edx, ghostCounter
        call storeGhost_L2

        add esi, 2 * type ghosts
        inc ghostCounter

        mov eax, ghostCounter
        cmp al, numGhosts
        jnz ghostInitL2

    ret
initialiseLevel2Ghosts ENDP

storeGhost_L2 PROC uses eax ebx ecx
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    mov al, GHOST
    mov cl, GRID[ebx] ; store prev char
    mov GRID[ebx], al

    cmp cl, 'G'
    jz skipTileSaveL2
    cmp cl, 'P'
    jz emptyTileSaveL2

    mov originalTiles[edx], cl
    jmp skipTileSaveL2
    
    emptyTileSaveL2:
        mov originalTiles[edx], 20h

    skipTileSaveL2:

    ; only redraw what's needed
    mov dh, BYTE PTR ghostRow
    add dh, 1
    mov dl, BYTE PTR ghostCol
    shl dl, 1
    call Gotoxy
    mov al, GHOST
    call WriteChar

    ret
storeGhost_L2 ENDP

storePacmanL2 PROC uses ebx ecx
    mov ebx, pacmanRow
    imul ebx, GRID_WIDTH
    mov ecx, pacmanCol
    add ebx, ecx
    mov al, PACMAN
    mov GRID[ebx], al

    mov dh, BYTE PTR pacmanRow
    add dh, 1
    mov dl, BYTE PTR pacmanCol
    shl dl, 1
    call Gotoxy
    mov al, PACMAN
    call WriteChar

    ret
storePacmanL2 ENDP

storeEmptyCharL2 PROC uses ebx ecx
    mov ebx, pacmanRow
    imul ebx, GRID_WIDTH
    mov ecx, pacmanCol
    add ebx, ecx
    mov GRID[ebx], 20h

    mov dh, BYTE PTR pacmanRow
    add dh, 1
    mov dl, BYTE PTR pacmanCol
    shl dl, 1
    call Gotoxy
    mov al, 20h
    call WriteChar

    ret
storeEmptyCharL2 ENDP

storeGhostPrevL2 PROC uses ebx ecx
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    mov cl, originalTiles[edx]
    mov GRID[ebx], cl

    mov dh, BYTE PTR ghostRow
    add dh, 1
    mov dl, BYTE PTR ghostCol
    shl dl, 1
    call Gotoxy
    mov al, cl
    call WriteChar

    ret
storeGhostPrevL2 ENDP

drawLevel2 PROC
    ; curr score
    call displayScore
    call displayLives

    mov dh, 1
    mov dl, 0
    CALL Gotoxy

    mov ecx, GRID_HEIGHT * GRID_WIDTH
    mov ebx, 0
    mov esi, 1 ; col counter
    level2Draw:
        mov al, [GRID + ebx* type GRID]
        call writechar

        ; extra char in bw walls
        cmp al, wallChar
        jnz extraWhiteSpace_L2
        call writechar
        jmp skipWhitespace_L2
        extraWhiteSpace_L2:
        mov al, 20h
        call writechar

        skipWhitespace_L2:
        cmp esi, GRID_WIDTH
        jnz skipCrlf_L2
        call Crlf
        mov esi, 0

        skipCrlf_L2:
        inc esi
        inc ebx

        dec ecx
        jnz level2Draw

    ret
drawLevel2 ENDP

playLevel2 PROC
    call Clrscr
    call drawLevel2

    mov speedCounter, 0 ; for ghosts movement speed
    inputLoop_L2:
        mov edx, ecx
        call ReadKey
        jz noInput_L2

        cmp al, 77h ; lowercase 'w'
        jnz inputCheck2_L2

        call storeEmptyCharL2
        sub pacmanRow, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveUp_L2

        cmp pacmanRow, 0
        jl revertMoveUp_L2

        jmp inputDone_L2

    revertMoveUp_L2:
        add pacmanRow, 1
        call storePacmanL2
        jmp noInput_L2

    inputCheck2_L2:
        cmp al, 61h ; lowercase 'a'
        jnz inputCheck3_L2

        call storeEmptyCharL2
        sub pacmanCol, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveLeft_L2

        cmp pacmanCol, 0
        jl revertMoveLeft_L2

        jmp inputDone_L2

    revertMoveLeft_L2:
        add pacmanCol, 1
        call storePacmanL2
        jmp noInput_L2

    inputCheck3_L2:
        cmp al, 73h ; lowercase 's'
        jnz inputCheck4_L2

        call storeEmptyCharL2
        add pacmanRow, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveDown_L2

        cmp pacmanRow, GRID_HEIGHT-1
        ja revertMoveDown_L2

        jmp inputDone_L2

    revertMoveDown_L2:
        sub pacmanRow, 1
        call storePacmanL2
        jmp noInput_L2

    inputCheck4_L2:
        cmp al, 64h ; lowercase 'd'
        jnz noInput_L2

        call storeEmptyCharL2
        add pacmanCol, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveRight_L2

        cmp pacmanCol, GRID_WIDTH-1
        ja revertMoveRight_L2

        jmp inputDone_L2

    revertMoveRight_L2:
        sub pacmanCol, 1
        call storePacmanL2
        jmp noInput_L2

        ; input processing
    inputDone_L2:
        ; updating grid

        cmp gridVal, '.'
        jnz skipScoreUpdate_L2
        inc currentScore
        call displayScore

    skipScoreUpdate_L2:
        call storePacmanL2

    noInput_L2:
        inc speedCounter
        mov eax, speedCounter
        cmp eax, ghostSpeed ; speed: every 5 iterations
        jb skipGhostMove_L2
        
        mov speedCounter, 0
        call moveGhosts_L2
        cmp ghostCollision, 1
        JE level2Finished

    skipGhostMove_L2:
        mov eax, 50
        call Delay

        jmp inputLoop_L2

    level2Finished:
        dec lives
        CALL displayLives
        cmp lives, 0
        JNE resetPacman_L2
        jmp finish_L2

        resetPacman_L2:
            CALL storeEmptyCharL2
            mov pacmanRow, 8
            mov pacmanCol, 22
            CALL storePacmanL2

            mov ghostCollision, 0
            jmp inputLoop_L2

        finish_L2:
            mov dh, GRID_HEIGHT + 1
            mov dl, 0
            call Gotoxy

        ret
playLevel2 ENDP


getDirections_L2 PROC uses edx esi
    mov esi, 0
    mov edx, eax
    ; UP
    mov ghostRow, edx
    dec ghostRow
    mov ghostCol, ebx
    call getGridValGhost
    mov al, gridVal
    cmp al, wallChar ; Only check if it's not a wall
    je downCheck_l2
    mov possibleDirs[esi], UP
    inc esi

    downCheck_l2:
        ; DOWN
        mov ghostRow, edx
        inc ghostRow
        mov ghostCol, ebx
        call getGridValGhost
        mov al, gridVal
        cmp al, wallChar
        je leftCheck_l2
        mov possibleDirs[esi], DOWN
        inc esi

    leftCheck_l2:
        ; LEFT
        mov ghostRow, edx
        mov ghostCol, ebx
        dec ghostCol
        call getGridValGhost
        mov al, gridVal
        cmp al, wallChar
        je rightCheck_l2
        mov possibleDirs[esi], LEFT
        inc esi

    rightCheck_l2:
        ; RIGHT
        mov ghostRow, edx
        mov ghostCol, ebx
        inc ghostCol
        call getGridValGhost
        mov al, gridVal
        cmp al, wallChar
        je doneCheck_l2
        mov possibleDirs[esi], RIGHT
        inc esi

    doneCheck_l2:
        mov eax, esi ; num of directions
        ret
getDirections_L2 ENDP

moveGhosts_L2 PROC uses ecx esi eax ebx
    mov ecx, 0 ; ghostCounter 
    mov esi, 0

    ghostLoop_L2:
        mov currDist, 0FFFFFFFFh
        mov eax, ghosts[esi]
        mov ebx, ghosts[esi + type ghosts]
        call getDirections_L2  ; Stores all possible directions in possibleDirs
        mov edx, eax    ; Number of directions

        ; Calculate distances & select a direction
        mov edi, 0
        selectDirection_l2:
            mov eax, ghosts[esi]
            mov ebx, ghosts[esi + type ghosts]
            
            cmp possibleDirs[edi], UP
            jne check_DOWN
            sub eax, 1
            jmp calc_dist

        check_DOWN:
            cmp possibleDirs[edi], DOWN
            jne check_LEFT
            add eax, 1
            jmp calc_dist

        check_LEFT:
            cmp possibleDirs[edi], LEFT
            jne check_RIGHT
            sub ebx, 1
            jmp calc_dist

        check_RIGHT:
            cmp possibleDirs[edi], RIGHT
            jne calc_dist
            add ebx, 1
            
        calc_dist:
            CALL calculateDistance
            cmp eax, currDist
            JAE skipUpdateDirn_l2
            mov bl, possibleDirs[edi]
            mov ghostDirection[ecx], bl
            mov currDist, eax

        skipUpdateDirn_l2:
            inc edi
            cmp edi, edx
            jnz selectDirection_l2


    ; Use ghostDirection to now move ghost position
        
    ; Clear previous position:
    mov eax, ghosts[esi]
    mov ebx, ghosts[esi + type ghosts]
    mov ghostRow, eax
    mov ghostCol, ebx
    
    mov edx, ecx
    CALL storeGhostPrevL2

    mov eax, ghosts[esi]
    mov ebx, ghosts[esi + type ghosts]
    movzx edx, ghostDirection[ecx]
        
    cmp edx, UP
    jne check_DOWN_move
    sub ghosts[esi], 1
    jmp store_ghost

    check_DOWN_move:
        cmp edx, DOWN
        jne check_LEFT_move
        add ghosts[esi], 1
        jmp store_ghost

    check_LEFT_move:
        cmp edx, LEFT
        jne check_RIGHT_move
        sub ghosts[esi + type ghosts], 1
        jmp store_ghost

    check_RIGHT_move:
        cmp edx, RIGHT
        jne store_ghost
        add ghosts[esi + type ghosts], 1
        
    store_ghost:
        ; Store ghost at new position
        mov eax, ghosts[esi]
        mov ebx, ghosts[esi + type ghosts]
        mov ghostRow, eax 
        mov ghostCol, ebx

        CALL getGridValGhost
        mov al, gridVal
        cmp al, PACMAN
        JNE skipCollisionSet_L2
        mov ghostCollision, 1
        CALL storeGhost_L2
        jmp ghostDone_L2

        skipCollisionSet_L2:
            mov edx, ecx
            CALL storeGhost_L2

        add esi, 2 * type ghosts
        inc ecx
        cmp cl, numGhosts
        jnz ghostLoop_L2
    
    ghostDone_L2:
        ret
moveGhosts_L2 ENDP