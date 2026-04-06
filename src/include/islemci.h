/* ========================================================================
   TurkOS - İŞLEMCİ VE KESME BAŞLIKLARI
   Tanım: C ve Assembly arasında kesme yönetimi için gerekli tanımlamalar.
   Yazar: Batuhan ALGÜL & TurkOS Geliştirme Ekibi
   Tarih: 2026
   ======================================================================== */

#ifndef ISLEMCI_H
#define ISLEMCI_H

#include <stdint.h>

/* Kesme İşleyici Fonksiyon Prototipleri (Assembly'de tanımlanacak) */
extern void isr0();   // Division by Zero
extern void isr1();   // Debug
extern void isr32();  // Timer Interrupt (IRQ 0)
extern void isr33();  // Keyboard Interrupt (IRQ 1)

/* Kesme Kapısı Kurulum Fonksiyonu */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

/* Kesme Yöneticisi Başlatma */
void islemci_kesme_baslat();

/* Sistem Zamanı ve Tick Sayacı */
extern volatile uint32_t toplam_sistem_tik_sayisi;
uint32_t sistem_zamani_al();

#endif
