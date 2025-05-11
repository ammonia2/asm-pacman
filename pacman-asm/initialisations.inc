initialiseLevel1 PROC
    mov ecx, GRID_HEIGHT
    mov esi, -1         ; row counter (0 to GRID_HEIGHT-1)
    mov ebx, 0          ; grid index

    OuterLoopL1:
        inc esi
        push ecx            ; save OuterLoop counter
        mov ecx, GRID_WIDTH
        mov edi, -1         ; column counter (0 to GRID_WIDTH-1)

        InnerLoopL1:
            inc edi

            ; 1. Check for Outer Boundary Walls
            cmp esi, 0
            je StoreWall_L1
            cmp esi, GRID_HEIGHT-1
            je StoreWall_L1
            cmp edi, 0
            je StoreWall_L1
            cmp edi, GRID_WIDTH-1
            je StoreWall_L1

            ; 2. Original 1 Vertical Wall at edi = 8
            ; This wall runs from esi = 4 to esi = 16
            cmp edi, 9
            jne CheckTwoShape_Entry_L1  ; If not column 8, proceed to check '2' shape
            ; If it IS column 8, check row bounds for this specific wall
            cmp esi, 4
            jl CheckTwoShape_Entry_L1   ; If row is outside this wall's specific range, try other shapes
            cmp esi, 16
            jg CheckTwoShape_Entry_L1   ; If row is outside this wall's specific range, try other shapes
            jmp StoreWall_L1                    ; It's part of this specific vertical wall

        CheckTwoShape_Entry_L1:
            ; 3. Original '2' Shape Walls
            ; Check Top horizontal bar of '2': esi = 6, edi from 18 to 26
            cmp esi, 6
            jne CheckTwoShape_MiddlePart_L1 ; If not row 6, try next part of '2'
            cmp edi, 18
            jl CheckOneShape_Entry_L1        ; If col < 18, not this segment, try Wall at 36
            cmp edi, 26
            jg CheckOneShape_Entry_L1        ; If col > 26, not this segment, try Wall at 36
            jmp StoreWall_L1                       ; It's part of the '2'

        CheckTwoShape_MiddlePart_L1:
            ; Check Right vertical bar of '2': edi = 26, esi from 6 to 10
            cmp edi, 26
            jne CheckTwoShape_BottomPart_L1 ; If not col 26, try next part of '2'
            cmp esi, 6
            jl CheckOneShape_Entry_L1         ; If row < 6, not this segment, try Wall at 36
            cmp esi, 10
            jg CheckOneShape_Entry_L1         ; If row > 10, not this segment, try Wall at 36
            jmp StoreWall_L1                        ; It's part of the '2'

        CheckTwoShape_BottomPart_L1:
            ; Check Middle horizontal bar of '2': esi = 10, edi from 18 to 26
            cmp esi, 10
            jne CheckTwoShape_LeftPart_L1   ; If not row 10, try next part of '2'
            cmp edi, 18
            jl CheckOneShape_Entry_L1         ; If col < 18, not this segment, try Wall at 36
            cmp edi, 26
            jg CheckOneShape_Entry_L1         ; If col > 26, not this segment, try Wall at 36
            jmp StoreWall_L1                        ; It's part of the '2'

        CheckTwoShape_LeftPart_L1:
            ; Check Left vertical bar of '2': edi = 18, esi from 10 to 14
            cmp edi, 18
            jne CheckTwoShape_HorizontalBottom_L1 ; If not col 18, try next part of '2'
            cmp esi, 10
            jl CheckOneShape_Entry_L1               ; If row < 10, not this segment, try Wall at 36
            cmp esi, 14
            jg CheckOneShape_Entry_L1               ; If row > 14, not this segment, try Wall at 36
            jmp StoreWall_L1                              ; It's part of the '2'

        CheckTwoShape_HorizontalBottom_L1:
            ; Check Bottom most horizontal bar of '2': esi = 14, edi from 18 to 26
            cmp esi, 14
            jne CheckOneShape_Entry_L1 ; If not row 14, '2' shape not found here, try Wall at 36
            cmp edi, 18
            jl CheckOneShape_Entry_L1  ; If col < 18, not this segment, try Wall at 36
            cmp edi, 26
            jg CheckOneShape_Entry_L1  ; If col > 26, not this segment, try Wall at 36
            jmp StoreWall_L1                 ; It's part of the '2'

        CheckOneShape_Entry_L1:
            ; 4. Original Vertical Wall at edi = 36
            ; This wall runs from esi = 4 to esi = 16
            cmp edi, 35
            jne CheckNewWalls_SetA_Start_L1
            cmp esi, 4
            jl CheckNewWalls_SetA_Start_L1  ; If row is outside this wall's specific range, try new walls
            cmp esi, 16
            jg CheckNewWalls_SetA_Start_L1  ; If row is outside this wall's specific range, try new walls
            jmp StoreWall_L1                  ; It's part of this specific vertical wall

        CheckNewWalls_SetA_Start_L1:
            ; 5. New Walls Set A (Top-left L-shape & extensions)
            ; A1. Horizontal: esi = 2, edi from 2 to 6.
            cmp esi, 2
            jne CheckNewWalls_SetA2_L1
            cmp edi, 2
            jl CheckNewWalls_SetA2_L1
            cmp edi, 6
            jg CheckNewWalls_SetA2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetA2_L1: ; A2. Vertical: edi = 2, esi from 2 to 8.
            cmp edi, 2
            jne CheckNewWalls_SetA3_L1
            cmp esi, 2
            jl CheckNewWalls_SetA3_L1
            cmp esi, 8
            jg CheckNewWalls_SetA3_L1
            jmp StoreWall_L1
        CheckNewWalls_SetA3_L1: ; A3. Horizontal: esi = 8, edi from 2 to 6.
            cmp esi, 8
            jne CheckNewWalls_SetA4_L1
            cmp edi, 2
            jl CheckNewWalls_SetA4_L1
            cmp edi, 6
            jg CheckNewWalls_SetA4_L1
            jmp StoreWall_L1
        CheckNewWalls_SetA4_L1: ; A4. Vertical: edi = 6, esi from 2 to 4.
            cmp edi, 6
            jne CheckNewWalls_SetA5_L1
            cmp esi, 2
            jl CheckNewWalls_SetA5_L1
            cmp esi, 4
            jg CheckNewWalls_SetA5_L1
            jmp StoreWall_L1
        CheckNewWalls_SetA5_L1: ; A5. Horizontal: esi = 4, edi from 4 to 6.
            cmp esi, 4
            jne CheckNewWalls_SetB_Start_L1 ; Next Set
            cmp edi, 4
            jl CheckNewWalls_SetB_Start_L1
            cmp edi, 6
            jg CheckNewWalls_SetB_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetB_Start_L1:
            ; 6. New Walls Set B (Bottom-left L-shape & extensions)
            ; B1. Horizontal: esi = 12, edi from 2 to 6.
            cmp esi, 12
            jne CheckNewWalls_SetB2_L1
            cmp edi, 2
            jl CheckNewWalls_SetB2_L1
            cmp edi, 6
            jg CheckNewWalls_SetB2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetB2_L1: ; B2. Vertical: edi = 2, esi from 12 to 18.
            cmp edi, 2
            jne CheckNewWalls_SetB3_L1
            cmp esi, 12
            jl CheckNewWalls_SetB3_L1
            cmp esi, 18
            jg CheckNewWalls_SetB3_L1
            jmp StoreWall_L1
        CheckNewWalls_SetB3_L1: ; B3. Horizontal: esi = 18, edi from 2 to 6.
            cmp esi, 18
            jne CheckNewWalls_SetB4_L1
            cmp edi, 2
            jl CheckNewWalls_SetB4_L1
            cmp edi, 6
            jg CheckNewWalls_SetB4_L1
            jmp StoreWall_L1
        CheckNewWalls_SetB4_L1: ; B4. Vertical: edi = 6, esi from 16 to 18.
            cmp edi, 6
            jne CheckNewWalls_SetB5_L1
            cmp esi, 16
            jl CheckNewWalls_SetB5_L1
            cmp esi, 18
            jg CheckNewWalls_SetB5_L1
            jmp StoreWall_L1
        CheckNewWalls_SetB5_L1: ; B5. Horizontal: esi = 16, edi from 4 to 6.
            cmp esi, 16
            jne CheckNewWalls_SetC_Start_L1 ; Next Set
            cmp edi, 4
            jl CheckNewWalls_SetC_Start_L1
            cmp edi, 6
            jg CheckNewWalls_SetC_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetC_Start_L1:
            ; 7. New Walls Set C (Top-right L-shape & extensions)
            ; C1. Horizontal: esi = 2, edi from 38 to 42.
            cmp esi, 2
            jne CheckNewWalls_SetC2_L1
            cmp edi, 38
            jl CheckNewWalls_SetC2_L1
            cmp edi, 42
            jg CheckNewWalls_SetC2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetC2_L1: ; C2. Vertical: edi = 42, esi from 2 to 8.
            cmp edi, 42
            jne CheckNewWalls_SetC3_L1
            cmp esi, 2
            jl CheckNewWalls_SetC3_L1
            cmp esi, 8
            jg CheckNewWalls_SetC3_L1
            jmp StoreWall_L1
        CheckNewWalls_SetC3_L1: ; C3. Horizontal: esi = 8, edi from 38 to 42.
            cmp esi, 8
            jne CheckNewWalls_SetC4_L1
            cmp edi, 38
            jl CheckNewWalls_SetC4_L1
            cmp edi, 42
            jg CheckNewWalls_SetC4_L1
            jmp StoreWall_L1
        CheckNewWalls_SetC4_L1: ; C4. Vertical: edi = 38, esi from 2 to 4.
            cmp edi, 38
            jne CheckNewWalls_SetC5_L1
            cmp esi, 2
            jl CheckNewWalls_SetC5_L1
            cmp esi, 4
            jg CheckNewWalls_SetC5_L1
            jmp StoreWall_L1
        CheckNewWalls_SetC5_L1: ; C5. Horizontal: esi = 4, edi from 38 to 40.
            cmp esi, 4
            jne CheckNewWalls_SetD_Start_L1 ; Next Set
            cmp edi, 38
            jl CheckNewWalls_SetD_Start_L1
            cmp edi, 40
            jg CheckNewWalls_SetD_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetD_Start_L1:
            ; 8. New Walls Set D (Bottom-right L-shape & extensions)
            ; D1. Horizontal: esi = 12, edi from 38 to 42.
            cmp esi, 12
            jne CheckNewWalls_SetD2_L1
            cmp edi, 38
            jl CheckNewWalls_SetD2_L1
            cmp edi, 42
            jg CheckNewWalls_SetD2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetD2_L1: ; D2. Vertical: edi = 42, esi from 12 to 18.
            cmp edi, 42
            jne CheckNewWalls_SetD3_L1
            cmp esi, 12
            jl CheckNewWalls_SetD3_L1
            cmp esi, 18
            jg CheckNewWalls_SetD3_L1
            jmp StoreWall_L1
        CheckNewWalls_SetD3_L1: ; D3. Horizontal: esi = 18, edi from 38 to 42.
            cmp esi, 18
            jne CheckNewWalls_SetD4_L1
            cmp edi, 38
            jl CheckNewWalls_SetD4_L1
            cmp edi, 42
            jg CheckNewWalls_SetD4_L1
            jmp StoreWall_L1
        CheckNewWalls_SetD4_L1: ; D4. Vertical: edi = 38, esi from 16 to 18.
            cmp edi, 38
            jne CheckNewWalls_SetD5_L1
            cmp esi, 16
            jl CheckNewWalls_SetD5_L1
            cmp esi, 18
            jg CheckNewWalls_SetD5_L1
            jmp StoreWall_L1
        CheckNewWalls_SetD5_L1: ; D5. Horizontal: esi = 16, edi from 38 to 40.
            cmp esi, 16
            jne CheckNewWalls_SetE_Start_L1 ; Next Set
            cmp edi, 38
            jl CheckNewWalls_SetE_Start_L1
            cmp edi, 40
            jg CheckNewWalls_SetE_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetE_Start_L1:
            ; 9. New Walls Set E (Top connecting horizontal lines)
            ; E1. esi = 2, edi from 10 to 16.
            cmp esi, 2
            jne CheckNewWalls_SetE2_L1
            cmp edi, 11
            jl CheckNewWalls_SetE2_L1
            cmp edi, 17
            jg CheckNewWalls_SetE2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetE2_L1:
            cmp esi, 2
            jne CheckNewWalls_SetF_Start_L1 ; Next Set
            cmp edi, 27
            jl CheckNewWalls_SetF_Start_L1
            cmp edi, 33
            jg CheckNewWalls_SetF_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetF_Start_L1:
            ; 10. New Walls Set F (Bottom connecting horizontal lines)
            ; F1. esi = 18, edi from 10 to 16.
            cmp esi, 18
            jne CheckNewWalls_SetF2_L1
            cmp edi, 11
            jl CheckNewWalls_SetF2_L1
            cmp edi, 17
            jg CheckNewWalls_SetF2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetF2_L1:
            cmp esi, 18
            jne CheckNewWalls_SetG_Start_L1 ; Next Set
            cmp edi, 27
            jl CheckNewWalls_SetG_Start_L1
            cmp edi, 33
            jg CheckNewWalls_SetG_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetG_Start_L1:
            ; 11. New Walls Set G (Top-left "T" / Pillar)
            ; G1. Horizontal top: esi = 4, edi from 10 to 14.
            cmp esi, 4
            jne CheckNewWalls_SetG2_L1
            cmp edi, 12
            jl CheckNewWalls_SetG2_L1
            cmp edi, 16
            jg CheckNewWalls_SetG2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetG2_L1: ; G2. Vertical stem: edi = 12, esi from 4 to 6.
            cmp edi, 14
            jne CheckNewWalls_SetH_Start_L1 ; Next Set
            cmp esi, 4
            jl CheckNewWalls_SetH_Start_L1
            cmp esi, 6
            jg CheckNewWalls_SetH_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetH_Start_L1:
            ; 12. New Walls Set H (Bottom-left "T" / Pillar)
            ; H1. Horizontal bottom: esi = 16, edi from 10 to 14.
            cmp esi, 16
            jne CheckNewWalls_SetH2_L1
            cmp edi, 12
            jl CheckNewWalls_SetH2_L1
            cmp edi, 16
            jg CheckNewWalls_SetH2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetH2_L1: ; H2. Vertical stem: edi = 12, esi from 14 to 16.
            cmp edi, 14
            jne CheckNewWalls_SetI_Start_L1 ; Next Set
            cmp esi, 14
            jl CheckNewWalls_SetI_Start_L1
            cmp esi, 16
            jg CheckNewWalls_SetI_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetI_Start_L1:
            ; 13. New Walls Set I (Top-right "T" / Pillar)
            ; I1. Horizontal top: esi = 4, edi from 30 to 34.
            cmp esi, 4
            jne CheckNewWalls_SetI2_L1
            cmp edi, 28
            jl CheckNewWalls_SetI2_L1
            cmp edi, 32
            jg CheckNewWalls_SetI2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetI2_L1: ; I2. Vertical stem: edi = 32, esi from 4 to 6.
            cmp edi, 30
            jne CheckNewWalls_SetJ_Start_L1 ; Next Set
            cmp esi, 4
            jl CheckNewWalls_SetJ_Start_L1
            cmp esi, 6
            jg CheckNewWalls_SetJ_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetJ_Start_L1:
            ; 14. New Walls Set J (Bottom-right "T" / Pillar)
            ; J1. Horizontal bottom
            cmp esi, 16
            jne CheckNewWalls_SetJ2_L1
            cmp edi, 28
            jl CheckNewWalls_SetJ2_L1
            cmp edi, 32
            jg CheckNewWalls_SetJ2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetJ2_L1: ; J2. Vertical stem
            cmp edi, 30
            jne CheckNewWalls_SetK_Start_L1 ; Next Set
            cmp esi, 14
            jl CheckNewWalls_SetK_Start_L1
            cmp esi, 16
            jg CheckNewWalls_SetK_Start_L1
            jmp StoreWall_L1

        CheckNewWalls_SetK_Start_L1:
            ; 15. New Walls Set K (Small horizontal segments near central '2')
            ; K1. esi = 8, edi from 14 to 16.
            cmp esi, 8
            jne CheckNewWalls_SetK2_L1
            cmp edi, 13
            jl CheckNewWalls_SetK2_L1
            cmp edi, 15
            jg CheckNewWalls_SetK2_L1
            jmp StoreWall_L1
        CheckNewWalls_SetK2_L1: ; K2. esi = 12, edi from 14 to 16.
            cmp esi, 12
            jne CheckNewWalls_SetK3_L1
            cmp edi, 13
            jl CheckNewWalls_SetK3_L1
            cmp edi, 15
            jg CheckNewWalls_SetK3_L1
            jmp StoreWall_L1
        CheckNewWalls_SetK3_L1: ; K3. esi = 8, edi from 28 to 30.
            cmp esi, 8
            jne CheckNewWalls_SetK4_L1
            cmp edi, 29
            jl CheckNewWalls_SetK4_L1
            cmp edi, 31
            jg CheckNewWalls_SetK4_L1
            jmp StoreWall_L1
        CheckNewWalls_SetK4_L1: ; K4. esi = 12, edi from 28 to 30.
            cmp esi, 12
            jne CheckPacmanPos_L1 ; All wall checks done, now check Pacman
            cmp edi, 29
            jl CheckPacmanPos_L1
            cmp edi, 31
            jg CheckPacmanPos_L1
            jmp StoreWall_L1      ; It's this small segment

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
            mov al, 50h         ; Character for Pacman (assuming 'P')
            ; jmp MoveCharacter_L1 ; Fall through

        MoveCharacter_L1:
            mov [GRID + ebx * TYPE GRID], al  ; Store character in grid
            inc ebx             ; Increment grid index
            dec ecx
            jnz InnerLoopL1    ; Decrement ecx (inner loop counter) and loop if not zero

        pop ecx             ; Restore OuterLoop counter
        dec ecx
        jnz OuterLoopL1
    ret
