/* ========================================================================
   TurkOS - BAĞLAYICI BETİĞİ (LINKER SCRIPT)
   Tanım: Çekirdek bileşenlerinin bellekteki yerleşim düzenini belirler.
   Yazar: Batuhan ALGÜL & TurkOS Geliştirme Ekibi
   Tarih: 2026
   ======================================================================== */

ENTRY(_start) /* Giriş noktası: boot.s içindeki _start etiketi */

SECTIONS
{
    /* Çekirdeği 1MB adresinden başlat (Multiboot gereği) */
    . = 1M;

    /* Metin bölümü (Kod) */
    .text : ALIGN(4K)
    {
        *(.multiboot)      /* Multiboot başlığı en başta olmalı */
        *(.text*)          /* Tüm kod bölümleri */
    }

    /* Salt okunur veri */
    .rodata : ALIGN(4K)
    {
        *(.rodata*)        /* Sabit veriler */
    }

    /* Okunur-yazılır veri */
    .data : ALIGN(4K)
    {
        *(.data*)          /* Başlatılmış global değişkenler */
    }

    /* Sıfırlanmış veri (BSS) */
    .bss : ALIGN(4K)
    {
        *(COMMON)          /* Ortak semboller */
        *(.bss*)           /* Başlatılmamış global değişkenler */
    }
}
