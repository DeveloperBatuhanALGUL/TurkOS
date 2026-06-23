.global gorev_degistir
gorev_degistir:
push %ebx
push %esi
push %edi
push %ebp
mov 20(%esp), %eax
mov %esp, (%eax)
mov 24(%esp), %esp
pop %ebp
pop %edi
pop %esi
pop %ebx
ret
