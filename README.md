# TurkOS
Türk yapımı açık kaynak işletim sistemi | Turkish made open source operating system


TürkOS

Türkiye Cumhuriyeti'nin İlk Bağımsız ve Milli İşletim Sistemi Projesi

---

PROJE HAKKINDA

TürkOS, Türk Milleti'nin teknolojik egemenliği ve siber bağımsızlığı için tasarlanan, tamamen açık kaynak kodlu, yerli ve milli bir işletim sistemi projesidir. Proje, Türkiye Cumhuriyeti'nin bilişim teknolojileri alanında dışa bağımlılığını azaltmak ve stratejik alanlarda teknolojik özgürlüğünü sağlamak amacıyla geliştirilmektedir.

TürkOS, askeri, kurumsal ve sivil alanlarda kullanılmak üzere tasarlanmış, uluslararası güvenlik standartlarına uygun, denetlenebilir ve şeffaf bir platform sunmayı hedeflemektedir.

---

MİSYON VE VİZYON

Misyon
Türk Milleti'ne ait, bağımsız, güvenli ve sürdürülebilir bir işletim sistemi altyapısı oluşturarak ulusal siber egemenliğe katkı sağlamak.

Vizyon
Türkiye Cumhuriyeti'ni bilişim teknolojileri alanında küresel bir güç haline getirmek ve açık kaynak felsefesiyle uluslararası işbirliğine öncülük etmek.

---

STRATEJİK HEDEFLER

1. Siber Egemenlik: Kritik altyapılarda yabancı yazılım bağımlılığının ortadan kaldırılması
2. Teknolojik Bağımsızlık: Ulusal savunma ve kamu kurumlarında yerli işletim sistemi kullanımı
3. Açık Kaynak Liderliği: Uluslararası açık kaynak topluluklarında Türkiye'nin etkin rol alması
4. İnsan Kaynağı Gelişimi: Yerli yazılım mühendisleri ve siber güvenlik uzmanları yetiştirilmesi
5. Ekonomik Katkı: Yazılım ihracatı ve teknoloji transferi yoluyla ulusal ekonomiye katkı

---

SİSTEM ÖZELLİKLERİ

Çekirdek Mimari
- Mikrokernel tabanlı modüler mimari
- Bellek koruma mekanizmaları ve izolasyon
- Gerçek zamanlı görev çizelgeleme
- Çoklu işlemci ve çoklu çekirdek desteği
- Donanım sanallaştırma desteği

Güvenlik Altyapısı
- Askeri düzey şifreleme protokolleri (AES-256, RSA-4096)
- Mandatory Access Control (MAC) tabanlı erişim kontrolü
- Kernel düzeyinde güvenlik denetimi ve loglama
- Sandboxing ve containerization desteği
- Secure Boot ve güvenli önyükleme
- Integrity Measurement Architecture (IMA)
- TPM 2.0 donanım güvenlik modülü entegrasyonu

Ağ ve İletişim
- IPv4/IPv6 tam desteği
- Gelişmiş firewall ve IDS/IPS entegrasyonu
- VPN ve encrypted tunnel protokolleri
- Quantum-safe kriptografi hazırlığı

---

ANADOLU ZEKA (ANATOLIA AI)

TürkOS'un en kritik bileşenlerinden biri, sisteme entegre edilmiş **Anadolu Zeka** (Anatolia AI) yapay zeka motorudur. Bu motor, sistemin yerel olarak çalışan, internet bağlantısı gerektirmeyen ve kullanıcı verilerini dış sunuculara göndermeden işleyen bağımsız bir yapay zeka çözümüdür.

Teknik Özellikler
- Yerel çalışma (Offline AI): Tüm hesaplamalar cihaz üzerinde
- Doğal dil işleme: Türkçe ve 193 BM üyesi ülkenin resmi dilleri
- Kod asistanı: C, C++, Python, Rust, Assembly dil desteği
- Sistem optimizasyonu: Performans analizi ve öneriler
- Güvenlik analizi: Tehdit tespiti ve önleme önerileri
- Veri mahremiyeti: Hiçbir veri sistem dışına çıkmaz

Uygulama Alanları
- Yazılım geliştirme asistanlığı
- Sistem yönetimi ve optimizasyonu
- Doküman analizi ve özetleme
- Siber güvenlik tehdit analizi
- Kod incelemesi ve güvenlik taraması

---