initialiseLevel1 ENDP

initialiseLevel2 PROC
    mov ecx, GRID_HEIGHT
    mov esi, -1         ; row counter (0 to GRID_HEIGHT-1)
    mov ebx, 0          ; grid index

    OuterLoopL2:
        inc esi
        push ecx            ; save OuterLoop counter
        mov ecx, GRID_WIDTH
        mov edi, -1         ; column counter (0 to GRID_WIDTH-1)

    InnerLoopL2:
        inc edi

        ; 1. Check for Outer Boundary Walls
        cmp esi, 0
        je StoreWall_L2
        cmp esi, GRID_HEIGHT-1
        je StoreWall_L2
        cmp edi, 0
        je StoreWall_L2
        cmp edi, GRID_WIDTH-1
        je StoreWall_L2

        ; 2. User's Vertical Wall at edi = 9
        cmp edi, 9
        jne CheckTwoShape_Entry_L2
        cmp esi, 4
        jl CheckTwoShape_Entry_L2
        cmp esi, 16
        jg CheckTwoShape_Entry_L2
        jmp StoreWall_L2

    CheckTwoShape_Entry_L2:
        ; 3. User's '2' Shape Walls
        ; Top horizontal bar of '2': esi = 6, edi from 18 to 26
        cmp esi, 6
        jne CheckTwoShape_MiddlePart_L2
        cmp edi, 18
        jl CheckOneShape_Entry_L2
        cmp edi, 26
        jg CheckOneShape_Entry_L2
        jmp StoreWall_L2

    CheckTwoShape_MiddlePart_L2:
        ; Right vertical bar of '2': edi = 26, esi from 6 to 10
        cmp edi, 26
        jne CheckTwoShape_BottomPart_L2
        cmp esi, 6
        jl CheckOneShape_Entry_L2
        cmp esi, 10
        jg CheckOneShape_Entry_L2
        jmp StoreWall_L2

    CheckTwoShape_BottomPart_L2:
        ; Middle horizontal bar of '2': esi = 10, edi from 18 to 26
        cmp esi, 10
        jne CheckTwoShape_LeftPart_L2
        cmp edi, 18
        jl CheckOneShape_Entry_L2
        cmp edi, 26
        jg CheckOneShape_Entry_L2
        jmp StoreWall_L2

    CheckTwoShape_LeftPart_L2:
        ; Left vertical bar of '2': edi = 18, esi from 10 to 14
        cmp edi, 18
        jne CheckTwoShape_HorizontalBottom_L2
        cmp esi, 10
        jl CheckOneShape_Entry_L2
        cmp esi, 14
        jg CheckOneShape_Entry_L2
        jmp StoreWall_L2

    CheckTwoShape_HorizontalBottom_L2:
        ; Bottom most horizontal bar of '2': esi = 14, edi from 18 to 26
        cmp esi, 14
        jne CheckOneShape_Entry_L2
        cmp edi, 18
        jl CheckOneShape_Entry_L2
        cmp edi, 26
        jg CheckOneShape_Entry_L2
        jmp StoreWall_L2

    CheckOneShape_Entry_L2:
        ; 4. User's Vertical Wall at edi = 35
        cmp edi, 35
        jne CheckL1Walls_SetA_Start_L2
        cmp esi, 4
        jl CheckL1Walls_SetA_Start_L2
        cmp esi, 16
        jg CheckL1Walls_SetA_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetA_Start_L2:
        ; 5. User's Walls Set A (Top-left L-shape & extensions)
        ; A1. Horizontal: esi = 2, edi from 2 to 6.
        cmp esi, 2
        jne CheckL1Walls_SetA2_L2
        cmp edi, 2
        jl CheckL1Walls_SetA2_L2
        cmp edi, 6
        jg CheckL1Walls_SetA2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetA2_L2: ; A2. Vertical: edi = 2, esi from 2 to 8.
        cmp edi, 2
        jne CheckL1Walls_SetA3_L2
        cmp esi, 2
        jl CheckL1Walls_SetA3_L2
        cmp esi, 8
        jg CheckL1Walls_SetA3_L2
        jmp StoreWall_L2
    CheckL1Walls_SetA3_L2: ; A3. Horizontal: esi = 8, edi from 2 to 6.
        cmp esi, 8
        jne CheckL1Walls_SetA4_L2
        cmp edi, 2
        jl CheckL1Walls_SetA4_L2
        cmp edi, 6
        jg CheckL1Walls_SetA4_L2
        jmp StoreWall_L2
    CheckL1Walls_SetA4_L2: ; A4. Vertical: edi = 6, esi from 2 to 4.
        cmp edi, 6
        jne CheckL1Walls_SetA5_L2
        cmp esi, 2
        jl CheckL1Walls_SetA5_L2
        cmp esi, 4
        jg CheckL1Walls_SetA5_L2
        jmp StoreWall_L2
    CheckL1Walls_SetA5_L2: ; A5. Horizontal: esi = 4, edi from 4 to 6.
        cmp esi, 4
        jne CheckL1Walls_SetB_Start_L2
        cmp edi, 4
        jl CheckL1Walls_SetB_Start_L2
        cmp edi, 6
        jg CheckL1Walls_SetB_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetB_Start_L2:
        ; 6. User's Walls Set B (Bottom-left L-shape & extensions)
        ; B1. Horizontal: esi = 12, edi from 2 to 6.
        cmp esi, 12
        jne CheckL1Walls_SetB2_L2
        cmp edi, 2
        jl CheckL1Walls_SetB2_L2
        cmp edi, 6
        jg CheckL1Walls_SetB2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetB2_L2: ; B2. Vertical: edi = 2, esi from 12 to 18.
        cmp edi, 2
        jne CheckL1Walls_SetB3_L2
        cmp esi, 12
        jl CheckL1Walls_SetB3_L2
        cmp esi, 18
        jg CheckL1Walls_SetB3_L2
        jmp StoreWall_L2
    CheckL1Walls_SetB3_L2: ; B3. Horizontal: esi = 18, edi from 2 to 6.
        cmp esi, 18
        jne CheckL1Walls_SetB4_L2
        cmp edi, 2
        jl CheckL1Walls_SetB4_L2
        cmp edi, 6
        jg CheckL1Walls_SetB4_L2
        jmp StoreWall_L2
    CheckL1Walls_SetB4_L2: ; B4. Vertical: edi = 6, esi from 16 to 18.
        cmp edi, 6
        jne CheckL1Walls_SetB5_L2
        cmp esi, 16
        jl CheckL1Walls_SetB5_L2
        cmp esi, 18
        jg CheckL1Walls_SetB5_L2
        jmp StoreWall_L2
    CheckL1Walls_SetB5_L2: ; B5. Horizontal: esi = 16, edi from 4 to 6.
        cmp esi, 16
        jne CheckL1Walls_SetC_Start_L2
        cmp edi, 4
        jl CheckL1Walls_SetC_Start_L2
        cmp edi, 6
        jg CheckL1Walls_SetC_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetC_Start_L2:
        ; 7. User's Walls Set C (Top-right L-shape & extensions)
        ; C1. Horizontal: esi = 2, edi from 38 to 42.
        cmp esi, 2
        jne CheckL1Walls_SetC2_L2
        cmp edi, 38
        jl CheckL1Walls_SetC2_L2
        cmp edi, 42
        jg CheckL1Walls_SetC2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetC2_L2: ; C2. Vertical: edi = 42, esi from 2 to 8.
        cmp edi, 42
        jne CheckL1Walls_SetC3_L2
        cmp esi, 2
        jl CheckL1Walls_SetC3_L2
        cmp esi, 8
        jg CheckL1Walls_SetC3_L2
        jmp StoreWall_L2
    CheckL1Walls_SetC3_L2: ; C3. Horizontal: esi = 8, edi from 38 to 42.
        cmp esi, 8
        jne CheckL1Walls_SetC4_L2
        cmp edi, 38
        jl CheckL1Walls_SetC4_L2
        cmp edi, 42
        jg CheckL1Walls_SetC4_L2
        jmp StoreWall_L2
    CheckL1Walls_SetC4_L2: ; C4. Vertical: edi = 38, esi from 2 to 4.
        cmp edi, 38
        jne CheckL1Walls_SetC5_L2
        cmp esi, 2
        jl CheckL1Walls_SetC5_L2
        cmp esi, 4
        jg CheckL1Walls_SetC5_L2
        jmp StoreWall_L2
    CheckL1Walls_SetC5_L2: ; C5. Horizontal: esi = 4, edi from 38 to 40.
        cmp esi, 4
        jne CheckL1Walls_SetD_Start_L2
        cmp edi, 38
        jl CheckL1Walls_SetD_Start_L2
        cmp edi, 40
        jg CheckL1Walls_SetD_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetD_Start_L2:
        ; 8. User's Walls Set D (Bottom-right L-shape & extensions)
        ; D1. Horizontal: esi = 12, edi from 38 to 42.
        cmp esi, 12
        jne CheckL1Walls_SetD2_L2
        cmp edi, 38
        jl CheckL1Walls_SetD2_L2
        cmp edi, 42
        jg CheckL1Walls_SetD2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetD2_L2: ; D2. Vertical: edi = 42, esi from 12 to 18.
        cmp edi, 42
        jne CheckL1Walls_SetD3_L2
        cmp esi, 12
        jl CheckL1Walls_SetD3_L2
        cmp esi, 18
        jg CheckL1Walls_SetD3_L2
        jmp StoreWall_L2
    CheckL1Walls_SetD3_L2: ; D3. Horizontal: esi = 18, edi from 38 to 42.
        cmp esi, 18
        jne CheckL1Walls_SetD4_L2
        cmp edi, 38
        jl CheckL1Walls_SetD4_L2
        cmp edi, 42
        jg CheckL1Walls_SetD4_L2
        jmp StoreWall_L2
    CheckL1Walls_SetD4_L2: ; D4. Vertical: edi = 38, esi from 16 to 18.
        cmp edi, 38
        jne CheckL1Walls_SetD5_L2
        cmp esi, 16
        jl CheckL1Walls_SetD5_L2
        cmp esi, 18
        jg CheckL1Walls_SetD5_L2
        jmp StoreWall_L2
    CheckL1Walls_SetD5_L2: ; D5. Horizontal: esi = 16, edi from 38 to 40.
        cmp esi, 16
        jne CheckL1Walls_SetE_Start_L2
        cmp edi, 38
        jl CheckL1Walls_SetE_Start_L2
        cmp edi, 40
        jg CheckL1Walls_SetE_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetE_Start_L2:
        ; 9. User's Walls Set E (Top connecting horizontal lines)
        ; E1. esi = 2, edi from 11 to 17.
        cmp esi, 2
        jne CheckL1Walls_SetE2_L2
        cmp edi, 11
        jl CheckL1Walls_SetE2_L2
        cmp edi, 17
        jg CheckL1Walls_SetE2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetE2_L2: ; E2. esi = 2, edi from 27 to 33.
        cmp esi, 2
        jne CheckL1Walls_SetF_Start_L2
        cmp edi, 27
        jl CheckL1Walls_SetF_Start_L2
        cmp edi, 33
        jg CheckL1Walls_SetF_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetF_Start_L2:
        ; 10. User's Walls Set F (Bottom connecting horizontal lines)
        ; F1. esi = 18, edi from 11 to 17.
        cmp esi, 18
        jne CheckL1Walls_SetF2_L2
        cmp edi, 11
        jl CheckL1Walls_SetF2_L2
        cmp edi, 17
        jg CheckL1Walls_SetF2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetF2_L2: ; F2. esi = 18, edi from 27 to 33.
        cmp esi, 18
        jne CheckL1Walls_SetG_Start_L2
        cmp edi, 27
        jl CheckL1Walls_SetG_Start_L2
        cmp edi, 33
        jg CheckL1Walls_SetG_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetG_Start_L2:
        ; 11. User's Walls Set G (Top-left "T" / Pillar)
        ; G1. Horizontal top: esi = 4, edi from 12 to 16.
        cmp esi, 4
        jne CheckL1Walls_SetG2_L2
        cmp edi, 12
        jl CheckL1Walls_SetG2_L2
        cmp edi, 16
        jg CheckL1Walls_SetG2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetG2_L2: ; G2. Vertical stem: edi = 14, esi from 4 to 6.
        cmp edi, 14
        jne CheckL1Walls_SetH_Start_L2
        cmp esi, 4
        jl CheckL1Walls_SetH_Start_L2
        cmp esi, 6
        jg CheckL1Walls_SetH_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetH_Start_L2:
        ; 12. User's Walls Set H (Bottom-left "T" / Pillar)
        ; H1. Horizontal bottom: esi = 16, edi from 12 to 16.
        cmp esi, 16
        jne CheckL1Walls_SetH2_L2
        cmp edi, 12
        jl CheckL1Walls_SetH2_L2
        cmp edi, 16
        jg CheckL1Walls_SetH2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetH2_L2: ; H2. Vertical stem: edi = 14, esi from 14 to 16.
        cmp edi, 14
        jne CheckL1Walls_SetI_Start_L2
        cmp esi, 14
        jl CheckL1Walls_SetI_Start_L2
        cmp esi, 16
        jg CheckL1Walls_SetI_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetI_Start_L2:
        ; 13. User's Walls Set I (Top-right "T" / Pillar)
        ; I1. Horizontal top: esi = 4, edi from 28 to 32.
        cmp esi, 4
        jne CheckL1Walls_SetI2_L2
        cmp edi, 28
        jl CheckL1Walls_SetI2_L2
        cmp edi, 32
        jg CheckL1Walls_SetI2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetI2_L2: ; I2. Vertical stem: edi = 30, esi from 4 to 6.
        cmp edi, 30
        jne CheckL1Walls_SetJ_Start_L2
        cmp esi, 4
        jl CheckL1Walls_SetJ_Start_L2
        cmp esi, 6
        jg CheckL1Walls_SetJ_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetJ_Start_L2:
        ; 14. User's Walls Set J (Bottom-right "T" / Pillar)
        ; J1. Horizontal bottom: esi = 16, edi from 28 to 32.
        cmp esi, 16
        jne CheckL1Walls_SetJ2_L2
        cmp edi, 28
        jl CheckL1Walls_SetJ2_L2
        cmp edi, 32
        jg CheckL1Walls_SetJ2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetJ2_L2: ; J2. Vertical stem: edi = 30, esi from 14 to 16.
        cmp edi, 30
        jne CheckL1Walls_SetK_Start_L2
        cmp esi, 14
        jl CheckL1Walls_SetK_Start_L2
        cmp esi, 16
        jg CheckL1Walls_SetK_Start_L2
        jmp StoreWall_L2

    CheckL1Walls_SetK_Start_L2:
        ; 15. User's Walls Set K (Small horizontal segments)
        ; K1. esi = 8, edi from 13 to 15.
        cmp esi, 8
        jne CheckL1Walls_SetK2_L2
        cmp edi, 13
        jl CheckL1Walls_SetK2_L2
        cmp edi, 15
        jg CheckL1Walls_SetK2_L2
        jmp StoreWall_L2
    CheckL1Walls_SetK2_L2: ; K2. esi = 12, edi from 13 to 15.
        cmp esi, 12
        jne CheckL1Walls_SetK3_L2
        cmp edi, 13
        jl CheckL1Walls_SetK3_L2
        cmp edi, 15
        jg CheckL1Walls_SetK3_L2
        jmp StoreWall_L2
    CheckL1Walls_SetK3_L2: ; K3. esi = 8, edi from 29 to 31.
        cmp esi, 8
        jne CheckL1Walls_SetK4_L2
        cmp edi, 29
        jl CheckL1Walls_SetK4_L2
        cmp edi, 31
        jg CheckL1Walls_SetK4_L2
        jmp StoreWall_L2
    CheckL1Walls_SetK4_L2: ; K4. esi = 12, edi from 29 to 31.
        cmp esi, 12
        jne CheckL2_CorridorBaffles1_L2
        cmp edi, 29
        jl CheckL2_CorridorBaffles1_L2
        cmp edi, 31
        jg CheckL2_CorridorBaffles1_L2
        jmp StoreWall_L2

    ; --- START OF LEVEL 2 ORIGINAL ADDITIONS ---
    CheckL2_CorridorBaffles1_L2:
        ; 16. L2 Walls: Corridor Baffles
        ; Top corridor: esi=1, edi=15 OR edi=29
        cmp esi, 1
        jne CheckL2_CorridorBaffles2_L2
        cmp edi, 15
        je StoreWall_L2
        cmp edi, 29
        je StoreWall_L2
    CheckL2_CorridorBaffles2_L2:
        ; Bottom corridor: esi=20, edi=15 OR edi=29
        cmp esi, 20 ; GRID_HEIGHT-2 = 20
        jne CheckL2_K_Connectors_V1_L2
        cmp edi, 15
        je StoreWall_L2
        cmp edi, 29
        je StoreWall_L2

    CheckL2_K_Connectors_V1_L2:
        ; 17. L2 Walls: K-Block Vertical Connectors (Left Side)
        ; edi=13, esi from 9 to 11
        cmp edi, 13
        jne CheckL2_K_Connectors_V2_L2
        cmp esi, 9
        jl CheckL2_K_Connectors_V2_L2
        cmp esi, 11
        jg CheckL2_K_Connectors_V2_L2
        jmp StoreWall_L2
    CheckL2_K_Connectors_V2_L2:
        ; edi=15, esi from 9 to 11
        cmp edi, 15
        jne CheckL2_K_Connectors_V3_L2
        cmp esi, 9
        jl CheckL2_K_Connectors_V3_L2
        cmp esi, 11
        jg CheckL2_K_Connectors_V3_L2
        jmp StoreWall_L2

    CheckL2_K_Connectors_V3_L2:
        ; K-Block Vertical Connectors (Right Side)
        ; edi=29, esi from 9 to 11
        cmp edi, 29
        jne CheckL2_K_Connectors_V4_L2
        cmp esi, 9
        jl CheckL2_K_Connectors_V4_L2
        cmp esi, 11
        jg CheckL2_K_Connectors_V4_L2
        jmp StoreWall_L2
    CheckL2_K_Connectors_V4_L2:
        ; edi=31, esi from 9 to 11
        cmp edi, 31
        jne CheckL2_FillZeroes_Left_L2 ; Proceed to Zero filling
        cmp esi, 9
        jl CheckL2_FillZeroes_Left_L2
        cmp esi, 11
        jg CheckL2_FillZeroes_Left_L2
        jmp StoreWall_L2

    CheckL2_FillZeroes_Left_L2:
        ; 18. L2 User Mod: Fill Left "Zero"
        ; edi=14, esi from 9 to 11
        cmp edi, 14
        jne CheckL2_FillZeroes_Right_L2
        cmp esi, 9
        jl CheckL2_FillZeroes_Right_L2
        cmp esi, 11
        jg CheckL2_FillZeroes_Right_L2
        jmp StoreWall_L2

    CheckL2_FillZeroes_Right_L2:
        ; 19. L2 User Mod: Fill Right "Zero"
        ; edi=30, esi from 9 to 11
        cmp edi, 30
        jne CheckL2_LShape_Internal_A1_L2
        cmp esi, 9
        jl CheckL2_LShape_Internal_A1_L2
        cmp esi, 11
        jg CheckL2_LShape_Internal_A1_L2
        jmp StoreWall_L2

    CheckL2_LShape_Internal_A1_L2:
        ; 20. L2 Walls: Original L-Shape Internal Blocks
        ; Top-Left L-Shape (Set A related): esi=5, edi=4 OR edi=5
        cmp esi, 5
        jne CheckL2_LShape_Internal_B1_L2
        cmp edi, 4
        je StoreWall_L2
        cmp edi, 5
        je StoreWall_L2
        ; If esi is 5 but edi not 4 or 5, continue
    CheckL2_LShape_Internal_B1_L2:
        ; Bottom-Left L-Shape (Set B related): esi=15, edi=4 OR edi=5
        cmp esi, 15
        jne CheckL2_LShape_Internal_C1_L2
        cmp edi, 4
        je StoreWall_L2
        cmp edi, 5
        je StoreWall_L2
        ; If esi is 15 but edi not 4 or 5, continue
    CheckL2_LShape_Internal_C1_L2:
        ; Top-Right L-Shape (Set C related): esi=5, edi=39 OR edi=40
        cmp esi, 5
        jne CheckL2_LShape_Internal_D1_L2
        cmp edi, 39
        je StoreWall_L2
        cmp edi, 40
        je StoreWall_L2
        ; If esi is 5 but edi not 39 or 40, continue
    CheckL2_LShape_Internal_D1_L2:
        ; Bottom-Right L-Shape (Set D related): esi=15, edi=39 OR edi=40
        cmp esi, 15
        jne CheckL2_NewCaps_Top_L2 ; Proceed to new cap walls
        cmp edi, 39
        je StoreWall_L2
        cmp edi, 40
        je StoreWall_L2
        ; If esi is 15 but edi not 39 or 40, continue

    CheckL2_NewCaps_Top_L2:
        ; 21. L2 User Mod: New Wall - Top Cap for '2'
        cmp esi, 4
        jne CheckL2_NewCaps_Bottom_L2
        cmp edi, 21
        jl CheckL2_NewCaps_Bottom_L2
        cmp edi, 23
        jg CheckL2_NewCaps_Bottom_L2
        jmp StoreWall_L2

    CheckL2_NewCaps_Bottom_L2:
        ; 22. L2 User Mod: New Wall - Bottom Cap for '2'
        cmp esi, 16
        jne CheckL2_NewLProtrusion_TL_L2
        cmp edi, 21
        jl CheckL2_NewLProtrusion_TL_L2
        cmp edi, 23
        jg CheckL2_NewLProtrusion_TL_L2
        jmp StoreWall_L2

    CheckL2_NewLProtrusion_TL_L2:
        ; 23. L2 User Mod: New Wall - Inner L-Shape Protrusion (Top-Left)
        ; edi = 4, esi = 7
        cmp edi, 4
        jne CheckL2_NewLProtrusion_TR_L2
        cmp esi, 7
        je StoreWall_L2
        ; If edi is 4 but esi not 7, continue

    CheckL2_NewLProtrusion_TR_L2:
        ; 24. L2 User Mod: New Wall - Inner L-Shape Protrusion (Top-Right)
        ; edi = 40, esi = 7
        cmp edi, 40
        jne CheckL2_NewLProtrusion_BL_L2
        cmp esi, 7
        je StoreWall_L2
        ; If edi is 40 but esi not 7, continue

    CheckL2_NewLProtrusion_BL_L2:
        ; 25. L2 User Mod: New Wall - Inner L-Shape Protrusion (Bottom-Left)
        ; edi = 4, esi = 13
        cmp edi, 4
        jne CheckL2_NewLProtrusion_BR_L2
        cmp esi, 13
        je StoreWall_L2
        ; If edi is 4 but esi not 13, continue

    CheckL2_NewLProtrusion_BR_L2:
        ; 26. L2 User Mod: New Wall - Inner L-Shape Protrusion (Bottom-Right)
        ; edi = 40, esi = 13
        cmp edi, 40
        jne CheckPacmanPos_L2 ; All wall checks done
        cmp esi, 13
        je StoreWall_L2
        ; If edi is 40 but esi not 13, continue to Pacman
    ; --- END OF USER REQUESTED MODIFICATIONS ---

    CheckPacmanPos_L2:
        cmp esi, pacmanRow
        jne StoreDot_L2
        cmp edi, pacmanCol
        jne StoreDot_L2
        jmp StorePacman_L2

    StoreWall_L2:
        mov al, wallChar
        jmp MoveCharacter_L2

    StoreDot_L2:
        mov al, dotChar
        jmp MoveCharacter_L2

    StorePacman_L2:
        mov al, 50h         ; Character for Pacman (ASCII 'P')
        ; jmp MoveCharacter_L2 ; Fall through

    MoveCharacter_L2:
        mov [GRID + ebx * TYPE GRID], al
        inc ebx
        dec ecx
        jnz InnerLoopL2

        pop ecx
        dec ecx
        jnz OuterLoopL2
    ret
