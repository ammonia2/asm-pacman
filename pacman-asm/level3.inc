initialiseLevel3Ghosts PROC uses esi eax ebx ecx
    LOCAL ghostCounter:DWORD

    mov ghostCollision, 0
    mov ghostCounter, 0
    mov ghostSpeed, 3
    mov numGhosts, 7       ; 7 Ghosts for Level 3
    mov esi, 0             ; ghost array index offset
    
    ghostInitL3:
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
        jnz ghostInitL3

        mov eax, ghostRow
        mov ghosts[esi], eax
        mov eax, ghostCol
        mov ghosts[esi + type ghosts], eax
        mov edx, ghostCounter
        call storeGhost_L3

        add esi, 2 * type ghosts
        inc ghostCounter

        mov eax, ghostCounter
        cmp al, numGhosts
        jnz ghostInitL3

    ret
initialiseLevel3Ghosts ENDP

storeGhost_L3 PROC uses eax ebx ecx
    mov ebx, ghostRow
    imul ebx, GRID_WIDTH
    mov ecx, ghostCol
    add ebx, ecx
    mov al, GHOST
    mov cl, GRID[ebx] ; store prev char
    mov GRID[ebx], al

    cmp cl, 'G'
    jz skipTileSaveL3
    cmp cl, 'P'
    jz emptyTileSaveL3

    mov originalTiles[edx], cl
    jmp skipTileSaveL3
    
    emptyTileSaveL3:
        mov originalTiles[edx], 20h

    skipTileSaveL3:

    ; only redraw what's needed
    mov dh, BYTE PTR ghostRow
    add dh, 1
    mov dl, BYTE PTR ghostCol
    shl dl, 1
    call Gotoxy
    mov al, GHOST
    call WriteChar

    ret
storeGhost_L3 ENDP

storePacmanL3 PROC uses ebx ecx
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
storePacmanL3 ENDP

storeEmptyCharL3 PROC uses ebx ecx
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
storeEmptyCharL3 ENDP

storeGhostPrevL3 PROC uses ebx ecx
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
storeGhostPrevL3 ENDP

drawLevel3 PROC
    ; curr score
    call displayScore
    call displayLives

    mov dh, 1
    mov dl, 0
    CALL Gotoxy

    mov ecx, GRID_HEIGHT * GRID_WIDTH
    mov ebx, 0
    mov esi, 1 ; col counter
    level3Draw:
        mov al, [GRID + ebx* type GRID]
        call writechar

        ; extra char in bw walls
        cmp al, wallChar
        jnz extraWhiteSpace_L3
        call writechar
        jmp skipWhitespace_L3
        extraWhiteSpace_L3:
        mov al, 20h
        call writechar

        skipWhitespace_L3:
        cmp esi, GRID_WIDTH
        jnz skipCrlf_L3
        call Crlf
        mov esi, 0

        skipCrlf_L3:
        inc esi
        inc ebx

        dec ecx
        jnz level3Draw

    ret
drawLevel3 ENDP

