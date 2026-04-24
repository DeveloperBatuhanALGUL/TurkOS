; ⵢ TAMGA:81B5 | Batuhan ALGÜL | Apache-2.0 | kesme_vektörleri.s
; BENGÜ = (TAMGA_145 × ONGAN_229) mod 65536

bits 64

extern kesme_işleyici_genel

%macro HATA_KODU_YOK 1
global kesme_vektör_%1
kesme_vektör_%1:
    push qword 0
    push qword %1
    jmp kesme_ortak_giriş
%endmacro

%macro HATA_KODU_VAR 1
global kesme_vektör_%1
kesme_vektör_%1:
    push qword %1
    jmp kesme_ortak_giriş
%endmacro

HATA_KODU_YOK  0
HATA_KODU_YOK  1
HATA_KODU_YOK  2
HATA_KODU_YOK  3
HATA_KODU_YOK  4
HATA_KODU_YOK  5
HATA_KODU_YOK  6
HATA_KODU_YOK  7
HATA_KODU_VAR  8
HATA_KODU_YOK  9
HATA_KODU_VAR  10
HATA_KODU_VAR  11
HATA_KODU_VAR  12
HATA_KODU_VAR  13
HATA_KODU_VAR  14
HATA_KODU_YOK  15
HATA_KODU_YOK  16
HATA_KODU_VAR  17
HATA_KODU_YOK  18
HATA_KODU_YOK  19
HATA_KODU_YOK  20
HATA_KODU_YOK  21
HATA_KODU_YOK  22
HATA_KODU_YOK  23
HATA_KODU_YOK  24
HATA_KODU_YOK  25
HATA_KODU_YOK  26
HATA_KODU_YOK  27
HATA_KODU_YOK  28
HATA_KODU_YOK  29
HATA_KODU_VAR  30
HATA_KODU_YOK  31

%assign i 32
%rep 224
HATA_KODU_YOK i
%assign i i+1
%endrep

section .text
kesme_ortak_giriş:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    add rdi, 15*8
    mov rsi, [rsp + 15*8]
    call kesme_işleyici_genel

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16
    iretq

section .data
global kesme_vektör_tablosu
kesme_vektör_tablosu:
%assign i 0
%rep 256
    dq kesme_vektör_ %+ i
%assign i i+1
%endrep
