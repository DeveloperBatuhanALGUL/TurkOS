// ⵢ TAMGA:25F3 | Batuhan ALGÜL | Apache-2.0 | ramfs.c
// BENGÜ = (TAMGA_145 × ONGAN_67) mod 65536

#include "ramfs.h"
#include "../bellek/yığın_yönetici.h"
#include "../ortak/dizge.h"

static ramfs_süper_t	ramfs_süper;
static vfs_işlemler_t	ramfs_işlemler;

static ramfs_düğüm_t *vfs_den_ramfs(vfs_düğüm_t *vfs)
{
	return (ramfs_düğüm_t *)vfs;
}

static int32_t ramfs_aç(vfs_düğüm_t *düğüm, uint32_t bayraklar)
{
	(void)bayraklar;
	return düğüm ? VFS_HATA_YOK : VFS_HATA_GEÇERSİZ;
}

static int32_t ramfs_kapat(vfs_düğüm_t *düğüm)
{
	(void)düğüm;
	return VFS_HATA_YOK;
}

static int64_t ramfs_oku(
	vfs_düğüm_t *düğüm, uint64_t konum,
	boyut_t boyut, void *tampon)
{
	ramfs_düğüm_t *r = vfs_den_ramfs(düğüm);
	if (!r->veri || konum >= r->veri_boyutu)
		return 0;

	boyut_t okunacak = r->veri_boyutu - (boyut_t)konum;
	if (okunacak > boyut)
		okunacak = boyut;

	bellek_kopyala(tampon, r->veri + konum, okunacak);
	return (int64_t)okunacak;
}

static int64_t ramfs_yaz(
	vfs_düğüm_t *düğüm, uint64_t konum,
	boyut_t boyut, const void *tampon)
{
	ramfs_düğüm_t *r = vfs_den_ramfs(düğüm);
	boyut_t yeni_boyut = (boyut_t)konum + boyut;

	if (yeni_boyut > r->veri_boyutu) {
		uint8_t *yeni_veri = yigin_ayır(yeni_boyut);
		if (!yeni_veri)
			return -12;
		if (r->veri) {
			bellek_kopyala(yeni_veri, r->veri,
				r->veri_boyutu);
			yigin_serbest(r->veri);
		}
		r->veri       = yeni_veri;
		r->veri_boyutu = yeni_boyut;
		r->vfs.boyut   = yeni_boyut;
	}

	bellek_kopyala(r->veri + konum, tampon, boyut);
	ramfs_süper.toplam_veri += boyut;
	return (int64_t)boyut;
}

static int32_t ramfs_kes(vfs_düğüm_t *düğüm, boyut_t boyut)
{
	ramfs_düğüm_t *r = vfs_den_ramfs(düğüm);
	if (boyut == 0) {
		if (r->veri)
			yigin_serbest(r->veri);
		r->veri        = BOŞ;
		r->veri_boyutu = 0;
		r->vfs.boyut   = 0;
		return VFS_HATA_YOK;
	}
	if (boyut > r->veri_boyutu)
		return VFS_HATA_GEÇERSİZ;
	r->veri_boyutu = boyut;
	r->vfs.boyut   = boyut;
	return VFS_HATA_YOK;
}

static int32_t ramfs_stat(
	vfs_düğüm_t *düğüm,
	uint64_t *boyut_ptr, uint32_t *tür_ptr)
{
	if (boyut_ptr)
		*boyut_ptr = düğüm->boyut;
	if (tür_ptr)
		*tür_ptr = düğüm->tür;
	return VFS_HATA_YOK;
}

static vfs_dizin_girdisi_t *ramfs_dizin_oku(
	vfs_düğüm_t *düğüm, uint32_t dizin)
{
	ramfs_düğüm_t *r = vfs_den_ramfs(düğüm);
	if (dizin >= r->çocuk_sayısı)
		return BOŞ;

	vfs_dizin_girdisi_t *girdi =
		yigin_ayır(sizeof(vfs_dizin_girdisi_t));
	if (!girdi)
		return BOŞ;

	dizge_kopyala(girdi->ad,
		r->çocuklar[dizin]->vfs.ad, VFS_AD_AZAMI);
	girdi->düğüm_no = r->çocuklar[dizin]->vfs.düğüm_no;
	girdi->tür      = r->çocuklar[dizin]->vfs.tür;
	return girdi;
}

