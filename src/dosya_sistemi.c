/* ========================================================================
   TurkOS - SANAL DOSYA SİSTEMİ ÇEKİRDEĞİ (VFS CORE)
   Tanım: Dosya işlemlerini yönlendiren merkezi VFS mantığı.
   İmza: Batuhan ALGÜL [Hash: 0x7F3A9B2C]
   Yazar: Batuhan ALGÜL & TurkOS Geliştirme Ekibi
   Tarih: 2026
   ======================================================================== */

#include "include/dosya_sistemi.h"
#include <string.h> // strlen, memcmp gibi fonksiyonlar için

/* Basit RAM Tabanlı Dosya Sistemi Değişkenleri */
static Inode inode_tablosu[128]; /* Maksimum 128 dosya/dizin desteği */
static AcikDosya acik_dosya_tablosu[MAX_ACILIK_DOSYA_SAYISI];
static uint32_t sonraki_inode_no = 1;
static uint32_t sonraki_fd_no = 3; /* 0,1,2 stdin/stdout/stderr için ayrılmış */

/* Yardımcı Fonksiyon: Yol Ayrıştırıcı (Basit Versiyon) */
static int yol_parcala(const char* yol, char* dizin, char* dosya_adi) {
    const char* son_egl = strrchr(yol, '/');
    if (!son_egl) {
        strcpy(dizin, "/");
        strcpy(dosya_adi, yol);
        return 0;
    }
    
    size_t dizin_uzunlugu = son_egl - yol;
    if (dizin_uzunlugu == 0) {
        strcpy(dizin, "/");
    } else {
        strncpy(dizin, yol, dizin_uzunlugu);
        dizin[dizin_uzunlugu] = '\0';
    }
    
    strcpy(dosya_adi, son_egl + 1);
    return 0;
}

/* VFS Başlatma */
void vfs_baslat() {
    memset(inode_tablosu, 0, sizeof(inode_tablosu));
    memset(acik_dosya_tablosu, 0, sizeof(acik_dosya_tablosu));
    
    /* Kök Dizini Oluştur (/) */
    inode_tablosu[0].inode_no = 0;
    inode_tablosu[0].tur = DOSYA_TURU_DIZIN;
    inode_tablosu[0].boyut = 0;
    strcpy(inode_tablosu[0].isim, "/");
    inode_tablosu[0].ebeveyn_inode = 0;
    
    sonraki_inode_no = 1;
}

/* Inode Arama Fonksiyonu */
Inode* vfs_inode_bul(const char* yol) {
    /* Basit doğrusal arama (Optimizasyon gerektirir) */
    for (int i = 0; i < 128; i++) {
        if (inode_tablosu[i].inode_no != 0 && strcmp(inode_tablosu[i].isim, yol) == 0) {
            return &inode_tablosu[i];
        }
    }
    return NULL;
}

/* Dosya Açma İşlemi */
int vfs_dosya_ac(const char* yol, DosyaModu mod) {
    Inode* inode = vfs_inode_bul(yol);
    
    if (!inode) {
        /* Dosya yoksa oluştur (Sadece normal dosyalar için) */
        if (mod & MOD_YAZMA) {
            int index = -1;
            for (int i = 0; i < 128; i++) {
                if (inode_tablosu[i].inode_no == 0) {
                    index = i;
                    break;
                }
            }
            
            if (index == -1) return -1; /* Tablo dolu */
            
            inode = &inode_tablosu[index];
            inode->inode_no = sonraki_inode_no++;
            inode->tur = DOSYA_TURU_NORMAL;
            inode->boyut = 0;
            strcpy(inode->isim, yol);
            inode->ebeveyn_inode = 0; /* Şimdilik kök dizin */
        } else {
            return -1; /* Dosya bulunamadı */
        }
    }
    
    /* Boş FD slotu bul */
    int fd_index = -1;
    for (int i = 0; i < MAX_ACILIK_DOSYA_SAYISI; i++) {
        if (!acik_dosya_tablosu[i].kullaniliyor_mu) {
            fd_index = i;
            break;
        }
    }
    
    if (fd_index == -1) return -1; /* Çok fazla açık dosya */
    
    acik_dosya_tablosu[fd_index].fd = sonraki_fd_no++;
    acik_dosya_tablosu[fd_index].inode = inode;
    acik_dosya_tablosu[fd_index].konum_imleci = 0;
    acik_dosya_tablosu[fd_index].mod = mod;
    acik_dosya_tablosu[fd_index].kullaniliyor_mu = true;
    
    return acik_dosya_tablosu[fd_index].fd;
}

/* Dosya Okuma İşlemi */
int vfs_dosya_oku(int fd, void* tampon, size_t boyut) {
    AcikDosya* dosya = NULL;
    for (int i = 0; i < MAX_ACILIK_DOSYA_SAYISI; i++) {
        if (acik_dosya_tablosu[i].fd == fd && acik_dosya_tablosu[i].kullaniliyor_mu) {
            dosya = &acik_dosya_tablosu[i];
            break;
        }
    }
    
    if (!dosya) return -1;
    if (!(dosya->mod & MOD_OKUMA)) return -1; /* Okuma izni yok */
    
    /* Gerçek disk okuması burada yapılacak. Şimdilik RAM simülasyonu. */
    /* Not: Bu kısım ileride disk sürücüsü ile değiştirilecek. */
    size_t okunacak = boyut;
    if (dosya->konum_imleci + okunacak > dosya->inode->boyut) {
        okunacak = dosya->inode->boyut - dosya->konum_imleci;
    }
    
    /* Simüle edilmiş veri kopyalama */
    memset(tampon, 'A', okunacak); 
    
    dosya->konum_imleci += okunacak;
    return (int)okunacak;
}

/* Dosya Yazma İşlemi */
int vfs_dosya_yaz(int fd, const void* tampon, size_t boyut) {
    AcikDosya* dosya = NULL;
    for (int i = 0; i < MAX_ACILIK_DOSYA_SAYISI; i++) {
        if (acik_dosya_tablosu[i].fd == fd && acik_dosya_tablosu[i].kullaniliyor_mu) {
            dosya = &acik_dosya_tablosu[i];
            break;
        }
    }
    
    if (!dosya) return -1;
    if (!(dosya->mod & MOD_YAZMA)) return -1; /* Yazma izni yok */
    
    /* Gerçek disk yazması burada yapılacak. */
    /* Simüle edilmiş boyut güncellemesi */
    dosya->inode->boyut += boyut;
    dosya->konum_imleci += boyut;
    
    return (int)boyut;
}

/* Dosya Kapatma İşlemi */
void vfs_dosya_kapat(int fd) {
    for (int i = 0; i < MAX_ACILIK_DOSYA_SAYISI; i++) {
        if (acik_dosya_tablosu[i].fd == fd && acik_dosya_tablosu[i].kullaniliyor_mu) {
            acik_dosya_tablosu[i].kullaniliyor_mu = false;
            return;
        }
    }
}
