ENTRY(_baslangic)

PHDRS
{
    metin PT_LOAD FLAGS(5);
    salt_okunur PT_LOAD FLAGS(4);
    kullanici_metin PT_LOAD FLAGS(5);
    kullanici_veri PT_LOAD FLAGS(6);
    veri PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 1M;

    .text ALIGN(4K) :
    {
        cekirdek_metin_baslangici = .;
        KEEP(*(.multiboot))
        *(.text .text.*)
        . = ALIGN(4K);
        cekirdek_metin_sonu = .;
    } :metin

    .rodata ALIGN(4K) :
    {
        cekirdek_salt_okunur_baslangici = .;
        *(.rodata .rodata.*)
        . = ALIGN(4K);
        cekirdek_salt_okunur_sonu = .;
    } :salt_okunur

    .kullanici_metin ALIGN(4K) :
    {
        kullanici_metin_baslangici = .;
        KEEP(*(.kullanici_metin))
        . = ALIGN(4K);
        kullanici_metin_sonu = .;
    } :kullanici_metin

    .kullanici_veri ALIGN(4K) :
    {
        kullanici_veri_baslangici = .;
        KEEP(*(.kullanici_veri))
        . = ALIGN(4K);
        kullanici_veri_sonu = .;
    } :kullanici_veri

    .data ALIGN(4K) :
    {
        cekirdek_veri_baslangici = .;
        *(.data .data.*)
    } :veri

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss .bss.*)
        . = ALIGN(4K);
        cekirdek_veri_sonu = .;
    } :veri

    /DISCARD/ : { *(.comment) *(.eh_frame) *(.note*) *(.got*) }
}