static vfs_düğüm_t *ramfs_bul(
	vfs_düğüm_t *düğüm, const char *ad)
{
	ramfs_düğüm_t *r = vfs_den_ramfs(düğüm);
	for (uint8_t i = 0; i < r->çocuk_sayısı; i++) {
		if (dizge_karşılaştır(
			r->çocuklar[i]->vfs.ad, ad) == 0)
			return &r->çocuklar[i]->vfs;
	}
	return BOŞ;
}

static int32_t ramfs_oluştur(
	vfs_düğüm_t *düğüm, const char *ad, uint32_t tür)
{
	ramfs_düğüm_t *ana = vfs_den_ramfs(düğüm);
	if (ana->çocuk_sayısı >= RAMFS_AZAMI_ÇOCUK)
		return VFS_HATA_DOLU;

	ramfs_düğüm_t *yeni =
		ramfs_düğüm_oluştur(&ramfs_süper, ad, tür);
	if (!yeni)
		return -12;

	yeni->ana = ana;
	ana->çocuklar[ana->çocuk_sayısı++] = yeni;
	return VFS_HATA_YOK;
}

static int32_t ramfs_sil(
	vfs_düğüm_t *düğüm, const char *ad)
{
	ramfs_düğüm_t *ana = vfs_den_ramfs(düğüm);
	for (uint8_t i = 0; i < ana->çocuk_sayısı; i++) {
		if (dizge_karşılaştır(
			ana->çocuklar[i]->vfs.ad, ad) == 0) {
			if (ana->çocuklar[i]->veri)
				yigin_serbest(
					ana->çocuklar[i]->veri);
			ana->çocuklar[i]->kullanımda = YANLIŞ;
			for (uint8_t j = i;
			     j < ana->çocuk_sayısı - 1; j++)
				ana->çocuklar[j] =
					ana->çocuklar[j + 1];
			ana->çocuk_sayısı--;
			return VFS_HATA_YOK;
		}
	}
	return VFS_HATA_BULUNAMADI;
}

ramfs_düğüm_t *ramfs_düğüm_oluştur(
	ramfs_süper_t *süper,
	const char *ad,
	uint32_t tür)
{
	for (uint32_t i = 0; i < RAMFS_AZAMI_DÜĞÜM; i++) {
		if (!süper->düğümler[i].kullanımda) {
			ramfs_düğüm_t *d = &süper->düğümler[i];
			bellek_doldur(d, 0, sizeof(ramfs_düğüm_t));
			dizge_kopyala(d->vfs.ad, ad, VFS_AD_AZAMI);
			d->vfs.tür       = tür;
			d->vfs.düğüm_no  = i;
			d->vfs.işlemler  = &ramfs_işlemler;
			d->vfs.boyut     = 0;
			d->kullanımda    = DOĞRU;
			d->çocuk_sayısı  = 0;
			d->veri          = BOŞ;
			d->veri_boyutu   = 0;
			süper->kullanılan_düğüm++;
			return d;
		}
	}
	return BOŞ;
}

int32_t ramfs_dosya_yaz(
	ramfs_düğüm_t *düğüm,
	const void *veri,
	boyut_t boyut)
{
	return (int32_t)ramfs_yaz(
		&düğüm->vfs, 0, boyut, veri);
}

vfs_düğüm_t *ramfs_başlat(void)
{
	bellek_doldur(&ramfs_süper, 0, sizeof(ramfs_süper_t));

	ramfs_işlemler.aç        = ramfs_aç;
	ramfs_işlemler.kapat     = ramfs_kapat;
	ramfs_işlemler.oku       = ramfs_oku;
	ramfs_işlemler.yaz       = ramfs_yaz;
	ramfs_işlemler.kes       = ramfs_kes;
	ramfs_işlemler.stat      = ramfs_stat;
	ramfs_işlemler.dizin_oku = ramfs_dizin_oku;
	ramfs_işlemler.bul       = ramfs_bul;
	ramfs_işlemler.oluştur   = ramfs_oluştur;
	ramfs_işlemler.sil       = ramfs_sil;
	ramfs_işlemler.taşı      = BOŞ;

	ramfs_düğüm_t *kök =
		ramfs_düğüm_oluştur(
			&ramfs_süper, "/", VFS_TÜR_DİZİN);
	if (!kök)
		return BOŞ;

	ramfs_süper.kök = &kök->vfs;

	ramfs_oluştur(&kök->vfs, "aygit", VFS_TÜR_DİZİN);
	ramfs_oluştur(&kök->vfs, "tmp",   VFS_TÜR_DİZİN);
	ramfs_oluştur(&kök->vfs, "bin",   VFS_TÜR_DİZİN);
	ramfs_oluştur(&kök->vfs, "etc",   VFS_TÜR_DİZİN);

	return &kök->vfs;
}