initialiseLevel2 ENDP

initialiseLevel3 PROC
    mov ecx, GRID_HEIGHT
    mov esi, -1         ; row counter (0 to GRID_HEIGHT-1)
    mov ebx, 0          ; grid index

    OuterLoopL3:
        inc esi
        push ecx            ; save OuterLoop counter
        mov ecx, GRID_WIDTH
        mov edi, -1         ; column counter (0 to GRID_WIDTH-1)

    InnerLoopL3:
        inc edi

        ; Check for Outer Boundary Walls
        cmp esi, 0
        je StoreWall_L3
        cmp esi, GRID_HEIGHT-1
        je StoreWall_L3
        cmp edi, 0
        je StoreWall_L3
        cmp edi, GRID_WIDTH-1
        je StoreWall_L3

        ; User's Vertical Wall at edi = 9 (Core Structure 1 - Left)
        cmp edi, 9
        jne CheckTwoShape_Entry_L3
        cmp esi, 4
        jl CheckTwoShape_Entry_L3
        cmp esi, 16
        jg CheckTwoShape_Entry_L3
        jmp StoreWall_L3

    CheckTwoShape_Entry_L3:
        ; User's '2' Shape Walls (Core Structure 2 - Center)
        ; Top horizontal bar of '2': esi = 6, edi from 18 to 26
        cmp esi, 6
        jne CheckTwoShape_MiddlePart_L3
        cmp edi, 18
        jl CheckOneShape_Entry_L3
        cmp edi, 26
        jg CheckOneShape_Entry_L3
        jmp StoreWall_L3

    CheckTwoShape_MiddlePart_L3:
        ; Right vertical bar of '2': edi = 26, esi from 6 to 10
        cmp edi, 26
        jne CheckTwoShape_BottomPart_L3
        cmp esi, 6
        jl CheckOneShape_Entry_L3
        cmp esi, 10
        jg CheckOneShape_Entry_L3
        jmp StoreWall_L3

    CheckTwoShape_BottomPart_L3:
        ; Middle horizontal bar of '2': esi = 10, edi from 18 to 26
        cmp esi, 10
        jne CheckTwoShape_LeftPart_L3
        cmp edi, 18
        jl CheckOneShape_Entry_L3
        cmp edi, 26
        jg CheckOneShape_Entry_L3
        jmp StoreWall_L3

    CheckTwoShape_LeftPart_L3:
        ; Left vertical bar of '2': edi = 18, esi from 10 to 14
        cmp edi, 18
        jne CheckTwoShape_HorizontalBottom_L3
        cmp esi, 10
        jl CheckOneShape_Entry_L3
        cmp esi, 14
        jg CheckOneShape_Entry_L3
        jmp StoreWall_L3

    CheckTwoShape_HorizontalBottom_L3:
        ; Bottom most horizontal bar of '2': esi = 14, edi from 18 to 26
        cmp esi, 14
        jne CheckOneShape_Entry_L3
        cmp edi, 18
        jl CheckOneShape_Entry_L3
        cmp edi, 26
        jg CheckOneShape_Entry_L3
        jmp StoreWall_L3

    CheckOneShape_Entry_L3:
        ; User's Vertical Wall at edi = 35 (Core Structure 3 - Right)
        cmp edi, 35
        jne CheckL3_H_Shape_TL_V1_L3
        cmp esi, 4
        jl CheckL3_H_Shape_TL_V1_L3
        cmp esi, 16
        jg CheckL3_H_Shape_TL_V1_L3
        jmp StoreWall_L3

    CheckL3_H_Shape_TL_V1_L3: ; Top-Left H-Shape: Vertical 1
        cmp edi, 4
        jne CheckL3_H_Shape_TL_V2_L3
        cmp esi, 2
        jl CheckL3_H_Shape_TL_V2_L3
        cmp esi, 4
        jg CheckL3_H_Shape_TL_V2_L3
        jmp StoreWall_L3
    CheckL3_H_Shape_TL_V2_L3: ; Top-Left H-Shape: Vertical 2
        cmp edi, 6
        jne CheckL3_H_Shape_TL_H_L3_Orig ; Changed label to avoid conflict with extended
        cmp esi, 2
        jl CheckL3_H_Shape_TL_H_L3_Orig
        cmp esi, 4
        jg CheckL3_H_Shape_TL_H_L3_Orig
        jmp StoreWall_L3
    CheckL3_H_Shape_TL_H_L3_Orig: ; Top-Left H-Shape: Original Horizontal Connector (kept for clarity, new one below)
        cmp esi, 3
        jne CheckL3_H_Shape_TR_V1_L3
        cmp edi, 4
        jl CheckL3_H_Shape_TR_V1_L3
        cmp edi, 6
        jg CheckL3_H_Shape_TR_V1_L3
        jmp StoreWall_L3

    CheckL3_H_Shape_TR_V1_L3: ; Top-Right H-Shape: Vertical 1 (edi=38)
        cmp edi, 38
        jne CheckL3_H_Shape_TR_V2_L3
        cmp esi, 2
        jl CheckL3_H_Shape_TR_V2_L3
        cmp esi, 4
        jg CheckL3_H_Shape_TR_V2_L3
        jmp StoreWall_L3
    CheckL3_H_Shape_TR_V2_L3: ; Top-Right H-Shape: Vertical 2 (edi=40)
        cmp edi, 40
        jne CheckL3_H_Shape_TR_H_L3_Orig ; Changed label
        cmp esi, 2
        jl CheckL3_H_Shape_TR_H_L3_Orig
        cmp esi, 4
        jg CheckL3_H_Shape_TR_H_L3_Orig
        jmp StoreWall_L3
    CheckL3_H_Shape_TR_H_L3_Orig: ; Top-Right H-Shape: Original Horizontal Connector
        cmp esi, 3
        jne CheckL3_H_Shape_BL_V1_L3
        cmp edi, 38
        jl CheckL3_H_Shape_BL_V1_L3
        cmp edi, 40
        jg CheckL3_H_Shape_BL_V1_L3
        jmp StoreWall_L3

    CheckL3_H_Shape_BL_V1_L3: ; Bottom-Left H-Shape: Vertical 1
        cmp edi, 4
        jne CheckL3_H_Shape_BL_V2_L3
        cmp esi, 17
        jl CheckL3_H_Shape_BL_V2_L3
        cmp esi, 19
        jg CheckL3_H_Shape_BL_V2_L3
        jmp StoreWall_L3
    CheckL3_H_Shape_BL_V2_L3: ; Bottom-Left H-Shape: Vertical 2
        cmp edi, 6
        jne CheckL3_H_Shape_BL_H_L3_Orig ; Changed label
        cmp esi, 17
        jl CheckL3_H_Shape_BL_H_L3_Orig
        cmp esi, 19
        jg CheckL3_H_Shape_BL_H_L3_Orig
        jmp StoreWall_L3
    CheckL3_H_Shape_BL_H_L3_Orig: ; Bottom-Left H-Shape: Original Horizontal Connector
        cmp esi, 18
        jne CheckL3_H_Shape_BR_V1_L3
        cmp edi, 4
        jl CheckL3_H_Shape_BR_V1_L3
        cmp edi, 6
        jg CheckL3_H_Shape_BR_V1_L3
        jmp StoreWall_L3

    CheckL3_H_Shape_BR_V1_L3: ; Bottom-Right H-Shape: Vertical 1 (edi=38)
        cmp edi, 38
        jne CheckL3_H_Shape_BR_V2_L3
        cmp esi, 17
        jl CheckL3_H_Shape_BR_V2_L3
        cmp esi, 19
        jg CheckL3_H_Shape_BR_V2_L3
        jmp StoreWall_L3
    CheckL3_H_Shape_BR_V2_L3: ; Bottom-Right H-Shape: Vertical 2 (edi=40)
        cmp edi, 40
        jne CheckL3_H_Shape_BR_H_L3_Orig ; Changed label
        cmp esi, 17
        jl CheckL3_H_Shape_BR_H_L3_Orig
        cmp esi, 19
        jg CheckL3_H_Shape_BR_H_L3_Orig
        jmp StoreWall_L3
    CheckL3_H_Shape_BR_H_L3_Orig: ; Bottom-Right H-Shape: Original Horizontal Connector
        cmp esi, 18
        jne CheckL3_T_Blockade_Top_H_L3
        cmp edi, 38
        jl CheckL3_T_Blockade_Top_H_L3
        cmp edi, 40
        jg CheckL3_T_Blockade_Top_H_L3
        jmp StoreWall_L3

    CheckL3_T_Blockade_Top_H_L3: ; Top T-Blockade: Horizontal Bar
        cmp esi, 4
        jne CheckL3_T_Blockade_Top_V_L3
        cmp edi, 21
        jl CheckL3_T_Blockade_Top_V_L3
        cmp edi, 23
        jg CheckL3_T_Blockade_Top_V_L3
        jmp StoreWall_L3
    CheckL3_T_Blockade_Top_V_L3: ; Top T-Blockade: Vertical Stem
        cmp edi, 22
        jne CheckL3_T_Blockade_Bottom_H_L3
        cmp esi, 5
        jne CheckL3_T_Blockade_Bottom_H_L3
        jmp StoreWall_L3

    CheckL3_T_Blockade_Bottom_H_L3: ; Bottom T-Blockade: Horizontal Bar
        cmp esi, 16
        jne CheckL3_T_Blockade_Bottom_V_L3
        cmp edi, 21
        jl CheckL3_T_Blockade_Bottom_V_L3
        cmp edi, 23
        jg CheckL3_T_Blockade_Bottom_V_L3
        jmp StoreWall_L3
    CheckL3_T_Blockade_Bottom_V_L3: ; Bottom T-Blockade: Vertical Stem
        cmp edi, 22
        jne CheckL3_Cross_Left_H_L3
        cmp esi, 15
        jne CheckL3_Cross_Left_H_L3
        jmp StoreWall_L3

    CheckL3_Cross_Left_H_L3: ; Left Cross-Shape: Horizontal Part
        cmp esi, 10
        jne CheckL3_Cross_Left_V_L3
        cmp edi, 3
        jl CheckL3_Cross_Left_V_L3
        cmp edi, 5
        jg CheckL3_Cross_Left_V_L3
        jmp StoreWall_L3
    CheckL3_Cross_Left_V_L3: ; Left Cross-Shape: Vertical Part
        cmp edi, 4
        jne CheckL3_Cross_Right_H_L3
        cmp esi, 9
        jl CheckL3_Cross_Right_H_L3
        cmp esi, 11
        jg CheckL3_Cross_Right_H_L3
        jmp StoreWall_L3

    CheckL3_Cross_Right_H_L3: ; Right Cross-Shape: Horizontal Part
        cmp esi, 10
        jne CheckL3_Cross_Right_V_L3
        cmp edi, 39
        jl CheckL3_Cross_Right_V_L3
        cmp edi, 41
        jg CheckL3_Cross_Right_V_L3
        jmp StoreWall_L3
    CheckL3_Cross_Right_V_L3: ; Right Cross-Shape: Vertical Part
        cmp edi, 40
        jne CheckL3_NEW_CB_Top_Left_L3
        cmp esi, 9
        jl CheckL3_NEW_CB_Top_Left_L3
        cmp esi, 11
        jg CheckL3_NEW_CB_Top_Left_L3
        jmp StoreWall_L3

    CheckL3_NEW_CB_Top_Left_L3: ; Top Corridor, Left Block
        cmp esi, 1
        jne CheckL3_NEW_CB_Top_Right_L3
        cmp edi, 10
        jl CheckL3_NEW_CB_Top_Right_L3
        cmp edi, 11
        jg CheckL3_NEW_CB_Top_Right_L3
        jmp StoreWall_L3
    CheckL3_NEW_CB_Top_Right_L3: ; Top Corridor, Right Block
        cmp esi, 1
        jne CheckL3_NEW_CB_Bottom_Left_L3
        cmp edi, 33
        jl CheckL3_NEW_CB_Bottom_Left_L3
        cmp edi, 34
        jg CheckL3_NEW_CB_Bottom_Left_L3
        jmp StoreWall_L3
    CheckL3_NEW_CB_Bottom_Left_L3: ; Bottom Corridor, Left Block
        cmp esi, 20
        jne CheckL3_NEW_CB_Bottom_Right_L3
        cmp edi, 10
        jl CheckL3_NEW_CB_Bottom_Right_L3
        cmp edi, 11
        jg CheckL3_NEW_CB_Bottom_Right_L3
        jmp StoreWall_L3
    CheckL3_NEW_CB_Bottom_Right_L3: ; Bottom Corridor, Right Block
        cmp esi, 20
        jne CheckL3_NEW_SCF_Left_Top_L3
        cmp edi, 33
        jl CheckL3_NEW_SCF_Left_Top_L3
        cmp edi, 34
        jg CheckL3_NEW_SCF_Left_Top_L3
        jmp StoreWall_L3

    CheckL3_NEW_SCF_Left_Top_L3: ; Left Channel, Top Filler
        cmp esi, 5
        jne CheckL3_NEW_SCF_Left_Bottom_L3
        cmp edi, 11
        jl CheckL3_NEW_SCF_Left_Bottom_L3
        cmp edi, 12
        jg CheckL3_NEW_SCF_Left_Bottom_L3
        jmp StoreWall_L3
    CheckL3_NEW_SCF_Left_Bottom_L3: ; Left Channel, Bottom Filler
        cmp esi, 16
        jne CheckL3_NEW_SCF_Right_Top_L3
        cmp edi, 11
        jl CheckL3_NEW_SCF_Right_Top_L3
        cmp edi, 12
        jg CheckL3_NEW_SCF_Right_Top_L3
        jmp StoreWall_L3
    CheckL3_NEW_SCF_Right_Top_L3: ; Right Channel, Top Filler
        cmp esi, 5
        jne CheckL3_NEW_SCF_Right_Bottom_L3
        cmp edi, 32
        jl CheckL3_NEW_SCF_Right_Bottom_L3
        cmp edi, 33
        jg CheckL3_NEW_SCF_Right_Bottom_L3
        jmp StoreWall_L3
    CheckL3_NEW_SCF_Right_Bottom_L3: ; Right Channel, Bottom Filler
        cmp esi, 16
        jne CheckL3_NEW_CFB_Left_Top_L3
        cmp edi, 32
        jl CheckL3_NEW_CFB_Left_Top_L3
        cmp edi, 33
        jg CheckL3_NEW_CFB_Left_Top_L3
        jmp StoreWall_L3

    CheckL3_NEW_CFB_Left_Top_L3: ; Left of '2', Top Block
        cmp edi, 16
        jne CheckL3_NEW_CFB_Left_Bottom_L3
        cmp esi, 7
        jl CheckL3_NEW_CFB_Left_Bottom_L3
        cmp esi, 8
        jg CheckL3_NEW_CFB_Left_Bottom_L3
        jmp StoreWall_L3
    CheckL3_NEW_CFB_Left_Bottom_L3: ; Left of '2', Bottom Block
        cmp edi, 16
        jne CheckL3_NEW_CFB_Right_Top_L3
        cmp esi, 12
        jl CheckL3_NEW_CFB_Right_Top_L3
        cmp esi, 13
        jg CheckL3_NEW_CFB_Right_Top_L3
        jmp StoreWall_L3
    CheckL3_NEW_CFB_Right_Top_L3: ; Right of '2', Top Block
        cmp edi, 28
        jne CheckL3_NEW_CFB_Right_Bottom_L3
        cmp esi, 7
        jl CheckL3_NEW_CFB_Right_Bottom_L3
        cmp esi, 8
        jg CheckL3_NEW_CFB_Right_Bottom_L3
        jmp StoreWall_L3
    CheckL3_NEW_CFB_Right_Bottom_L3: ; Right of '2', Bottom Block
        cmp edi, 28
        jne CheckL3_NEW_ISB_Top_Left_L3
        cmp esi, 12
        jl CheckL3_NEW_ISB_Top_Left_L3
        cmp esi, 13
        jg CheckL3_NEW_ISB_Top_Left_L3
        jmp StoreWall_L3

    CheckL3_NEW_ISB_Top_Left_L3: ; Top-Left Inner Block
        cmp esi, 7
        jne CheckL3_NEW_ISB_Top_Right_L3
        cmp edi, 12
        jl CheckL3_NEW_ISB_Top_Right_L3
        cmp edi, 13
        jg CheckL3_NEW_ISB_Top_Right_L3
        jmp StoreWall_L3
    CheckL3_NEW_ISB_Top_Right_L3: ; Top-Right Inner Block
        cmp esi, 7
        jne CheckL3_NEW_ISB_Bottom_Left_L3
        cmp edi, 31
        jl CheckL3_NEW_ISB_Bottom_Left_L3
        cmp edi, 32
        jg CheckL3_NEW_ISB_Bottom_Left_L3
        jmp StoreWall_L3
    CheckL3_NEW_ISB_Bottom_Left_L3: ; Bottom-Left Inner Block
        cmp esi, 13
        jne CheckL3_NEW_ISB_Bottom_Right_L3
        cmp edi, 12
        jl CheckL3_NEW_ISB_Bottom_Right_L3
        cmp edi, 13
        jg CheckL3_NEW_ISB_Bottom_Right_L3
        jmp StoreWall_L3
    CheckL3_NEW_ISB_Bottom_Right_L3: ; Bottom-Right Inner Block
        cmp esi, 13
        jne CheckL3_NEW_EHSC_TL_L3 ; Jump to the newest set of walls
        cmp edi, 31
        jl CheckL3_NEW_EHSC_TL_L3
        cmp edi, 32
        jg CheckL3_NEW_EHSC_TL_L3
        jmp StoreWall_L3

    CheckL3_NEW_EHSC_TL_L3: ; H-Shape Connector Top-Left
        cmp esi, 3
        jne CheckL3_NEW_EHSC_TR_L3
        cmp edi, 3 
        jl CheckL3_NEW_EHSC_TR_L3
        cmp edi, 7
        jg CheckL3_NEW_EHSC_TR_L3
        jmp StoreWall_L3
    CheckL3_NEW_EHSC_TR_L3: ; H-Shape Connector Top-Right
        cmp esi, 3
        jne CheckL3_NEW_EHSC_BL_L3
        cmp edi, 37
        jl CheckL3_NEW_EHSC_BL_L3
        cmp edi, 41 
        jg CheckL3_NEW_EHSC_BL_L3
        jmp StoreWall_L3
    CheckL3_NEW_EHSC_BL_L3: ; H-Shape Connector Bottom-Left
        cmp esi, 18
        jne CheckL3_NEW_EHSC_BR_L3
        cmp edi, 3 
        jl CheckL3_NEW_EHSC_BR_L3
        cmp edi, 7
        jg CheckL3_NEW_EHSC_BR_L3
        jmp StoreWall_L3
    CheckL3_NEW_EHSC_BR_L3: ; H-Shape Connector Bottom-Right
        cmp esi, 18
        jne CheckL3_NEW_HBI_TL_1_L3
        cmp edi, 37 
        jl CheckL3_NEW_HBI_TL_1_L3
        cmp edi, 41 
        jg CheckL3_NEW_HBI_TL_1_L3
        jmp StoreWall_L3

    CheckL3_NEW_HBI_TL_1_L3: ; H-Shape Barrier Inside Top-Left, Arm 1
        cmp esi, 2
        jne CheckL3_NEW_HBI_TL_2_L3
        cmp edi, 5
        je StoreWall_L3
    CheckL3_NEW_HBI_TL_2_L3: ; H-Shape Barrier Inside Top-Left, Arm 2
        cmp esi, 4
        jne CheckL3_NEW_HBI_TR_1_L3
        cmp edi, 5
        je StoreWall_L3

    CheckL3_NEW_HBI_TR_1_L3: ; H-Shape Barrier Inside Top-Right, Arm 1
        cmp esi, 2
        jne CheckL3_NEW_HBI_TR_2_L3
        cmp edi, 39
        je StoreWall_L3
    CheckL3_NEW_HBI_TR_2_L3: ; H-Shape Barrier Inside Top-Right, Arm 2
        cmp esi, 4
        jne CheckL3_NEW_HBI_BL_1_L3
        cmp edi, 39
        je StoreWall_L3

    CheckL3_NEW_HBI_BL_1_L3: ; H-Shape Barrier Inside Bottom-Left, Arm 1
        cmp esi, 17
        jne CheckL3_NEW_HBI_BL_2_L3
        cmp edi, 5
        je StoreWall_L3
    CheckL3_NEW_HBI_BL_2_L3: ; H-Shape Barrier Inside Bottom-Left, Arm 2
        cmp esi, 19
        jne CheckL3_NEW_HBI_BR_1_L3
        cmp edi, 5
        je StoreWall_L3

    CheckL3_NEW_HBI_BR_1_L3: ; H-Shape Barrier Inside Bottom-Right, Arm 1
        cmp esi, 17
        jne CheckL3_NEW_HBI_BR_2_L3
        cmp edi, 39
        je StoreWall_L3
    CheckL3_NEW_HBI_BR_2_L3: ; H-Shape Barrier Inside Bottom-Right, Arm 2
        cmp esi, 19
        jne CheckL3_NEW_CTB_Top_L_L3
        cmp edi, 39
        je StoreWall_L3

    CheckL3_NEW_CTB_Top_L_L3: ; Connecting T-Blockades Top Left
        cmp esi, 4
        jne CheckL3_NEW_CTB_Top_R_L3
        cmp edi, 20 ; Next to T-blockade, towards '2'
        je StoreWall_L3
    CheckL3_NEW_CTB_Top_R_L3: ; Connecting T-Blockades Top Right
        cmp esi, 4
        jne CheckL3_NEW_CTB_Bottom_L_L3
        cmp edi, 24 ; Next to T-blockade, towards '2'
        je StoreWall_L3
    CheckL3_NEW_CTB_Bottom_L_L3: ; Connecting T-Blockades Bottom Left
        cmp esi, 16
        jne CheckL3_NEW_CTB_Bottom_R_L3
        cmp edi, 20 ; Next to T-blockade, towards '2'
        je StoreWall_L3
    CheckL3_NEW_CTB_Bottom_R_L3: ; Connecting T-Blockades Bottom Right
        cmp esi, 16
        jne CheckL3_NEW_ASCB_L_T_L3
        cmp edi, 24 ; Next to T-blockade, towards '2'
        je StoreWall_L3

    CheckL3_NEW_ASCB_L_T_L3: ; Side Channel Blocks Left Top
        cmp esi, 7
        jne CheckL3_NEW_ASCB_L_B_L3
        cmp edi, 7
        je StoreWall_L3
    CheckL3_NEW_ASCB_L_B_L3: ; Side Channel Blocks Left Bottom
        cmp esi, 14
        jne CheckL3_NEW_ASCB_R_T_L3
        cmp edi, 7
        je StoreWall_L3
    CheckL3_NEW_ASCB_R_T_L3: ; Side Channel Blocks Right Top
        cmp esi, 7
        jne CheckL3_NEW_ASCB_R_B_L3
        cmp edi, 37
        je StoreWall_L3
    CheckL3_NEW_ASCB_R_B_L3: ; Side Channel Blocks Right Bottom
        cmp esi, 14
        jne CheckPacmanPos_L3
        cmp edi, 37
        je StoreWall_L3

    CheckPacmanPos_L3:
        cmp esi, pacmanRow
        jne StoreDot_L3
        cmp edi, pacmanCol
        jne StoreDot_L3
        jmp StorePacman_L3

    StoreWall_L3:
        mov al, wallChar
        jmp MoveCharacter_L3

    StoreDot_L3:
        mov al, dotChar
        jmp MoveCharacter_L3

    StorePacman_L3:
        mov al, 50h         ; Character for Pacman (ASCII 'P')
        ; jmp MoveCharacter_L3 ; Fall through

    MoveCharacter_L3:
        mov [GRID + ebx * TYPE GRID], al
        inc ebx
        dec ecx
        jnz InnerLoopL3

        pop ecx
        dec ecx
        jnz OuterLoopL3
    ret
initialiseLevel3 ENDP
