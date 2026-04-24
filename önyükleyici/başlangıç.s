; ⵢ TAMGA:2E72 | Batuhan ALGÜL | Apache-2.0 | başlangıç.s
; BENGÜ = (TAMGA_145 × ONGAN_82) mod 65536

MULTIBOOT2_SİHİR        equ 0xE85250D6
MULTIBOOT2_MİMARİ       equ 0
MULTIBOOT2_BAŞLIK_BOY   equ (multiboot2_son - multiboot2_başlık)
MULTIBOOT2_SAĞLAMA      equ (0x100000000 - (MULTIBOOT2_SİHİR + MULTIBOOT2_MİMARİ + MULTIBOOT2_BAŞLIK_BOY))

ÖBEK_BOYUTU             equ 0x4000

section .multiboot2_başlık
align 8
multiboot2_başlık:
    dd MULTIBOOT2_SİHİR
    dd MULTIBOOT2_MİMARİ
    dd MULTIBOOT2_BAŞLIK_BOY
    dd MULTIBOOT2_SAĞLAMA

    align 8
    dw 0
    dw 0
    dd 8
multiboot2_son:

section .bss
align 16
öbek_alt:
    resb ÖBEK_BOYUTU
öbek_üst:

section .text
bits 32
global giriş_noktası
extern çekirdek_ana

giriş_noktası:
    mov esp, öbek_üst
    push 0
    popf

    push ebx
    push eax

    call çekirdek_ana_köprü

    cli
.dur:
    hlt
    jmp .dur

çekirdek_ana_köprü:
    pop eax
    pop ebx

    lgdt [geçici_kbt_gösterge]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:uzun_kipe_geç

bits 64
uzun_kipe_geç:
    mov rdi, rbx
    call çekirdek_ana
    cli
.dur:
    hlt
    jmp .dur

section .data
align 8
geçici_kbt:
    dq 0x0000000000000000
    dq 0x00AF9A000000FFFF
    dq 0x00CF92000000FFFF

geçici_kbt_gösterge:
    dw (3 * 8) - 1
    dd geçici_kbt
