.set MAGIC, 0x1BADB002
.set BAYRAK, 0x0
.set SAGLAMA, -(MAGIC + BAYRAK)

.section .multiboot
.align 4
.long MAGIC
.long BAYRAK
.long SAGLAMA

.section .bss
.align 16
yigin_alt:
.skip 16384
yigin_ust:

.section .text
.global _baslangic
.extern cekirdek_ana
_baslangic:
mov $yigin_ust, %esp
call cekirdek_ana
cli
1: hlt
jmp 1b
