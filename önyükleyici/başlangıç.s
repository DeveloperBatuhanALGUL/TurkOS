; ⵢ TAMGA:2E72 | Batuhan ALGÜL | Apache-2.0 | başlangıç.s
; BENGÜ = (TAMGA_145 × ONGAN_82) mod 65536

global giriş_noktası
extern çekirdek_ana

section .text
align 8
bits 64

çoklu_önyükleme_başlığı:
    dd 0xE85250D6
    dd 0
    dd çoklu_önyükleme_son - çoklu_önyükleme_başlığı
    dd -(0xE85250D6 + 0 + (çoklu_önyükleme_son - çoklu_önyükleme_başlığı))
çoklu_önyükleme_son:
    dw 0
    dw 0
    dd 0

giriş_noktası:
    cli
    mov rsp, öbek_sınırı
    mov rbp, rsp
    call çekirdek_ana
bekleme_döngüsü:
    hlt
    jmp bekleme_döngüsü

section .bss
align 16
öbek_sınırı:
    resb 65536
