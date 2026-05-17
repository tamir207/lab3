global main
extern strlen

section .data
    Infile dd 0
    Outfile dd 1
    Encoder dd 0
    EncoderCurrPointer dd 0

section .rodata
    newline db 10
    error_message db "cannot open file", 10

section .bss
    buffer resb 1

section .text
main:
    push ebp
    mov ebp, esp
    pushad

    mov esi, dword [ebp+12]
    xor edi, edi

print_loop:
    cmp edi, dword [ebp+8]
    jge call_encode

    mov ebx, dword [esi + edi*4]

    mov al, byte [ebx]
    cmp al, '+'
    je plus
    cmp al, '-'
    je minus
    jmp print

plus:
    cmp byte [ebx+1], 'V'
    jne print
    mov eax, ebx
    add eax, 2
    cmp byte [eax], 0
    je print
    mov dword [Encoder], eax
    mov dword [EncoderCurrPointer], eax
    jmp print

minus:
    cmp byte [ebx+1], 'i'
    je handle_infile
    cmp byte [ebx+1], 'o'
    je handle_outfile
    jmp print

handle_infile:
    push ebx
    add ebx, 2
    mov eax, 5
    mov ecx, 0
    mov edx, 0
    int 0x80
    pop ebx

    cmp eax, 0
    jl file_error
    mov dword [Infile], eax
    jmp print

handle_outfile:
    push ebx
    add ebx, 2
    mov eax, 5
    mov ecx, 577
    mov edx, 420
    int 0x80
    pop ebx

    cmp eax, 0
    jl file_error
    mov dword [Outfile], eax
    jmp print

file_error:
    mov eax, 4
    mov ebx, 2
    mov ecx, error_message
    mov edx, 17
    int 0x80

    popad
    mov eax, 1
    mov ebx, 0x55
    int 0x80

print:
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

call_encode:
    call encode

    popad
    mov eax, 1
    mov ebx, 0
    int 0x80

encode:
    push ebp
    mov ebp, esp
    pushad

read_loop:
    mov eax, 3
    mov ebx, dword [Infile]
    mov ecx, buffer
    mov edx, 1
    int 0x80

    cmp eax, 0
    jle encode_done

    mov edi, dword [EncoderCurrPointer]
    cmp edi, 0
    je write_char

    mov al, byte [buffer]
    
    cmp al, 'A'
    jl write_char
    cmp al, 'Z'
    jle upper
    
    cmp al, 'a'
    jl write_char
    cmp al, 'z'
    jg write_char

    mov cl, byte [edi]
    sub cl, 'A'
    add al, cl
    cmp al, 'z'
    jle update_char
    sub al, 26
    jmp update_char

upper:
    mov cl, byte [edi]
    sub cl, 'A'
    add al, cl
    cmp al, 'Z'
    jle update_char
    sub al, 26

update_char:
    mov byte [buffer], al
    inc edi
    cmp byte [edi], 0
    jne update_pointer
    mov edi, dword [Encoder]

update_pointer:
    mov dword [EncoderCurrPointer], edi

write_char:
    mov eax, 4
    mov ebx, dword [Outfile]
    mov ecx, buffer
    mov edx, 1
    int 0x80

    jmp read_loop

encode_done:
    popad
    mov esp, ebp
    pop ebp
    ret