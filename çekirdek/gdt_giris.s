.global gdt_yukle
gdt_yukle:
mov 4(%esp), %eax
lgdt (%eax)
mov $0x10, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %fs
mov %ax, %gs
mov %ax, %ss
jmp gdt_temizle
gdt_temizle:
ret

.global tss_yukle
tss_yukle:
movzwl 4(%esp), %eax
ltr %ax
ret
