#!/usr/bin/env bash
set -e

export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export LC_CTYPE=en_US.UTF-8
export PYTHONIOENCODING=utf-8

echo "=== Batuss Build Sistemi - TurkOS Derleyici ==="
echo "UTF-8 ortam degiskenleri ayarlandi (LANG=$LANG)"

kur_toolchain() {
    echo "i686-elf cross-compiler toolchain bulunamadi, kurulum deneniyor..."
    if [[ "$OSTYPE" == "darwin"* ]]; then
        if command -v brew >/dev/null 2>&1; then
            brew tap nativeos/i686-elf-toolchain 2>/dev/null || true
            brew install i686-elf-binutils i686-elf-gcc || {
                echo "HATA: Homebrew ile otomatik kurulum basarisiz."
                echo "Manuel kurulum icin: https://wiki.osdev.org/GCC_Cross-Compiler"
                exit 1
            }
        else
            echo "HATA: Homebrew bulunamadi. Once https://brew.sh adresinden kurun."
            exit 1
        fi
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if command -v apt-get >/dev/null 2>&1; then
            sudo apt-get update && sudo apt-get install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo qemu-system-x86
            echo "UYARI: apt repolarinda hazir i686-elf-gcc paketi olmayabilir."
            echo "Bu durumda OSDev wiki'sindeki kaynaktan derleme talimatlarini izleyin:"
            echo "https://wiki.osdev.org/GCC_Cross-Compiler"
            exit 1
        elif command -v pacman >/dev/null 2>&1; then
            sudo pacman -S --needed base-devel qemu-system-x86 || true
            echo "UYARI: i686-elf-gcc icin AUR'dan (i686-elf-gcc paketi) kurulum gerekebilir."
            exit 1
        else
            echo "HATA: Desteklenmeyen paket yoneticisi. Manuel kurulum gerekli:"
            echo "https://wiki.osdev.org/GCC_Cross-Compiler"
            exit 1
        fi
    else
        echo "HATA: Taninmayan isletim sistemi ($OSTYPE)."
        echo "Windows kullaniyorsaniz, batuss-derle.bat dosyasini calistirip WSL2 uzerinden devam edin."
        exit 1
    fi
}

if ! command -v i686-elf-gcc >/dev/null 2>&1 || ! command -v i686-elf-as >/dev/null 2>&1 || ! command -v i686-elf-ld >/dev/null 2>&1; then
    kur_toolchain
fi

command -v i686-elf-gcc >/dev/null 2>&1 || { echo "HATA: i686-elf-gcc kurulumdan sonra da bulunamadi."; exit 1; }
command -v i686-elf-as  >/dev/null 2>&1 || { echo "HATA: i686-elf-as kurulumdan sonra da bulunamadi."; exit 1; }
command -v i686-elf-ld  >/dev/null 2>&1 || { echo "HATA: i686-elf-ld kurulumdan sonra da bulunamadi."; exit 1; }

mkdir -p derleme

AS="i686-elf-as"
CC="i686-elf-gcc -ffreestanding -fno-stack-protector -fno-pic -nostdlib -Wall -Wextra -std=gnu99"

echo "--- Assembly dosyalari derleniyor ---"
$AS önyükleyici/başlangıç.s -o derleme/baslangic.o
$AS çekirdek/gdt_giris.s -o derleme/gdt_giris.o
$AS donanım/kkt/kesme_vektörleri.s -o derleme/kesme_vektorleri.o
$AS bellek/sayfalama_giris.s -o derleme/sayfalama_giris.o
$AS çekirdek/kullanici_modu_giris.s -o derleme/kullanici_modu_giris.o
$AS çekirdek/gorev_gecis.s -o derleme/gorev_gecis.o

echo "--- C dosyalari derleniyor ---"
$CC -c çekirdek/ana.c -o derleme/ana.o
$CC -c çekirdek/gdt.c -o derleme/gdt.o
$CC -c çekirdek/panik.c -o derleme/panik.o
$CC -c çekirdek/istisnalar.c -o derleme/istisnalar.o
$CC -c çekirdek/kullanici_modu.c -o derleme/kullanici_modu.o
$CC -c çekirdek/sistem_cagrisi.c -o derleme/sistem_cagrisi.o
$CC -c çekirdek/gorev.c -o derleme/gorev.o
$CC -c çekirdek/denetim/denetim_otobusu.c -o derleme/denetim_otobusu.o
$CC -c çekirdek/denetim/gorev_denetleyici.c -o derleme/gorev_denetleyici.o
$CC -c çekirdek/denetim/bellek_denetleyici.c -o derleme/bellek_denetleyici.o
$CC -c çekirdek/denetim/kesme_denetleyici.c -o derleme/kesme_denetleyici.o
$CC -c çekirdek/denetim/devriye.c -o derleme/devriye.o
$CC -c sürücüler/görüntü.c -o derleme/goruntu.o
$CC -c sürücüler/klavye.c -o derleme/klavye.o
$CC -c donanım/kkt/kesme_kapisi.c -o derleme/kesme_kapisi.o
$CC -c donanım/zaman/zamanlayıcı.c -o derleme/zamanlayici.o
$CC -c bellek/sayfalama.c -o derleme/sayfalama.o
$CC -c bellek/yığın_yönetici.c -o derleme/yigin_yonetici.o

echo "--- Link ediliyor ---"
i686-elf-ld -T önyükleyici/orhun.ld -o derleme/turkos.elf \
  derleme/baslangic.o derleme/gdt_giris.o derleme/kesme_vektorleri.o \
  derleme/sayfalama_giris.o derleme/kullanici_modu_giris.o derleme/gorev_gecis.o \
  derleme/ana.o derleme/gdt.o derleme/panik.o derleme/istisnalar.o \
  derleme/kullanici_modu.o derleme/sistem_cagrisi.o derleme/gorev.o \
  derleme/denetim_otobusu.o derleme/gorev_denetleyici.o derleme/bellek_denetleyici.o \
  derleme/kesme_denetleyici.o derleme/devriye.o derleme/goruntu.o derleme/klavye.o \
  derleme/kesme_kapisi.o derleme/zamanlayici.o derleme/sayfalama.o derleme/yigin_yonetici.o

echo "=== Derleme tamamlandi: derleme/turkos.elf ==="
ls -la derleme/turkos.elf

if [ "$1" == "--calistir" ]; then
  command -v qemu-system-i386 >/dev/null 2>&1 || {
      echo "HATA: qemu-system-i386 bulunamadi."
      if [[ "$OSTYPE" == "darwin"* ]]; then
          echo "Kurulum icin: brew install qemu"
      else
          echo "Kurulum icin: sudo apt-get install qemu-system-x86 (veya dagitiminizin paket yoneticisi)"
      fi
      exit 1
  }
  echo "--- QEMU baslatiliyor ---"
  qemu-system-i386 -kernel derleme/turkos.elf -display cocoa -d guest_errors -no-reboot -no-shutdown
fi
