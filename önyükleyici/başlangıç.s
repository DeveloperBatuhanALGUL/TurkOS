.set MAGIC, 0x1BADB002
.set BAYRAK, 0x0
.set SAGLAMA, -(MAGIC + BAYRAK)

.section .multiboot, "a"
.align 4
.long MAGIC
.long BAYRAK
.long SAGLAMA

.section .bss
.align 4
.global __stack_chk_guard
__stack_chk_guard:
.skip 4
.align 4096
.global yigin_koruma
yigin_koruma:
.skip 4096
yigin_alt:
.skip 16384
yigin_ust:

.section .text
.global _baslangic
.extern cekirdek_ana
_baslangic:
mov $yigin_ust, %esp
mov $1, %eax
cpuid
bt $30, %ecx
jnc 2f
mov $10, %ecx
1:
rdrand %eax
jc 3f
loop 1b
2:
rdtsc
xor %edx, %eax
rol $13, %eax
3:
and $0xFFFFFF00, %eax
test %eax, %eax
jnz 4f
mov $0x5A5A5A00, %eax
4:
mov %eax, __stack_chk_guard
call cekirdek_ana
cli
5: hlt
jmp 5b
