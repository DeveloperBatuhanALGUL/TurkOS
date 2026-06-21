ENTRY(_baslangic)
SECTIONS
{
. = 1M;
.multiboot : { *(.multiboot) }
.text : { *(.text) }
.bss : { *(.bss) }
}