YERLİ UYGULAMA PAKETİ: YAZMAN

TürkOS, ofis üretkenliği için **Yazman** adlı yerli uygulama paketini içermektedir. Yazman, modern iş ihtiyaçlarına cevap veren, açık standartlara uyumlu ve güvenlik odaklı bir ofis paketidir.

Yazman Bileşenleri

Yazman Belge(Kelime İşlemci)
- Tam Unicode desteği ile çok dilli belge oluşturma
- DOCX, ODT, PDF formatlarında okuma ve yazma
- Gelişmiş tipografi ve sayfa düzeni
- İşbirlikçi düzenleme özellikleri
- Dijital imza desteği

Yazman Çizelge (Hesap Tablosu)
- Karmaşık formül ve fonksiyon desteği
- XLSX, ODS, CSV format uyumluluğu
- Veri görselleştirme ve grafik araçları
- Makro ve otomasyon desteği
- Büyük veri setleri için optimize edilmiş performans

Yazman Sunu (Sunum Programı)
- PPTX, ODP format desteği
- Animasyon ve geçiş efektleri
- Multimedya entegrasyonu
- Sunum kaydedici
- Uzaktan sunum özellikleri

Yazman Veritabanı
- İlişkisel veritabanı yönetimi
- SQL sorgu editörü
- Form ve rapor tasarımcısı
- Veri içe/dışa aktarma araçları

Tüm Yazman uygulamaları açık kaynak kodludur ve uluslararası belge standartlarına tam uyumludur.

---

#GÜVENLİK MİMARİSİ

TürkOS güvenlik mimarisi, NATO ve uluslararası güvenlik standartlarına uygun olarak tasarlanmıştır.

Çok Katmanlı Güvenlik Modeli

Katman 1: Donanım Güvenliği
- Secure Boot ve UEFI güvenliği
- TPM 2.0 hardware security module
- Memory encryption (AMD SEV, Intel TME)
- Hardware random number generator

Katman 2: Kernel Güvenliği
- Address Space Layout Randomization (ASLR)
- Stack canaries ve buffer overflow koruması
- Control Flow Integrity (CFI)
- Kernel address space isolation
- Capability-based security model

Katman 3: Sistem Güvenliği
- SELinux benzeri mandatory access control
- Role-based access control (RBAC)
- Namespace isolation
- Seccomp ve syscall filtering
- Audit subsystem ile kapsamlı loglama

Katman 4: Uygulama Güvenliği
- Application sandboxing
- Capability dropping
- Encrypted file system
- Secure IPC mekanizmaları

Katman 5: Ağ Güvenliği
- Stateful packet inspection
- Intrusion detection ve prevention
- DDoS mitigation
- Encrypted DNS (DoT, DoH)
- Network namespace isolation

Sertifikasyon Hedefleri
- Common Criteria EAL4+ sertifikasyonu
- NATO ülkelerinde kullanım onayı
- FIPS 140-3 kriptografi standardı uyumluluğu
- Milli güvenlik kurumlarının onayı

---

SİSTEM MİMARİSİ

```
┌─────────────────────────────────────────────────────────┐
│                   Kullanıcı Alanı                       │
│  ┌──────────┐  ┌──────────┐  ┌──────────────────────┐  │
│  │  Yazman  │  │ Anadolu  │  │  Sistem Uygulamaları │  │
│  │  Paketi  │  │   Zeka   │  │                      │  │
│  └──────────┘  └──────────┘  └──────────────────────┘  │
├─────────────────────────────────────────────────────────┤
│              Sistem Kütüphaneleri (libc, libm)          │
├─────────────────────────────────────────────────────────┤
│                    Sistem Çağrıları                      │
├─────────────────────────────────────────────────────────┤
│                     TürkOS Kernel                        │
│  ┌────────────┐  ┌────────────┐  ┌─────────────────┐   │
│  │   Bellek   │  │   Süreç    │  │    Dosya        │   │
│  │  Yönetimi  │  │  Yönetimi  │  │   Sistemi       │   │
│  └────────────┘  └────────────┘  └─────────────────┘   │
│  ┌────────────┐  ┌────────────┐  ┌─────────────────┐   │
│  │    Ağ      │  │  Güvenlik  │  │    Sürücü       │   │
│  │  Alt Yap   │  │   Modülü   │  │   Yönetimi      │   │
│  └────────────┘  └────────────┘  └─────────────────┘   │
├─────────────────────────────────────────────────────────┤
│                    Donanım Katmanı                       │
│   CPU  │  Bellek  │  Disk  │  Ağ  │  Grafik  │ I/O    │
└─────────────────────────────────────────────────────────┘
```

