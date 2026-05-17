section .text
global _start
global system_call
global infect_file
extern main

_start:
    pop    dword ecx
    mov    esi,esp
    mov     eax,ecx
    shl     eax,2
    add     eax,esi
    add     eax,4
    push    dword eax
    push    dword esi
    push    dword ecx

    call    main

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp
    mov     ebp, esp
    sub     esp, 4
    pushad

    mov     eax, [ebp+8]
    mov     ebx, [ebp+12]
    mov     ecx, [ebp+16]
    mov     edx, [ebp+20]
    int     0x80
    mov     [ebp-4], eax
    popad
    mov     eax, [ebp-4]
    add     esp, 4
    pop     ebp
    ret

infect_file:
    push    ebp
    mov     ebp, esp
    pushad

    popad
    mov     esp, ebp
    pop     ebp
    ret