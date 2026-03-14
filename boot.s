; ==========================================
; TurkOS Önyükleme Yazılımı (Bootloader Stub)
; Telif Hakkı (c) 2026 TurkOS Geliştirme Ekibi [cite: 43]
; Lisans: Apache License 2.0
; ==========================================

SIHIRLI_SAYI    equ 0x1BADB002
HIZALAMA        equ 1 << 0
BELLEK_BILGISI  equ 1 << 1
BAYRAKLAR       equ HIZALAMA | BELLEK_BILGISI
DENETIM_TOPLAMI equ -(SIHIRLI_SAYI + BAYRAKLAR)

section .multiboot
align 4
    dd SIHIRLI_SAYI
    dd BAYRAKLAR
    dd DENETIM_TOPLAMI

section .bss
align 16
yigin_alt:
    resb 16384 ; 16 KB çekirdek yığını
yigin_ust:

section .text
global _start
extern kernel_main

_start:
    ; İşlemciyi temiz ve öngörülebilir bir duruma getir
    cli                         ; Kesmeleri devre dışı bırak
    cld                         ; Dizi işlemlerini ileri yönlü ayarla

    ; Yığın (Stack) kurulumu
    mov esp, yigin_ust
    mov ebp, esp                ; Hata ayıklama (stack tracing) için taban işaretçisi

    ; Önyükleyiciden gelen verileri C dünyasına taşı
    ; ebx: Multiboot bilgi yapısının adresi
    ; eax: Multiboot sihirli sayısı (doğrulama için)
    push ebx                    
    push eax                    

    ; TurkOS Çekirdeği başlatılıyor
    call kernel_main

    ; Çekirdek beklenmedik şekilde sonlanırsa işlemciyi güvenli moda al
.islemci_durdur:
    cli                         ; Kesmelerin kapalı olduğundan emin ol
    hlt                         ; İşlemciyi uyku moduna al
    jmp .islemci_durdur        ; Halt komutu geçilirse sonsuz döngüye gir
