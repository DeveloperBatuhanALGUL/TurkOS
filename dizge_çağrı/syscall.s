; ⵢ TAMGA:3C0A | Batuhan ALGÜL | Apache-2.0 | syscall.s
; BENGÜ = (TAMGA_145 × ONGAN_106) mod 65536

bits 64

; dç_çerçeve_t yapısındaki kaydırımlar
DÇ_NUMARA   equ 0
DÇ_ARG0     equ 8
DÇ_ARG1     equ 16
DÇ_ARG2     equ 24
DÇ_ARG3     equ 32
DÇ_ARG4     equ 40
DÇ_BOYUT    equ 48

; x86_64 SYSCALL ABI:
;   rax = sistem çağrı numarası
;   rdi = arg0
;   rsi = arg1
;   rdx = arg2
;   r10 = arg3  (rcx yerine — rcx SYSCALL tarafından bozulur)
;   r8  = arg4
;   rcx = kullanıcı RIP (SYSCALL tarafından kaydedilir)
;   r11 = kullanıcı RFLAGS (SYSCALL tarafından kaydedilir)

section .text
global syscall_giriş_noktası
extern dizge_çağrı_gönder
extern surec_geçerli_al

syscall_giriş_noktası:
    swapgs

    mov     [gs:0x08], rsp
    mov     rsp, [gs:0x00]

    push    rcx
    push    r11
    push    rbp
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15

    sub     rsp, DÇ_BOYUT

    mov     [rsp + DÇ_NUMARA], rax
    mov     [rsp + DÇ_ARG0],   rdi
    mov     [rsp + DÇ_ARG1],   rsi
    mov     [rsp + DÇ_ARG2],   rdx
    mov     [rsp + DÇ_ARG3],   r10
    mov     [rsp + DÇ_ARG4],   r8

    mov     rdi, rsp
    call    dizge_çağrı_gönder

    add     rsp, DÇ_BOYUT

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    pop     r11
    pop     rcx

    mov     rsp, [gs:0x08]
    swapgs

    sysretq

section .data
global syscall_çekirdek_yığın
syscall_çekirdek_yığın:
    dq 0
    dq 0
