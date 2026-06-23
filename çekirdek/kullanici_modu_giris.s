.global kullanici_moduna_atla
kullanici_moduna_atla:
mov 4(%esp), %eax
mov 8(%esp), %ebx
push $0x23
push %ebx
pushf
pop %ecx
or $0x200, %ecx
push %ecx
push $0x1B
push %eax
iret
