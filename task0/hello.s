SYS_WRITE equ 4
SYS_EXIT equ 1
STDOUT equ 1

section .data
    msg db "hello world", 10
    msg_len equ $ - msg

section .text
    global _start

_start:
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg
    mov edx, msg_len
    int 0x80

    mov eax, SYS_EXIT
    mov ebx, 0
    int 0x80