---

ULUSLARARASI DİL DESTEĞİ

TürkOS, küresel kullanım için Birleşmiş Milletler üyesi 193 ülkenin resmi dillerini desteklemektedir. Bu dil desteği yalnızca arayüz çevirisi değil, aynı zamanda sistem belgelerini, hata mesajlarını ve Anadolu Zeka yapay zeka motorunu da kapsamaktadır.

Desteklenen Dil Kategorileri

Birleşmiş Milletler Resmi Dilleri:
Arapça, Çince, İngilizce, Fransızca, Rusça, İspanyolca

NATO Üyesi Ülke Dilleri:
Almanca, Arnavutça, Bulgarca, Çekçe, Danca, Estonyaca, Fince, Flemenkçe, Hırvatça, İtalyanca, Letonca, Litvanca, Macarca, Norveççe, Lehçe, Portekizce, Romence, Slovakça, Slovence, İzlandaca

Diğer Uluslararası Diller:
Japonca, Korece, Urduca, Hintçe, Bengalce, Azerice, Özbekçe, Kazakça, Kırgızca, Tacikçe, Türkmence, Farsça, Peştuca ve tüm BM üyesi ülkelerin resmi dilleri

Dil desteği ISO 639 standartlarına uygun olarak geliştirilmektedir.

---

YAZILIM MAĞAZASI: TÜRK MARKET

TürkOS, kullanıcıların güvenli bir şekilde uygulama edinebilmesi için **Türk Market** adlı merkezi uygulama deposunu içermektedir.

 Mağaza Özellikleri
- Dijital imza ile doğrulanmış uygulamalar
- Otomatik güvenlik taraması
- Sandboxing ile güvenli uygulama çalıştırma
- Sürüm kontrolü ve otomatik güncelleme
- Kullanıcı yorumları ve değerlendirmeleri
- Açık kaynak ve özgür yazılım önceliği

Uygulama Kategorileri
- Geliştirme Araçları
- Ofis ve Üretkenlik
- Multimedya
- Ağ ve İnternet
- Sistem Araçları
- Eğitim ve Bilim
- Grafik ve Tasarım

Tüm uygulamalar katı güvenlik incelemesinden geçirilir ve TürkOS güvenlik standartlarına uygunluk denetimi yapılır.

---

 KURULUM VE SİSTEM GEREKSİNİMLERİ

 Minimum Sistem Gereksinimleri
- İşlemci: 64-bit x86_64 veya ARM64 mimari
- Bellek: 2 GB RAM
- Disk: 20 GB boş alan
- Grafik: 1024x768 çözünürlük

Önerilen Sistem Gereksinimleri
- İşlemci: Çok çekirdekli 64-bit işlemci
- Bellek: 4 GB RAM veya üzeri
- Disk: 50 GB SSD
- Grafik: 1920x1080 veya üzeri çözünürlük

 Kurulum Yöntemleri
- USB bellek üzerinden canlı kurulum
- Ağ üzerinden (PXE Boot) kurulum
- Sanal makine üzerinde kurulum (test ortamı)
- Dual-boot yapılandırması

Detaylı kurulum talimatları için `docs/INSTALLATION.md` dosyasına bakınız.

---

GELİŞTİRME ORTAMI

Gerekli Araçlar
- GCC 11.0 veya üzeri (C derleyici)
- NASM 2.15 veya üzeri (Assembly derleyici)
- GNU Make 4.3 veya üzeri
- QEMU 6.0 veya üzeri (Emülatör)
- Git 2.30 veya üzeri (Sürüm kontrolü)

 Derleme
```bash
git clone https://github.com/DeveloperBatuhanALGUL/TurkOS.git
cd TurkOS
make clean
make all
```

Test Ortamı
```bash
make test
make run-qemu
```

---

KATKIDA BULUNMA

TürkOS, Türk Milleti'nin ortak eseridir ve katkıya açıktır. Proje, açık kaynak felsefesine bağlı kalarak topluluk katkılarıyla gelişmektedir.

