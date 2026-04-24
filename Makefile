# ⵢ TAMGA:2B9D | Batuhan ALGÜL | Apache-2.0 | Makefile
# BENGÜ = (TAMGA_145 × ONGAN_77) mod 65536

CC      := x86_64-elf-gcc
AS      := nasm
LD      := x86_64-elf-ld

CFLAGS  := -ffreestanding -O2 -Wall -Wextra -std=c11 \
           -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
           -I. -fno-stack-protector
ASFLAGS := -f elf64
LDFLAGS := -T önyükleyici/orhun.ld -nostdlib

ÇIKTI_DİZİN  := çıktı
ÇEKIRDEK_ELF := $(ÇIKTI_DİZİN)/turkos.elf
ÇEKIRDEK_ISO := $(ÇIKTI_DİZİN)/turkos.iso

C_KAYNAKLAR := \
	ortak/dizge.c \
	donanım/kbt/küresel_bölüt.c \
	donanım/kkt/kesme_kapisi.c \
	donanım/zaman/zamanlayıcı.c \
	sürücüler/ekran/görüntü.c \
	sürücüler/klavye/klavye.c \
	bellek/sayfalama.c \
	bellek/yığın_yönetici.c \
	çekirdek/imza.c \
	çekirdek/ana.c

AS_KAYNAKLAR := \
	önyükleyici/başlangıç.s \
	donanım/kkt/kesme_vektörleri.s

C_NESNELER  := $(patsubst %.c, $(ÇIKTI_DİZİN)/%.o, $(C_KAYNAKLAR))
AS_NESNELER := $(patsubst %.s, $(ÇIKTI_DİZİN)/%.o, $(AS_KAYNAKLAR))
TÜM_NESNELER := $(AS_NESNELER) $(C_NESNELER)

.PHONY: hepsi temizle çalıştır iso

hepsi: $(ÇEKIRDEK_ELF)

$(ÇEKIRDEK_ELF): $(TÜM_NESNELER)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^

$(ÇIKTI_DİZİN)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ÇIKTI_DİZİN)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

iso: $(ÇEKIRDEK_ELF)
	@mkdir -p $(ÇIKTI_DİZİN)/iso/boot/grub
	cp $(ÇEKIRDEK_ELF) $(ÇIKTI_DİZİN)/iso/boot/turkos.elf
	@echo 'set timeout=0'                          > \
		$(ÇIKTI_DİZİN)/iso/boot/grub/grub.cfg
	@echo 'set default=0'                         >> \
		$(ÇIKTI_DİZİN)/iso/boot/grub/grub.cfg
	@echo 'menuentry "TurkOS" {'                  >> \
		$(ÇIKTI_DİZİN)/iso/boot/grub/grub.cfg
	@echo '  multiboot2 /boot/turkos.elf'         >> \
		$(ÇIKTI_DİZİN)/iso/boot/grub/grub.cfg
	@echo '}'                                     >> \
		$(ÇIKTI_DİZİN)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ÇEKIRDEK_ISO) \
		$(ÇIKTI_DİZİN)/iso

çalıştır: $(ÇEKIRDEK_ELF)
	qemu-system-x86_64 \
		-kernel $(ÇEKIRDEK_ELF) \
		-serial stdio \
		-m 128M \
		-no-reboot \
		-no-shutdown

çalıştır-iso: $(ÇEKIRDEK_ISO)
	qemu-system-x86_64 \
		-cdrom $(ÇEKIRDEK_ISO) \
		-serial stdio \
		-m 128M \
		-no-reboot \
		-no-shutdown

temizle:
	rm -rf $(ÇIKTI_DİZİN)
