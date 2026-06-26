#!/usr/bin/env bash
set -e

export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export LC_CTYPE=en_US.UTF-8
export PYTHONIOENCODING=utf-8

echo "=== Batuss Build Sistemi - TurkOS Derleyici ==="
echo "UTF-8 ortam degiskenleri ayarlandi (LANG=$LANG)"

command -v i686-elf-gcc >/dev/null 2>&1 || { echo "HATA: i686-elf-gcc bulunamadi. Once cross-compiler toolchain'i kurun."; exit 1; }
command -v i686-elf-as  >/dev/null 2>&1 || { echo "HATA: i686-elf-as bulunamadi."; exit 1; }
command -v i686-elf-ld  >/dev/null 2>&1 || { echo "HATA: i686-elf-ld bulunamadi."; exit 1; }

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
  echo "--- QEMU baslatiliyor ---"
  qemu-system-i386 -kernel derleme/turkos.elf -display cocoa -d guest_errors -no-reboot -no-shutdown
fi
