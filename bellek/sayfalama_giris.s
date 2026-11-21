.global sayfalama_etkinlestir
sayfalama_etkinlestir:
mov 8(%esp), %eax
test %eax, %eax
jz 1f
mov $0xC0000080, %ecx
rdmsr
or $0x800, %eax
wrmsr
1:
mov %cr4, %eax
or $0x20, %eax
mov %eax, %cr4
mov 4(%esp), %eax
mov %eax, %cr3
mov %cr0, %eax
or $0x80010000, %eax
mov %eax, %cr0
ret
