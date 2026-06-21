.global idt_yukle
idt_yukle:
mov 4(%esp), %eax
lidt (%eax)
ret

.global irq1_giris
.extern kesme_gonder
irq1_giris:
pusha
call irq1_cagir
popa
iret

irq1_cagir:
push $33
call kesme_gonder
add $4, %esp
ret

.global irq0_giris
irq0_giris:
pusha
call irq0_cagir
popa
iret

irq0_cagir:
push $32
call kesme_gonder
add $4, %esp
ret
