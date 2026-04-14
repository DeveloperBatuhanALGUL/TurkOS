; ==========================================
; TurkOS Kesme Servis Rutinleri (ISR) v0.1.0
; Telif Hakkı (c) 2026 TurkOS Geliştirme Ekibi
; Tanım: Donanım kesmelerini C tarafına bağlayan köprü.
; ==========================================

[BITS 32]

; idt.c'den çağrılan fonksiyon
global idt_load
extern idtp ; idt.c'deki IDT işaretçisi

idt_load:
    mov eax, [esp + 4]  ; Parametre olarak gelen idtp adresini al
    lidt [eax]          ; IDT'yi işlemciye yükle
    ret

; C tarafındaki işleyicileri buraya bağlıyoruz
extern timer_interrupt_handler
extern keyboard_handler ; keyboard.c'deki ana fonksiyonun ismiyle uyumlu olmalı

; --- KESME GİRİŞ NOKTALARI ---

global irq0
global irq1

; Zamanlayıcı (Timer) - IRQ 0
irq0:
    pushad              ; Tüm genel amaçlı yazmaçları sakla (EAX, EBX vb.)
    call timer_interrupt_handler
    
    ; PIC'e işlemin bittiğini bildir (EOI - End of Interrupt)
    mov al, 0x20
    out 0x20, al
    
    popad               ; Yazmaçları geri yükle
    iretd               ; Kesmeden dön (Interrupt Return)

; Klavye (Keyboard) - IRQ 1
irq1:
    pushad
    call keyboard_handler
    
    mov al, 0x20
    out 0x20, al
    
    popad
    iretd

; Batuhan ALGUL | "Donanım dile geldiğinde onu duymalıyız."
