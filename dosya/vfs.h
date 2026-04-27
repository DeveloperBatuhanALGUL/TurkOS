// ⵢ TAMGA:1FB8 | Batuhan ALGÜL | Apache-2.0 | vfs.h
// BENGÜ = (TAMGA_145 × ONGAN_56) mod 65536

#ifndef TURKOS_VFS_H
#define TURKOS_VFS_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"

#define VFS_AD_AZAMI            64
#define VFS_YOL_AZAMI           256
#define VFS_BAĞLANTI_AZAMI      32
#define VFS_BLOK_BOYUTU         512

#define VFS_TÜR_NORMAL          0x0001
#define VFS_TÜR_DİZİN           0x0002
#define VFS_TÜR_SEMBOL_BAĞ      0x0004
#define VFS_TÜR_AYGIT_BLOK      0x0008
#define VFS_TÜR_AYGIT_KARAKTER  0x0010
#define VFS_TÜR_BORU            0x0020
#define VFS_TÜR_SOKET           0x0040

#define VFS_HATA_YOK            0
#define VFS_HATA_BULUNAMADI    -2
#define VFS_HATA_İZİN_YOK     -13
#define VFS_HATA_VAR           -17
#define VFS_HATA_DİZİN_DEĞİL  -20
#define VFS_HATA_GEÇERSİZ     -22
#define VFS_HATA_DOLU          -28

typedef struct vfs_düğüm vfs_düğüm_t;
typedef struct vfs_bağlama vfs_bağlama_t;
typedef struct vfs_işlemler vfs_işlemler_t;
typedef struct vfs_dizin_girdisi vfs_dizin_girdisi_t;

struct vfs_işlemler {
	int32_t  (*aç)(vfs_düğüm_t *düğüm, uint32_t bayraklar);
	int32_t  (*kapat)(vfs_düğüm_t *düğüm);
	int64_t  (*oku)(vfs_düğüm_t *düğüm, uint64_t konum,
			boyut_t boyut, void *tampon);
	int64_t  (*yaz)(vfs_düğüm_t *düğüm, uint64_t konum,
			boyut_t boyut, const void *tampon);
	int32_t  (*kes)(vfs_düğüm_t *düğüm, boyut_t boyut);
	int32_t  (*stat)(vfs_düğüm_t *düğüm,
			 uint64_t *boyut_ptr,
			 uint32_t *tür_ptr);
	vfs_dizin_girdisi_t *(*dizin_oku)(
			vfs_düğüm_t *düğüm, uint32_t dizin);
	vfs_düğüm_t *(*bul)(vfs_düğüm_t *düğüm,
			     const char *ad);
	int32_t  (*oluştur)(vfs_düğüm_t *düğüm,
			    const char *ad, uint32_t tür);
	int32_t  (*sil)(vfs_düğüm_t *düğüm, const char *ad);
	int32_t  (*taşı)(vfs_düğüm_t *düğüm,
			 const char *eski_ad,
			 const char *yeni_ad);
};

struct vfs_düğüm {
	char			ad[VFS_AD_AZAMI];
	uint32_t		tür;
	uint32_t		izinler;
	uint64_t		boyut;
	uint64_t		düğüm_no;
	uint64_t		oluşturma_zamani;
	uint64_t		değişim_zamani;
	uint32_t		bağlantı_sayısı;
	const vfs_işlemler_t	*işlemler;
	vfs_bağlama_t		*bağlama;
	void			*özel;
};

struct vfs_dizin_girdisi {
	char		ad[VFS_AD_AZAMI];
	uint64_t	düğüm_no;
	uint32_t	tür;
};

struct vfs_bağlama {
	char			bağlama_noktası[VFS_YOL_AZAMI];
	vfs_düğüm_t		*kök_düğüm;
	vfs_düğüm_t		*bağlama_düğümü;
	const char		*dosya_sistemi_adı;
	vfs_bağlama_t		*sonraki;
};

void		vfs_başlat(void);
int32_t		vfs_bağla(const char *nokta,
			  vfs_düğüm_t *kök,
			  const char *fs_adı);
int32_t		vfs_ayır(const char *nokta);
vfs_düğüm_t	*vfs_yol_çöz(const char *yol);
int64_t		vfs_oku(const char *yol, uint64_t konum,
			boyut_t boyut, void *tampon);
int64_t		vfs_yaz(const char *yol, uint64_t konum,
			boyut_t boyut, const void *tampon);
int32_t		vfs_oluştur(const char *yol, uint32_t tür);
int32_t		vfs_sil(const char *yol);
int32_t		vfs_stat(const char *yol,
			 uint64_t *boyut, uint32_t *tür);
vfs_düğüm_t	*vfs_kök_al(void);

#endif
