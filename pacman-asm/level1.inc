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
    
            ; Check if we're on the border (top, bottom, left, right)
            cmp esi, 0              ; Top row?
            je StoreWall_L1
            cmp esi, GRID_HEIGHT-1  ; Bottom row?
            je StoreWall_L1
            cmp edi, 0              ; Left column?
            je StoreWall_L1
            cmp edi, GRID_WIDTH-1   ; Right column?
            je StoreWall_L1
    
            ; Check for internal walls - horizontal wall in middle
            mov eax, GRID_HEIGHT
            shr eax, 1              ; Divide by 2 for middle row
            cmp esi, eax            ; Middle row?
            jne CheckWall1_L1
            cmp edi, 20             ; Draw wall sections in middle row
            jle StoreWall_L1
            cmp edi, 40
            jge CheckMidSectionL1
            jmp StoreDot_L1
    
        CheckMidSectionL1:
            cmp edi, 80
            jle StoreWall_L1
            cmp edi, 100
            jge StoreWall_L1
            jmp StoreDot_L1
    
        CheckWall1_L1:
            ; Vertical wall 1/4 from left
            mov eax, GRID_WIDTH
            shr eax, 2              ; 1/4 of grid width
            cmp edi, eax
            jne CheckWall2_L1
            cmp esi, 5
            jl StoreDot_L1
            cmp esi, GRID_HEIGHT-5
            jg StoreDot_L1
            jmp StoreWall_L1
    
        CheckWall2_L1:
            ; Vertical wall 3/4 from left
            mov eax, GRID_WIDTH
            shr eax, 2
            imul eax, 3             ; 3/4 of grid width
            cmp edi, eax
            jne CheckWall3_L1
            cmp esi, 5
            jl StoreDot_L1
            cmp esi, GRID_HEIGHT-5
            jg StoreDot_L1
            jmp StoreWall_L1
    
        CheckWall3_L1:
            ; Some horizontal barriers in upper part
            cmp esi, 7
            jne CheckWall4_L1
            cmp edi, 40
            jl CheckSpecific1_L1
            cmp edi, 80
            jg CheckSpecific1_L1
            jmp StoreWall_L1
    
        CheckSpecific1_L1:
            cmp edi, 30
            je StoreWall_L1
            cmp edi, 90
            je StoreWall_L1
            jmp StoreDot_L1
    
        CheckWall4_L1:
            ; Some horizontal barriers in lower part
            cmp esi, GRID_HEIGHT-8
            jne CheckPacmanPos_L1
            cmp edi, 40
            jl CheckSpecific2_L1
            cmp edi, 80
            jg CheckSpecific2_L1
            jmp StoreWall_L1
    
        CheckSpecific2_L1:
            cmp edi, 30
            je StoreWall_L1
            cmp edi, 90
            je StoreWall_L1
            jmp CheckPacmanPos_L1

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
            mov al, PACMAN
            jmp MoveCharacter_L1
    
        MoveCharacter_L1:
            mov [GRID + ebx * type GRID], al
            
            inc ebx
            dec ecx
            jnz InnerLoopL1
    
        pop ecx
        dec ecx
        jnz OuterLoopL1
    
    call playLevel1

    ret
initialiseLevel1 ENDP


drawLevel1 PROC
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
    call drawLevel1

    
    ret
playLevel1 ENDP