section .data
    keyfile     db "keyfile.bin", 0
    mainfile    db "main", 0
    encrypted_hash db 0x12, 0x34, 0x56, 0x78, ... ; Replace with your actual encrypted bytes
    decrypted_hash times 32 db 0
    buffer      times 1024 db 0
    calc_hash   times 32 db 0
    xor_key     db 0xAA
    message     db "----- Unnoficial Windows Debugger Bootloader -----", 0

section .text
    global _start
    extern fopen, fread, fclose, SHA256, printf, execve

_start:
    ; Open the key file
    mov rdi, keyfile
    mov rsi, "rb"
    call fopen
    test rax, rax
    jz error
    mov r12, rax

    ; Read key file into buffer
    mov rdi, buffer
    mov rsi, 1
    mov rdx, 1024
    mov rcx, r12
    call fread

    ; Close the file
    mov rdi, r12
    call fclose

    ; Hash the buffer
    mov rdi, buffer
    mov rsi, rax         ; Size of data read
    mov rdx, calc_hash
    call SHA256

    ; Decrypt the stored hash
    mov rcx, 0
decrypt_loop:
    cmp rcx, 32
    je compare_hash
    mov al, [encrypted_hash + rcx]
    xor al, [xor_key]
    mov [decrypted_hash + rcx], al
    inc rcx
    jmp decrypt_loop

compare_hash:
    mov rcx, 0
check_loop:
    cmp rcx, 32
    je load_main
    mov al, [decrypted_hash + rcx]
    cmp al, [calc_hash + rcx]
    jne error
    inc rcx
    jmp check_loop

load_main:
    ; Execute the main program
    lea rdi, [mainfile]
    xor rsi, rsi
    xor rdx, rdx
    call execve

error:
    mov rax, 60
    mov rdi, 1
    syscall
