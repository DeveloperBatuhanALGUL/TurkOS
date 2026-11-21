bits 16
org 0x7C00

%ifndef CEKIRDEK_KESIM
%error "CEKIRDEK_KESIM tanimli degil"
%endif
%ifndef CEKIRDEK_GIRIS
%error "CEKIRDEK_GIRIS tanimli degil"
%endif
%ifndef BSS_SON
%error "BSS_SON tanimli degil"
%endif
%if CEKIRDEK_KESIM > 127
%error "Cekirdek tek okumada yuklenemeyecek kadar buyuk"
%endif

baslangic:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti
    mov [surucu], dl

    mov si, mesaj_basla
    call yaz

    mov ah, 0x41
    mov bx, 0x55AA
    mov dl, [surucu]
    int 0x13
    jc hata_lba
    cmp bx, 0xAA55
    jne hata_lba

    mov si, dap
    mov ah, 0x42
    mov dl, [surucu]
    int 0x13
    jc hata_disk

    in al, 0x92
    or al, 2
    and al, 0xFE
    out 0x92, al

    cli
    lgdt [gdt_tanimi]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:korumali_kip

yaz:
    lodsb
    test al, al
    jz .son
    mov ah, 0x0E
    mov bx, 0x0007
    int 0x10
    jmp yaz
.son:
    ret

hata_lba:
    mov si, mesaj_lba
    jmp hata
hata_disk:
    mov si, mesaj_disk
hata:
    call yaz
.dur:
    cli
    hlt
    jmp .dur

bits 32
korumali_kip:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x7C00
    cld
    mov esi, 0x10000
    mov edi, 0x100000
    mov ecx, CEKIRDEK_KESIM * 128
    rep movsd
    mov ecx, BSS_SON
    sub ecx, edi
    shr ecx, 2
    xor eax, eax
    rep stosd
    mov eax, CEKIRDEK_GIRIS
    jmp eax

align 8
gdt:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_tanimi:
    dw gdt_tanimi - gdt - 1
    dd gdt

dap:
    db 0x10
    db 0
    dw CEKIRDEK_KESIM
    dw 0x0000
    dw 0x1000
    dq 1

surucu: db 0
mesaj_basla: db "TurkOS onyukleyici", 13, 10, 0
mesaj_lba: db "HATA: LBA yok", 0
mesaj_disk: db "HATA: disk okunamadi", 0

times 446 - ($ - $$) db 0
db 0x80, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF
dd 1
dd CEKIRDEK_KESIM
times 48 db 0
dw 0xAA55
