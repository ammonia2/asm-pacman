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

initialiseLevel2 ENDP