MyProcedure1 PROC
    ; Return a value in EAX
    mov eax, 42
    ret
MyProcedure1 ENDP

; Procedure that takes parameters via registers
MyProcedure2 PROC
    ; EBX contains the input parameter
    ; Multiply the input by 2 and return in EAX
    mov eax, ebx
    add eax, eax
    ret
MyProcedure2 ENDP

; Procedure that takes parameters via stack
MyProcedure3 PROC
    push ebp
    mov ebp, esp
    ; Parameter is at [ebp+8]
    mov eax, [ebp+8]
    add eax, 10
    pop ebp
    ret 4  ; Clean up 4 bytes (1 DWORD parameter)
MyProcedure3 ENDP