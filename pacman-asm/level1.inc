initialiseLevel1 PROC
    mov ecx, GRID_HEIGHT
    mov esi, -1 ; row counter
    
    OuterLoop:
        inc esi
        push ecx ; save OuterLoop counter
        mov ecx, GRID_WIDTH
        mov edi, -1  ; Column counter
    
        InnerLoop:
            inc edi
    
            ; Check if we're on the border (top, bottom, left, right)
            cmp esi, 0              ; Top row?
            je DrawWall
            cmp esi, GRID_HEIGHT-1  ; Bottom row?
            je DrawWall
            cmp edi, 0              ; Left column?
            je DrawWall
            cmp edi, GRID_WIDTH-1   ; Right column?
            je DrawWall
    
            ; Check for internal walls - horizontal wall in middle
            mov eax, GRID_HEIGHT
            shr eax, 1              ; Divide by 2 for middle row
            cmp esi, eax            ; Middle row?
            jne CheckWall1
            cmp edi, 20             ; Draw wall sections in middle row
            jle DrawWall
            cmp edi, 40
            jge CheckMidSection
            jmp DrawDot
    
        CheckMidSection:
            cmp edi, 80
            jle DrawWall
            cmp edi, 100
            jge DrawWall
            jmp DrawDot
    
        CheckWall1:
            ; Vertical wall 1/4 from left
            mov eax, GRID_WIDTH
            shr eax, 2              ; 1/4 of grid width
            cmp edi, eax
            jne CheckWall2
            cmp esi, 5
            jl DrawDot
            cmp esi, GRID_HEIGHT-5
            jg DrawDot
            jmp DrawWall
    
        CheckWall2:
            ; Vertical wall 3/4 from left
            mov eax, GRID_WIDTH
            shr eax, 2
            imul eax, 3             ; 3/4 of grid width
            cmp edi, eax
            jne CheckWall3
            cmp esi, 5
            jl DrawDot
            cmp esi, GRID_HEIGHT-5
            jg DrawDot
            jmp DrawWall
    
        CheckWall3:
            ; Some horizontal barriers in upper part
            cmp esi, 7
            jne CheckWall4
            cmp edi, 40
            jl CheckSpecific1
            cmp edi, 80
            jg CheckSpecific1
            jmp DrawWall
    
        CheckSpecific1:
            cmp edi, 30
            je DrawWall
            cmp edi, 90
            je DrawWall
            jmp DrawDot
    
        CheckWall4:
            ; Some horizontal barriers in lower part
            cmp esi, GRID_HEIGHT-8
            jne CheckPacmanPos
            cmp edi, 40
            jl CheckSpecific2
            cmp edi, 80
            jg CheckSpecific2
            jmp DrawWall
    
        CheckSpecific2:
            cmp edi, 30
            je DrawWall
            cmp edi, 90
            je DrawWall
            jmp CheckPacmanPos

        CheckPacmanPos:
            cmp esi, pacmanRow
            jne DrawDot
            cmp edi, pacmanCol
            jne DrawDot
            jmp DrawPacman
    
        DrawWall:
            mov al, wallChar
            jmp DrawCharacter
    
        DrawDot:
            mov al, dotChar
            jmp DrawCharacter

        DrawPacman:
            mov al, PACMAN
            jmp DrawCharacter
    
        DrawCharacter:
            call WriteChar
    
            dec ecx
            jnz InnerLoop
    
        call Crlf
        pop ecx
        dec ecx
        jnz OuterLoop
    
    ret
initialiseLevel1 ENDP
