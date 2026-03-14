/*
 * TurkOS Kernel - Programmable Interval Timer (PIT) Driver
 * Telif Hakkı (c) 2026 TurkOS Geliştirme Ekibi
 * Lisans: Apache License 2.0
 */

#include <stdint.h>


#define PZAÖ_VERI_KAPISI_0    0x40  // Kanal 0 veri girişi
#define PZAÖ_KOMUT_KAPISI     0x43  // Komut yazmacı
#define ANA_SAAT_HIZI         1193182 // PIT kristal frekansı (Hz)


volatile uint32_t toplam_sistem_tik_sayisi = 0;
uint32_t sistem_hizi_hz = 0;


extern void outb(uint16_t kapi, uint8_t deger);


void timer_interrupt_handler() {
   
    toplam_sistem_tik_sayisi++;

  
}


void timer_init(uint32_t frequency) {
    sistem_hizi_hz = frequency;

    
    uint32_t bolen_orani = ANA_SAAT_HIZI / frequency;

  
    uint8_t komut_bayti = 0x36;
    outb(PZAÖ_KOMUT_KAPISI, komut_bayti);

    uint8_t dusuk_bayt = (uint8_t)(bolen_orani & 0xFF);
    uint8_t yuksek_bayt = (uint8_t)((bolen_orani >> 8) & 0xFF);

    outb(PZAÖ_VERI_KAPISI_0, dusuk_bayt);
    outb(PZAÖ_VERI_KAPISI_0, yuksek_bayt);
}


void timer_wait(uint32_t beklenecek_tik) {
    uint32_t baslangic_zamani = toplam_sistem_tik_sayisi;
    
    
    while(toplam_sistem_tik_sayisi < (baslangic_zamani + beklenecek_tik)) {
     
        __asm__ volatile("pause");
    }
}
