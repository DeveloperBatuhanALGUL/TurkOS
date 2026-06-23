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

.extern istisna_isleyici
istisna_ortak:
pusha
mov 32(%esp), %eax
mov 36(%esp), %ebx
push %ebx
push %eax
call istisna_isleyici
add $8, %esp
popa
add $8, %esp
iret

.macro ISTISNA_HATASIZ numara
.global isr\numara\()_giris
isr\numara\()_giris:
push $0
push $\numara
jmp istisna_ortak
.endm

.macro ISTISNA_HATALI numara
.global isr\numara\()_giris
isr\numara\()_giris:
push $\numara
jmp istisna_ortak
.endm

ISTISNA_HATASIZ 0
ISTISNA_HATASIZ 1
ISTISNA_HATASIZ 2
ISTISNA_HATASIZ 3
ISTISNA_HATASIZ 4
ISTISNA_HATASIZ 5
ISTISNA_HATASIZ 6
ISTISNA_HATASIZ 7
ISTISNA_HATALI 8
ISTISNA_HATASIZ 9
ISTISNA_HATALI 10
ISTISNA_HATALI 11
ISTISNA_HATALI 12
ISTISNA_HATALI 13
ISTISNA_HATALI 14
ISTISNA_HATASIZ 15
ISTISNA_HATASIZ 16
ISTISNA_HATALI 17
ISTISNA_HATASIZ 18
ISTISNA_HATASIZ 19
ISTISNA_HATASIZ 20
ISTISNA_HATASIZ 21
ISTISNA_HATASIZ 22
ISTISNA_HATASIZ 23
ISTISNA_HATASIZ 24
ISTISNA_HATASIZ 25
ISTISNA_HATASIZ 26
ISTISNA_HATASIZ 27
ISTISNA_HATASIZ 28
ISTISNA_HATASIZ 29
ISTISNA_HATASIZ 30
ISTISNA_HATASIZ 31