Katkı Alanları
- Kernel geliştirme
- Sürücü yazımı
- Güvenlik araştırması ve penetrasyon testi
- Belgelendirme ve çeviri
- Anadolu Zeka yapay zeka modelinin eğitimi
- Yazman ofis paketi geliştirme
- Kullanıcı arayüzü tasarımı

Katkı Süreci
1. Projeyi fork ediniz
2. Yeni bir feature branch oluşturunuz (`git checkout -b feature/yeni-ozellik`)
3. Değişikliklerinizi commit ediniz (`git commit -m 'feat: yeni özellik eklendi'`)
4. Branch'inizi push ediniz (`git push origin feature/yeni-ozellik`)
5. Pull Request oluşturunuz

Detaylı katkı kuralları için `CONTRIBUTING.md` dosyasını inceleyiniz.

Kod Standartları
- Linux Kernel Coding Style standardına uyum
- MISRA-C güvenlik kurallarına uygunluk
- Kapsamlı kod yorumlaması (Türkçe ve İngilizce)
- Unit test coverage minimum %80

---

GÜVENLİK AÇIĞI BİLDİRİMİ

TürkOS güvenliği ciddiye alınmaktadır. Güvenlik açığı tespit ederseniz:

Rapor Kanalı: security@turkos.org (şifreleme: PGP key ID: XXXX)

Lütfen güvenlik açıklarını public issue tracker üzerinden paylaşmayınız. Sorumlu açıklama politikamız:
- 90 gün içinde patch yayınlanması
- Bulucu bilgisinin (isteğe bağlı) belirtilmesi
- Hall of Fame listesine ekleme

---

PROJE YÖNETİŞİMİ

TürkOS açık yönetişim modeliyle geliştirilmektedir:

- Technical Steering Committee: Teknik kararların alındığı kurul
- Security Working Group: Güvenlik standartlarının belirlenmesi
- Community Council: Topluluk yönetimi ve koordinasyon

Tüm kararlar şeffaf bir şekilde documentation repository üzerinde belgelenmektedir.

---

LİSANS

TürkOS, Apache License 2.0 ile lisanslanmıştır. Bu lisans:
- Ticari kullanıma izin verir
- Patent koruması sağlar
- Değişikliklerin belirtilmesini gerektirir
- Kaynak kodunun açık kalması zorunluluğu getirmez

Detaylar için `LICENSE` dosyasını inceleyiniz.

```
Copyright 2026 TürkOS Geliştirme Ekibi
Batuhan ALGÜL ve Katkıda Bulunanlar

Licensed under the Apache License, Version 2.0
```

---

İLETİŞİM VE KAYNAKLAR

Proje Deposu: https://github.com/DeveloperBatuhanALGUL/TurkOS

Belgelendirme: https://docs.turkos.org

Topluluk Forumu: https://community.turkos.org

Resmi Web Sitesi: https://turkos.org

E-posta: info@turkos.org

IRC: turkos on irc.libera.chat

---

 YOL HARİTASI

Faz 1: Temel Altyapı (Q1-Q2 2026)
- Bootloader geliştirme ve UEFI desteği
- Minimal kernel implementasyonu
- Temel sürücü altyapısı
- Bellek yönetimi ve paging

Faz 2: Sistem Servisleri (Q3-Q4 2026)
- Dosya sistemi implementasyonu (ext4, btrfs)
- Ağ stack'i ve TCP/IP protokolleri
- Süreç yönetimi ve scheduler
- Sistem çağrıları API'si

 Faz 3: Güvenlik ve AI (Q1-Q2 2027)
- Güvenlik modüllerinin entegrasyonu
- Anadolu Zeka AI motorunun kernel entegrasyonu
- Sandboxing ve containerization
- Kriptografi kütüphaneleri

 Faz 4: Kullanıcı Alanı (Q3-Q4 2027)
- Grafik sunucu implementasyonu (Wayland)
- Yazman ofis paketi geliştirme
- Türk Market uygulama mağazası
- Desktop environment

 Faz 5: Optimizasyon ve Sertifikasyon (2028)
- Performans optimizasyonları
- Common Criteria sertifikasyon süreci
- Kapsamlı güvenlik auditi
- Resmi v1.0 sürümü

---

TürkOS - Bağımsız Türkiye'nin Bağımsız İşletim Sistemi

Bu proje Türk Milleti'nin teknolojik egemenliği için geliştirilmektedir.

Batuhan ALGÜL | Yüce Türk Milletinin Bir Ferdi...
