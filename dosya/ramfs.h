// ⵢ TAMGA:25F3 | Batuhan ALGÜL | Apache-2.0 | ramfs.h
// BENGÜ = (TAMGA_145 × ONGAN_67) mod 65536

#ifndef TURKOS_RAMFS_H
#define TURKOS_RAMFS_H

#include "../ortak/türler.h"
#include "../ortak/bağlantı.h"
#include "vfs.h"

#define RAMFS_AD                "ramfs"
#define RAMFS_AZAMI_DÜĞÜM       128
#define RAMFS_AZAMI_VERİ        0x100000
#define RAMFS_AZAMI_ÇOCUK       16
#define RAMFS_BLOK_BOYUTU       512

typedef struct ramfs_düğüm {
	vfs_düğüm_t		vfs;
	uint8_t			*veri;
	boyut_t			veri_boyutu;
	struct ramfs_düğüm	*çocuklar[RAMFS_AZAMI_ÇOCUK];
	uint8_t			çocuk_sayısı;
	struct ramfs_düğüm	*ana;
	uint8_t			kullanımda;
} ramfs_düğüm_t;

typedef struct {
	ramfs_düğüm_t	düğümler[RAMFS_AZAMI_DÜĞÜM];
	uint32_t	kullanılan_düğüm;
	boyut_t		toplam_veri;
	vfs_düğüm_t	*kök;
} ramfs_süper_t;

vfs_düğüm_t	*ramfs_başlat(void);
ramfs_düğüm_t	*ramfs_düğüm_oluştur(
			ramfs_süper_t *süper,
			const char *ad,
			uint32_t tür);
int32_t		ramfs_dosya_yaz(
			ramfs_düğüm_t *düğüm,
			const void *veri,
			boyut_t boyut);

#endif
