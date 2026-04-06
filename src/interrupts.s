; ========================================================================
; TurkOS - KESME VEKTÖRLERİ (INTERRUPT VECTORS)
; Tanım: Her bir kesme numarası için giriş noktası sağlar.
; İmza: Batuhan ALGÜL [Hash: 0x7F3A9B2C]
; Yazar: Batuhan ALGÜL & TurkOS Geliştirme Ekibi
; Tarih: 2026
; ========================================================================

.global isr0
.global isr1
.global isr32
.global isr33
.global islemci_kesme_yonetici

.extern c_islemci_kesme_isleyici

; Ortak Kesme İşleyici Makrosu
%macro KESME_GIRISI 1
    pusha                   ; Tüm genel amaçlı register'ları yığına at
    mov ax, ds              ; Data segment'i kaydet
    push eax
    
    mov ax, 0x10            ; Kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push %1                 ; Kesme numarasını parametre olarak geç
    call c_islemci_kesme_isleyici
    add esp, 4              ; Parametreyi yığından temizle
    
    pop eax                 ; Data segment'i geri yükle
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa                    ; Register'ları geri yükle
    iret                    ; Kesmeden dön
%endmacro

; Kesme Vektörleri
isr0:
    KESME_GIRISI 0

isr1:
    KESME_GIRISI 1

isr32: ; Timer
    KESME_GIRISI 32

isr33: ; Keyboard
    KESME_GIRISI 33

; C Tarafından Çağrılan Genel İşleyici Giriş Noktası
islemci_kesme_yonetici:
    ret
