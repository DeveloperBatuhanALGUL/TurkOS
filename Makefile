# ========================================================================
# TurkOS - MODERN DERLEME SİSTEMİ (MAKEFILE)
# Tanım: GCC ve NASM kullanılarak çoklu kaynak dosyalarını derler.
# Yazar: Batuhan ALGÜL & TurkOS Geliştirme Ekibi
# Tarih: 2026
# ========================================================================

# --- ARAÇLAR ---
CC = gcc
AS = nasm
LD = ld
QEMU = qemu-system-x86_64
RM = rm -rf

# --- BAYRAKLAR ---
CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=c99 -Isrc/include
ASFLAGS = -felf32
LDFLAGS = -T linker.ld -nostdlib -n

# --- HEDEFLER ---
KERNEL_BIN = turkos.bin
BUILD_DIR = build
SRC_DIR = src

# --- KAYNAK DOSYALAR ---
# Assembly
ASM_SRCS = $(wildcard $(SRC_DIR)/*.s)
ASM_OBJS = $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SRCS))

# C
C_SRCS = $(wildcard $(SRC_DIR)/*.c)
C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))

OBJS = $(ASM_OBJS) $(C_OBJS)

# --- RENKLİ ÇIKTI İÇİN TANIMLAR ---
BLUE = \033[1;34m
GREEN = \033[1;32m
YELLOW = \033[1;33m
NC = \033[0m # No Color

# --- HEDEFLER ---
.PHONY: all clean run debug iso

all: $(KERNEL_BIN)
	@echo "$(GREEN)✅ TurkOS Derlemesi Başarıyla Tamamlandı!$(NC)"

# Bağlama İşlemi
$(KERNEL_BIN): $(OBJS)
	@echo "$(BLUE)🔗 Bağlanıyor: $(KERNEL_BIN)$(NC)"
	$(LD) $(LDFLAGS) -o $@ $^

# Assembly Derleme
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(BUILD_DIR)
	@echo "$(YELLOW)🔨 Derleniyor (ASM): $<$(NC)"
	$(AS) $(ASFLAGS) -o $@ $<

# C Derleme
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "$(YELLOW)🔨 Derleniyor (C): $<$(NC)"
	$(CC) $(CFLAGS) -c -o $@ $<

# Temizlik
clean:
	@echo "$(YELLOW)🧹 Derleme artıkları temizleniyor...$(NC)"
	$(RM) $(BUILD_DIR) $(KERNEL_BIN)
	@echo "$(GREEN)✅ Temizlik Tamamlandı.$(NC)"

# Çalıştırma (QEMU)
run: $(KERNEL_BIN)
	@echo "$(BLUE)🚀 TurkOS Başlatılıyor (QEMU)...$(NC)"
	$(QEMU) -kernel $(KERNEL_BIN) -m 256M -no-reboot

# Debug Modu (GDB Bekler)
debug: $(KERNEL_BIN)
	@echo "$(BLUE)🐞 Debug Modu Başlatılıyor...$(NC)"
	$(QEMU) -s -S -kernel $(KERNEL_BIN) -m 256M

# ISO Oluşturma (Gelecek Adımlar İçin Hazır)
iso: $(KERNEL_BIN)
	@echo "ISO oluşturma aşaması henüz aktif değil."
