/* ⵢ TAMGA:338B | Batuhan ALGÜL | Apache-2.0 | orhun.ld */
/* BENGÜ = (TAMGA_145 × ONGAN_91) mod 65536          */

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(giriş_noktası)

TABAN_ADRES = 0x100000;

SECTIONS
{
    . = TABAN_ADRES;

    .multiboot2_başlık :
    {
        KEEP(*(.multiboot2_başlık))
    }

    .text ALIGN(4K) :
    {
        *(.text)
        *(.text.*)
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata)
        *(.rodata.*)
    }

    .data ALIGN(4K) :
    {
        *(.data)
        *(.data.*)
    }

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

    çekirdek_son = .;

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.note.*)
    }
}
