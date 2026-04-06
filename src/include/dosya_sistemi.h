/* ========================================================================
   TurkOS - SANAL DOSYA SİSTEMİ BAŞLIKLARI
   Tanım: VFS arayüzü, inode yapıları ve dosya işlemleri için tanımlamalar.
   İmza: Batuhan ALGÜL [Hash: 0x7F3A9B2C]
   Yazar: Batuhan ALGÜL & TurkOS Geliştirme Ekibi
   Tarih: 2026
   ======================================================================== */

#ifndef DOSYA_SISTEMI_H
#define DOSYA_SISTEMI_H

#include <stdint.h>
#include <stddef.h>

#define MAX_DOSYA_ADI_UZUNLUGU 64
#define MAX_YOL_UZUNLUGU 256
#define MAX_ACILIK_DOSYA_SAYISI 64

/* Dosya Türleri */
typedef enum {
    DOSYA_TURU_NORMAL = 1,
    DOSYA_TURU_DIZIN = 2,
    DOSYA_TURU_CIHAZ = 3,
    DOSYA_TURU_BAGLANTI = 4
} DosyaTuru;

/* Dosya Erişim Modları */
typedef enum {
    MOD_OKUMA = 0x01,
    MOD_YAZMA = 0x02,
    MOD_EKLEME = 0x04
} DosyaModu;

/* Inode (Dosya Düğümü) Yapısı */
typedef struct {
    uint32_t inode_no;          /* Benzersiz inode numarası */
    DosyaTuru tur;              /* Dosya türü */
    uint32_t boyut;             /* Dosya boyutu (byte) */
    uint32_t blok_adresi;       /* Verinin başladığı disk bloğu */
    char isim[MAX_DOSYA_ADI_UZUNLUGU];
    uint32_t ebeveyn_inode;     /* Üst dizinin inode numarası */
} Inode;

/* Açık Dosya Tanımlayıcısı (File Descriptor) */
typedef struct {
    int fd;                     /* Dosya tanımlayıcı numarası */
    Inode* inode;               /* İlgili inode göstericisi */
    uint32_t konum_imleci;      /* Okuma/yazma imlecinin konumu */
    DosyaModu mod;              /* Açılış modu */
    bool kullaniliyor_mu;       /* Slotun dolu olup olmadığı */
} AcikDosya;

/* VFS İşlem Fonksiyon Prototipleri */
void vfs_baslat();
int vfs_dosya_ac(const char* yol, DosyaModu mod);
int vfs_dosya_oku(int fd, void* tampon, size_t boyut);
int vfs_dosya_yaz(int fd, const void* tampon, size_t boyut);
void vfs_dosya_kapat(int fd);
int vfs_dizin_olustur(const char* yol);
Inode* vfs_inode_bul(const char* yol);

#endif
