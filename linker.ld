/* ========================================================================
   TurkOS - BAĞLAYICI BETİĞİ (LINKER SCRIPT)
   Tanım: Çekirdek bileşenlerinin bellekteki yerleşim düzenini belirler.
   ======================================================================== */

ENTRY(_start)

SECTIONS
{
    /* Multiboot gereği 1MB'den başla */
    . = 1M;

    .text : ALIGN(4K)
    {
        *(.multiboot)      /* Önyükleme bilgisi en başta */
        *(.text*)          /* Kod bölümü */
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata*)        /* Salt okunur veriler (String literals vb.) */
    }

    .data : ALIGN(4K)
    {
        *(.data*)          /* Başlatılmış veriler */
    }

    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss*)           /* Başlatılmamış veriler */
    }
}
