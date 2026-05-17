global main
extern strlen

section .rodata
    newline db 10

section .text
main:
    push ebp
    mov ebp, esp
    pushad

    mov esi, dword [ebp+12]
    xor edi, edi

print_loop:
    cmp edi, dword [ebp+8]
    jge done

    mov ebx, dword [esi + edi*4]
    push ebx
    call strlen
    add esp, 4

    mov edx, eax
    mov ecx, ebx
    mov ebx, 1
    mov eax, 4
    int 0x80

    mov edx, 1
    mov ecx, newline
    mov ebx, 1
    mov eax, 4
    int 0x80

    inc edi
    jmp print_loop

done:
    popad
    mov eax, 0
    mov esp, ebp
    pop ebp
    ret