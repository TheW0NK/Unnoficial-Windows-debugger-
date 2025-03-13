section .data
    filename db "keyfile.bin", 0     ; Name of the key file
    buffer   times 1024 db 0         ; Buffer to read file contents
    hash     times 32 db 0           ; Buffer to store SHA-256 hash
    fmt      db "%02x", 0            ; Format for printing hex

section .text
    global _start
    extern printf, fopen, fread, fclose, SHA256

_start:
    ; Open the file (fopen)
    mov rdi, filename        ; const char *filename
    mov rsi, "rb"            ; const char *mode (read binary)
    call fopen
    test rax, rax
    jz error                 ; If fopen fails, exit
    mov r12, rax             ; Save file pointer

    ; Read the file into buffer (fread)
    mov rdi, buffer          ; void *ptr
    mov rsi, 1               ; size_t size
    mov rdx, 1024            ; size_t nmemb
    mov rcx, r12             ; FILE *stream
    call fread

    ; Close the file (fclose)
    mov rdi, r12
    call fclose

    ; Hash the buffer (SHA256)
    mov rdi, buffer          ; const unsigned char *d
    mov rsi, rax             ; size_t n (number of bytes read)
    mov rdx, hash            ; unsigned char *md (output buffer)
    call SHA256

    ; Print the hash in hex
    mov rbx, 0               ; Counter
print_loop:
    cmp rbx, 32              ; SHA-256 produces 32 bytes
    je done
    movzx rdi, byte [hash + rbx]
    mov rsi, fmt
    xor rax, rax
    call printf
    inc rbx
    jmp print_loop

done:
    ; Exit the program (Linux syscall)
    mov rax, 60              ; syscall: exit
    xor rdi, rdi             ; status 0
    syscall

error:
    mov rax, 60              ; syscall: exit
    mov rdi, 1               ; status 1 (error)
    syscall
