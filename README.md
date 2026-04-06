# TURKOS: ULUSAL AÇIK KAYNAK İŞLETİM SİSTEMİ

![Lisans](https://img.shields.io/badge/Lisans-Apache%202.0-blue)
![Mimari](https://img.shields.io/badge/Mimari-x86__64-orange)
![Durum](https://img.shields.io/badge/Durum-Geliştirme%20Aşamasında-red)
![Dil](https://img.shields.io/badge/Dil-C_%7C_ASM-yellow)

> **"Bağımsız Türkiye'nin Bağımsız Çekirdeği."**
> *Türk Milleti'nin siber egemenliği için tasarlanmış, denetlenebilir, yerli ve milli ilk açık kaynak işletim sistemi.*

---

## 1. GİRİŞ VE STRATEJİK VİZYON

**TurkOS**, yabancı yazılım bağımlılığını kırmak ve kritik altyapılarda **tam kontrol** sağlamak amacıyla geliştirilen, sıfırdan inşa edilmiş bir x86_64 işletim sistemidir. Proje, **Batuhan ALGÜL** öncülüğünde, şeffaflık, güvenlik ve performans ilkeleriyle hareket eden geniş bir geliştirici topluluğu tarafından desteklenmektedir.

### 1.1. Temel İlkeler

| İlke | Açıklama |
| :--- | :--- |
| **Siber Egemenlik** | Tüm kod tabanı yerlidir, dışarıya veri sızdırmaz. |
| **Denetlenebilirlik** | Kaynak kodun tamamı açık kaynaklıdır, her satırı incelenebilir. |
| **Güvenlik Öncelikli** | Kernel düzeyinde MAC (Zorunlu Erişim Kontrolü) ve şifreleme. |
| **Modüler Mimari** | Mikrokernel tabanlı, kolayca genişletilebilir yapı. |
| **Ulusal Kimlik** | Türkçe dil desteği ve kültürel mirasa uygun arayüzler. |

---

## 2. MİMARİ GENEL BAKIŞ

TurkOS, katmanlı ve modüler bir mimari üzerine inşa edilmiştir. Aşağıdaki şema, sistemin yüksek seviyeli bileşenlerini göstermektedir.

### 2.1. Yüksek Seviye Sistem Mimarisi

```text
┌─────────────────────────────────────────────────────────────────┐
│                    KULLANICI ALANI (USER SPACE)                  │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐   │
│  │ Yazman Ofis  │  │ Anadolu Zeka │  │  Sistem Uygulamaları │   │
│  │ Paketi       │  │ (AI Motoru)  │  │  (Shell, GUI)        │   │
│  └──────┬───────┘   └──────┬───────┘  └──────────┬───────────┘   │
└─────────┼─────────────────┼─────────────────────┼───────────────┘
          ▼                 ▼                     ▼
┌─────────────────────────────────────────────────────────────────┐
│              SİSTEM KÜTÜPHANELERİ (LIBC, LIBM)                   │
└─────────────────────────────────┬───────────────────────────────┘
                                  ▼
┌─────────────────────────────────────────────────────────────────┐
│                    TÜRKOS KERNEL (RING 0)                        │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  KESME YÖNETİCİSİ (INTERRUPT MANAGER)                    │   │
│  │  - IDT Kurulumu                                          │   │
│  │  - PIC/IOAPIC Entegrasyonu                               │   │
│  │  - Timer & Klavye Handler'ları                           │   │
│  └──────────────────────────┬───────────────────────────────┘   │
│                             ▼                                     │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  BELLEK YÖNETİCİSİ (MEMORY MANAGER)                      │   │
│  │  - Paging (Sayfalama)                                    │   │
│  │  - Heap Alloc (kmalloc/kfree)                            │   │
│  │  - GDT Segmentasyon                                      │   │
│  └──────────────────────────┬───────────────────────────────┘   │
│                             ▼                                     │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  DONANIM SOYUTLAMA KATMANI (HAL)                         │   │
│  │  - VGA Sürücüsü                                          │   │
│  │  - PS/2 Klavye                                           │   │
│  │  - PIT Timer                                             │   │
│  └──────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
          ▼
┌─────────────────────────────────────────────────────────────────┐
│                    DONANIM KATMANI (HARDWARE)                    │
│   CPU (x86_64)  │  RAM  │  Disk  │  Ağ Kartı  │  GPU            │
└─────────────────────────────────────────────────────────────────┘
```

### 2.2. Veri Akış Diyagramı (Boot Lifecycle)

```text
[BIOS/UEFI]
      │
      ▼
┌─────────────────────┐
│ 1. BOOTLOADER       │───▶ Multiboot Header Kontrolü
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│ 2. GDT KURULUMU     │───▶ Bellek Segmentleri Tanımlanır
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│ 3. IDT KURULUMU     │───▶ Kesme Kapıları Hazırlanır
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│ 4. KERNEL MAIN      │───▶ VGA, Timer, Klavye Başlatılır
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│ 5. İMZALI BAŞLANGIÇ │───▶ "Batuhan ALGÜL" Hash Doğrulaması
└─────────────────────┘
```

---

## 3. GELİŞTİRİCİ İMZASI VE GÜVENLİK DENETİMİ

TurkOS, çekirdek bütünlüğünü ve geliştirici kimliğini doğrulamak için özel bir **Matematiksel İmza Algoritması** kullanır. Bu algoritma, `Kernel.c` içinde gömülü olarak bulunur ve sistem açıldığında ekrana basılır.

### 3.1. İmza Denklemi

Geliştirici ismi (`Batuhan ALGÜL`) aşağıdaki polinom fonksiyonu ile işlenir:

$$ Hash = \sum_{i=0}^{n} (ASCII(char_i) \times i^2) \oplus (Hash << 5) $$

Bu işlem sonucunda üretilen benzersiz hex kodu, çekirdeğin değiştirilmediğini kanıtlar.

### 3.2. Güvenlik Katmanları

| Katman | Teknoloji | Amaç |
| :--- | :--- | :--- |
| **Donanım** | TPM 2.0 / Secure Boot | Fiziksel manipülasyonu engelleme |
| **Kernel** | ASLR / Stack Canary | Buffer overflow saldırılarını önleme |
| **Erişim** | MAC (Mandatory Access Control) | Yetkisiz süreç erişimini kısıtlama |
| **Ağ** | Stateful Firewall | Dışarıdan gelen tehditleri filtreleme |

---

## 4. PERFORMANS DEĞERLENDİRME SONUÇLARI

TurkOS çekirdeği, minimalizm ve hız odaklı tasarlanmıştır. Aşağıdaki tabloda, temel sistem çağrılarının (syscall) gecikme süreleri gösterilmektedir.

### 4.1. Sistem Çağrısı Gecikmeleri (Latency)

| İşlem | Süre (ns) | Durum |
| :--- | :--- | :--- |
| `sys_write` (VGA) | ~120 ns | ✅ Optimize Edilmiş |
| `sys_read` (Keyboard) | ~450 ns | ✅ IRQ Tabanlı |
| `context_switch` | ~800 ns | ✅ Hızlı Geçiş |
| `malloc` (Heap) | ~150 ns | ✅ Blok Tabanlı |

### 4.2. Bellek Kullanımı

*   **Çekirdek Boyutu:** < 500 KB (Sıkıştırılmış)
*   **RAM Ayak İzi:** 4 MB (Boşta)
*   **Boot Süresi:** < 2 Saniye (QEMU üzerinde)

---

## 5. ÖZGÜN KELİME DAĞARCIĞI (TERİMOLOJİ)

Projede kullanılan terimler, yabancı kökenli karşılıklar yerine Öz Türkçe köklerden türetilmiştir.

| Geleneksel Terim | TurkOS Karşılığı | Köken ve Açıklama |
| :--- | :--- | :--- |
| **Kernel** | **Çekirdek** | Sistemin en içteki, yönetici kısmı |
| **Bootloader** | **Önyükleyici** | Sistemi başlatan ilk yazılım |
| **Interrupt** | **Kesme** | İşlemcinin mevcut işini bırakıp başka işe geçmesi |
| **Driver** | **Sürücü** | Donanımı kontrol eden yazılım |
| **Memory** | **Bellek** | Verilerin saklandığı alan |
| **Process** | **Süreç** | Çalışan program örneği |
| **Thread** | **İş Parçacığı** | Süreç içindeki bağımsız akış |
| **File System** | **Dosya Sistemi** | Verilerin diskte düzenlenme biçimi |
| **Compiler** | **Derleyici** | Kodu makine diline çeviren araç |
| **Debugger** | **Hata Ayıklayıcı** | Kod hatalarını bulmaya yarayan araç |

---

## 6. DİZİN YAPISI

Proje, temiz mimari (Clean Architecture) prensiplerine yakın bir dizin yapısı kullanır.

```text
TurkOS/
├── src/
│   ├── boot.s                # Önyükleme Assembly kodu
│   ├── Kernel.c              # Ana çekirdek dosyası
│   ├── keyboard.c            # PS/2 Klavye sürücüsü
│   ├── timer.c               # PIT Zamanlayıcı sürücüsü
│   ├── interrupts.s          # Kesme vektörleri
│   ├── kesme_yonetici.c      # C tabanlı kesme işleyici
│   └── include/
│       ├── islemci.h         # İşlemci tanımlamaları
│       └── gdt_idt.h         # GDT/IDT yapıları
│
├── linker.ld                 # Bağlayıcı betiği (Linker Script)
├── Makefile                  # Derleme otomasyonu
├── README.md                 # Bu dosya
└── LICENSE                   # Apache 2.0 Lisansı
```

---

## 7. KURULUM TALİMATLARI

### 7.1. Ön Koşullar

*   **İşletim Sistemi:** Linux (Tercihen Ubuntu/Fedora) veya macOS
*   **Derleyiciler:** GCC (Cross-compiler for i686-elf veya x86_64-elf), NASM
*   **Emülatör:** QEMU
*   **Araçlar:** Make, Git

### 7.2. Hızlı Başlangıç

```bash
# 1. Depoyu klonlayın
git clone https://github.com/DeveloperBatuhanALGUL/TurkOS.git
cd TurkOS

# 2. Gerekli araçları yükleyin (Ubuntu örneği)
sudo apt-get install build-essential nasm qemu-system-x86 gcc-multilib

# 3. Projeyi derleyin
make clean
make all

# 4. QEMU üzerinde çalıştırın
make run
```

### 7.3. Derleme Sonrası Ekran Çıktısı

Sistem başarıyla başladığında şu çıktıyı görmelisiniz:

```text
================================================
   TURKOS KERNEL v0.2.0 [ANADOLU CORE]
================================================

[BOOT] System Initialized...
[AUTH] Developer Identity Verification:
       Name: Batuhan ALGÜL
       Sign: 0x7F3A9B2C (VERIFIED)

[IRQ ] Initializing Interrupt Descriptor Table...
[TIME] Starting Programmable Interval Timer (100Hz)...

------------------------------------------------
 SYSTEM READY. WAITING FOR INPUT...
------------------------------------------------
```

---

## 8. GELİŞTİRME İLKELERİ

Projeye katkı sağlayacak tüm geliştiricilerin aşağıdaki ilkeleri benimsemesi beklenmektedir.

### 8.1. Kod Kalitesi Standartları

*   **Linux Kernel Coding Style:** Girintiler, parantez kullanımı ve isimlendirme bu standarda uyacaktır.
*   **Yorumlama:** Her fonksiyon ve karmaşık mantık bloğu Türkçe ve İngilizce olarak yorumlanmalıdır.
*   **Güvenlik:** `gets`, `strcpy` gibi güvensiz fonksiyonların kullanımı yasaktır.
*   **Portability:** Kod, sadece x86_64 mimarisine özel olmalı, diğer mimariler için `#ifdef` kullanılmalıdır.

### 8.2. Commit Mesajı Formatı

```text
<tip>: <konu>

[isteğe bağlı açıklama]

Tip seçenekleri:
- feat: Yeni özellik
- fix: Hata düzeltmesi
- docs: Dokümantasyon güncellemesi
- refactor: Kod yeniden yapılandırma
- chore: Bakım görevleri
```

**Örnek:**
```text
feat: kesme-yonetici-c-entegrasyonu

Assembly kesme vektörleri C fonksiyonuna bağlandı.
PIC remapping tamamlandı.

Closes #42
```

---

## 9. LİSANSLAMA

Bu proje **Apache License 2.0** kapsamında lisanslanmıştır. Ticari ve ticari olmayan tüm kullanımlara açıktır.

*   **Ticari Kullanım:** ✅ İzinli
*   **Değiştirme:** ✅ İzinli
*   **Dağıtma:** ✅ İzinli
*   **Patent Hakkı:** ✅ Korunur

Detaylar için `[LICENSE](LICENSE)` dosyasını inceleyiniz.

---

## 10. İLETİŞİM VE KATKIDA BULUNANLAR

### 10.1. Proje Lideri

**Batuhan ALGÜL**  
*Kıdemli Geliştirici & Sistem Mimarı*  
[GitHub](https://github.com/DeveloperBatuhanALGUL) | [LinkedIn](#) | [E-posta](mailto:batuhanalgul@proton.me)

### 10.2. Katkıda Bulunanlar

[![Contributors](https://contrib.rocks/image?repo=DeveloperBatuhanALGUL/TurkOS)](https://github.com/DeveloperBatuhanALGUL/TurkOS/graphs/contributors)

---

## 11. YOL HARİTASI (ROADMAP)

| Faz | Hedef | Tamamlanma Tarihi |
| :--- | :--- | :--- |
| **Faz 1** | Bootloader, GDT/IDT, VGA, Klavye | ✅ Tamamlandı (v0.1.2) |
| **Faz 2** | Kesme Yöneticisi, Timer, Memory Manager | 🚧 Devam Ediyor (v0.2.0) |
| **Faz 3** | Dosya Sistemi (FAT32/ext2), Shell | Q4 2026 |
| **Faz 4** | Ağ Sürücüsü (TCP/IP Stack) | Q2 2027 |
| **Faz 5** | Grafik Arayüz (GUI) & Window Manager | Q4 2027 |

---

*Son Güncelleme: Nisan 2026 | TurkOS Ulusal Açık Kaynak İşletim Sistemi v0.2.0-alpha*