playLevel3 PROC
    call Clrscr
    call drawLevel3

    mov speedCounter, 0 ; for ghosts movement speed
    inputLoop_L3:
        mov edx, ecx
        call ReadKey
        jz noInput_L3

        cmp al, 77h ; lowercase 'w'
        jnz inputCheck2_L3

        call storeEmptyCharL3
        sub pacmanRow, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveUp_L3

        cmp pacmanRow, 0
        jl revertMoveUp_L3

        jmp inputDone_L3

    revertMoveUp_L3:
        add pacmanRow, 1
        call storePacmanL3
        jmp noInput_L3

    inputCheck2_L3:
        cmp al, 61h ; lowercase 'a'
        jnz inputCheck3_L3

        call storeEmptyCharL3
        sub pacmanCol, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveLeft_L3

        cmp pacmanCol, 0
        jl revertMoveLeft_L3

        jmp inputDone_L3

    revertMoveLeft_L3:
        add pacmanCol, 1
        call storePacmanL3
        jmp noInput_L3

    inputCheck3_L3:
        cmp al, 73h ; lowercase 's'
        jnz inputCheck4_L3

        call storeEmptyCharL3
        add pacmanRow, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveDown_L3

        cmp pacmanRow, GRID_HEIGHT-1
        ja revertMoveDown_L3

        jmp inputDone_L3

    revertMoveDown_L3:
        sub pacmanRow, 1
        call storePacmanL3
        jmp noInput_L3

    inputCheck4_L3:
        cmp al, 64h ; lowercase 'd'
        jnz noInput_L3

        call storeEmptyCharL3
        add pacmanCol, 1

        call getGridVal
        mov bl, wallChar
        cmp gridVal, bl
        je revertMoveRight_L3

        cmp pacmanCol, GRID_WIDTH-1
        ja revertMoveRight_L3

        jmp inputDone_L3

    revertMoveRight_L3:
        sub pacmanCol, 1
        call storePacmanL3
        jmp noInput_L3

        ; input processing
    inputDone_L3:
        ; updating grid

        cmp gridVal, '.'
        jnz skipScoreUpdate_L3
        inc currentScore
        call displayScore

    skipScoreUpdate_L3:
        call storePacmanL3

    noInput_L3:
        inc speedCounter
        mov eax, speedCounter
        cmp eax, ghostSpeed ; speed: every X iterations
        jb skipGhostMove_L3
        
        mov speedCounter, 0
        call moveGhosts_L3
        cmp ghostCollision, 1
        JE level3Finished

    skipGhostMove_L3:
        mov eax, 50
        call Delay

        jmp inputLoop_L3

    level3Finished:
        dec lives
        CALL displayLives
        cmp lives, 0
        JNE resetPacman_L3
        jmp finish_L3

        resetPacman_L3:
            CALL storeEmptyCharL3
            mov pacmanRow, 8 
            mov pacmanCol, 22
            CALL storePacmanL3

            mov ghostCollision, 0
            jmp inputLoop_L3

        finish_L3:
            mov dh, GRID_HEIGHT + 1
            mov dl, 0
            call Gotoxy

        ret
playLevel3 ENDP


getDirections_L3 PROC uses edx esi
    mov esi, 0
    mov edx, eax
    ; UP
    mov ghostRow, edx
    dec ghostRow
    mov ghostCol, ebx
    call getGridValGhost
    mov al, gridVal
    cmp al, wallChar ; Only check if it's not a wall
    je downCheck_l3
    mov possibleDirs[esi], UP
    inc esi

    downCheck_l3:
        ; DOWN
        mov ghostRow, edx
        inc ghostRow
        mov ghostCol, ebx
        call getGridValGhost
        mov al, gridVal
        cmp al, wallChar
        je leftCheck_l3
        mov possibleDirs[esi], DOWN
        inc esi

    leftCheck_l3:
        ; LEFT
        mov ghostRow, edx
        mov ghostCol, ebx
        dec ghostCol
        call getGridValGhost
        mov al, gridVal
        cmp al, wallChar
        je rightCheck_l3
        mov possibleDirs[esi], LEFT
        inc esi

    rightCheck_l3:
        ; RIGHT
        mov ghostRow, edx
        mov ghostCol, ebx
        inc ghostCol
        call getGridValGhost
        mov al, gridVal
        cmp al, wallChar
        je doneCheck_l3
        mov possibleDirs[esi], RIGHT
        inc esi

    doneCheck_l3:
        mov eax, esi ; num of directions
        ret
getDirections_L3 ENDP

moveGhosts_L3 PROC uses ecx esi eax ebx
    mov ecx, 0 ; ghostCounter 
    mov esi, 0

    ghostLoop_L3:
        mov currDist, 0FFFFFFFFh
        mov eax, ghosts[esi]
        mov ebx, ghosts[esi + type ghosts]
        call getDirections_L3  ; Stores all possible directions in possibleDirs
        mov edx, eax    ; Number of directions

        ; Calculate distances & select a direction
        mov edi, 0
        selectDirection_l3:
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
            JAE skipUpdateDirn_l3
            mov bl, possibleDirs[edi]
            mov ghostDirection[ecx], bl
            mov currDist, eax

        skipUpdateDirn_l3:
            inc edi
            cmp edi, edx
            jnz selectDirection_l3


    ; Use ghostDirection to now move ghost position
        
    ; Clear previous position:
    mov eax, ghosts[esi]
    mov ebx, ghosts[esi + type ghosts]
    mov ghostRow, eax
    mov ghostCol, ebx
    
    mov edx, ecx
    CALL storeGhostPrevL3

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
        JNE skipCollisionSet_L3
        mov ghostCollision, 1
        CALL storeGhost_L3
        jmp ghostDone_L3

        skipCollisionSet_L3:
            mov edx, ecx
            CALL storeGhost_L3

        add esi, 2 * type ghosts
        inc ecx
        cmp cl, numGhosts
        jnz ghostLoop_L3
    
    ghostDone_L3:
        ret
moveGhosts_L3 ENDP