.global sayfalama_etkinlestir
sayfalama_etkinlestir:
mov 4(%esp), %eax
mov %eax, %cr3
mov %cr0, %eax
or $0x80000000, %eax
mov %eax, %cr0
ret
