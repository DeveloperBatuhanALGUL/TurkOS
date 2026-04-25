; ⵢ TAMGA:18EC | Batuhan ALGÜL | Apache-2.0 | bağlam.s
; BENGÜ = (TAMGA_145 × ONGAN_44) mod 65536

bits 64

; yazmaç_durumu_t yapısındaki kaydırımlar (süreç.h ile eşleşmeli)
RAX_KAYDIRIM    equ 0
RBX_KAYDIRIM    equ 8
RCX_KAYDIRIM    equ 16
RDX_KAYDIRIM    equ 24
RSI_KAYDIRIM    equ 32
RDI_KAYDIRIM    equ 40
RBP_KAYDIRIM    equ 48
RSP_KAYDIRIM    equ 56
R8_KAYDIRIM     equ 64
R9_KAYDIRIM     equ 72
R10_KAYDIRIM    equ 80
R11_KAYDIRIM    equ 88
R12_KAYDIRIM    equ 96
R13_KAYDIRIM    equ 104
R14_KAYDIRIM    equ 112
R15_KAYDIRIM    equ 120
RIP_KAYDIRIM    equ 128
RFLAGS_KAYDIRIM equ 136
CS_KAYDIRIM     equ 144
SS_KAYDIRIM     equ 152

section .text
global bağlam_değiştir

; void bağlam_değiştir(yazmaç_durumu_t *eski, const yazmaç_durumu_t *yeni)
; rdi = eski yazmaç_durumu_t* (kaydedilecek, BOŞ olabilir)
; rsi = yeni yazmaç_durumu_t* (yüklenecek)
bağlam_değiştir:
    test    rdi, rdi
    jz      .yeni_yükle

    mov     [rdi + RAX_KAYDIRIM],    rax
    mov     [rdi + RBX_KAYDIRIM],    rbx
    mov     [rdi + RCX_KAYDIRIM],    rcx
    mov     [rdi + RDX_KAYDIRIM],    rdx
    mov     [rdi + RSI_KAYDIRIM],    rsi
    mov     [rdi + RDI_KAYDIRIM],    rdi
    mov     [rdi + RBP_KAYDIRIM],    rbp
    mov     [rdi + R8_KAYDIRIM],     r8
    mov     [rdi + R9_KAYDIRIM],     r9
    mov     [rdi + R10_KAYDIRIM],    r10
    mov     [rdi + R11_KAYDIRIM],    r11
    mov     [rdi + R12_KAYDIRIM],    r12
    mov     [rdi + R13_KAYDIRIM],    r13
    mov     [rdi + R14_KAYDIRIM],    r14
    mov     [rdi + R15_KAYDIRIM],    r15

    lea     rax, [rel .geri_dön]
    mov     [rdi + RIP_KAYDIRIM],    rax

    mov     rax, rsp
    mov     [rdi + RSP_KAYDIRIM],    rax

    pushfq
    pop     rax
    mov     [rdi + RFLAGS_KAYDIRIM], rax

    mov     ax, cs
    mov     [rdi + CS_KAYDIRIM],     ax
    mov     ax, ss
    mov     [rdi + SS_KAYDIRIM],     ax

.yeni_yükle:
    mov     rax, [rsi + RFLAGS_KAYDIRIM]
    push    rax
    popfq

    mov     rsp, [rsi + RSP_KAYDIRIM]
    mov     rbx, [rsi + RBX_KAYDIRIM]
    mov     rcx, [rsi + RCX_KAYDIRIM]
    mov     rdx, [rsi + RDX_KAYDIRIM]
    mov     rbp, [rsi + RBP_KAYDIRIM]
    mov     r8,  [rsi + R8_KAYDIRIM]
    mov     r9,  [rsi + R9_KAYDIRIM]
    mov     r10, [rsi + R10_KAYDIRIM]
    mov     r11, [rsi + R11_KAYDIRIM]
    mov     r12, [rsi + R12_KAYDIRIM]
    mov     r13, [rsi + R13_KAYDIRIM]
    mov     r14, [rsi + R14_KAYDIRIM]
    mov     r15, [rsi + R15_KAYDIRIM]
    mov     rdi, [rsi + RDI_KAYDIRIM]
    mov     rax, [rsi + RIP_KAYDIRIM]

    mov     rsi, [rsi + RSI_KAYDIRIM]

    jmp     rax

.geri_dön:
    ret
