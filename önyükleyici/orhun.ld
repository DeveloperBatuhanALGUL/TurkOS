ENTRY(_baslangic)

PHDRS
{
    metin PT_LOAD FLAGS(5);
    salt_okunur PT_LOAD FLAGS(4);
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
