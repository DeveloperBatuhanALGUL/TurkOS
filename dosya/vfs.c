// ⵢ TAMGA:1FB8 | Batuhan ALGÜL | Apache-2.0 | vfs.c
// BENGÜ = (TAMGA_145 × ONGAN_56) mod 65536

#include "vfs.h"
#include "../bellek/yığın_yönetici.h"
#include "../ortak/dizge.h"

static vfs_bağlama_t	*bağlama_listesi = BOŞ;
static vfs_düğüm_t	*kök_düğüm       = BOŞ;

static vfs_bağlama_t *bağlama_noktası_bul(const char *yol)
{
	vfs_bağlama_t *en_uzun   = BOŞ;
	boyut_t        en_boy     = 0;
	vfs_bağlama_t *geçerli   = bağlama_listesi;

	while (geçerli) {
		boyut_t boy =
			dizge_uzunluk(geçerli->bağlama_noktası);
		if (boy > en_boy &&
		    dizge_karşılaştır(
			geçerli->bağlama_noktası, yol) == 0) {
			en_uzun = geçerli;
			en_boy  = boy;
		}
		geçerli = geçerli->sonraki;
	}
	return en_uzun;
}

static void yol_parçala(
	const char *yol, char *parça, boyut_t azami,
	const char **geri_kalan)
{
	while (*yol == '/')
		yol++;

	boyut_t i = 0;
	while (*yol && *yol != '/' && i + 1 < azami) {
		parça[i++] = *yol++;
	}
	parça[i] = '\0';

	while (*yol == '/')
		yol++;

	*geri_kalan = yol;
}

static vfs_düğüm_t *düğüm_gez(
	vfs_düğüm_t *başlangıç, const char *yol)
{
	if (!yol || !*yol)
		return başlangıç;

	vfs_düğüm_t *geçerli = başlangıç;
	char	     parça[VFS_AD_AZAMI];
	const char  *kalan;

	while (*yol) {
		yol_parçala(yol, parça, VFS_AD_AZAMI, &kalan);
		if (!*parça)
			break;

		if (!geçerli->işlemler ||
		    !geçerli->işlemler->bul)
			return BOŞ;

		vfs_düğüm_t *sonraki =
			geçerli->işlemler->bul(geçerli, parça);
		if (!sonraki)
			return BOŞ;

		if (sonraki->bağlama)
			geçerli = sonraki->bağlama->kök_düğüm;
		else
			geçerli = sonraki;

		yol = kalan;
	}
	return geçerli;
}

vfs_düğüm_t *vfs_yol_çöz(const char *yol)
{
	if (!yol)
		return BOŞ;

	if (*yol != '/')
		return BOŞ;

	if (dizge_uzunluk(yol) == 1)
		return kök_düğüm;

	vfs_bağlama_t *bağlama = BOŞ;
	vfs_düğüm_t   *başlangıç = kök_düğüm;
	vfs_bağlama_t *geçerli = bağlama_listesi;

	while (geçerli) {
		boyut_t boy =
			dizge_uzunluk(geçerli->bağlama_noktası);
		if (dizge_karşılaştır(
			geçerli->bağlama_noktası, "/") != 0 &&
		    boy <= dizge_uzunluk(yol)) {
			char önek[VFS_YOL_AZAMI];
			dizge_kopyala(önek, yol, boy + 1);
			if (dizge_karşılaştır(
				önek,
				geçerli->bağlama_noktası) == 0) {
				bağlama = geçerli;
				başlangıç = geçerli->kök_düğüm;
				yol += boy;
				break;
			}
		}
		geçerli = geçerli->sonraki;
	}

	(void)bağlama;
	return düğüm_gez(başlangıç, yol);
}

int32_t vfs_bağla(
	const char *nokta,
	vfs_düğüm_t *kök,
	const char *fs_adı)
{
	if (!nokta || !kök)
		return VFS_HATA_GEÇERSİZ;

	vfs_bağlama_t *yeni =
		yigin_ayır(sizeof(vfs_bağlama_t));
	if (!yeni)
		return -12;

	dizge_kopyala(yeni->bağlama_noktası,
		nokta, VFS_YOL_AZAMI);
	yeni->kök_düğüm        = kök;
	yeni->bağlama_düğümü   = BOŞ;
	yeni->dosya_sistemi_adı = fs_adı;
	yeni->sonraki          = bağlama_listesi;
	bağlama_listesi        = yeni;

	if (dizge_karşılaştır(nokta, "/") == 0)
		kök_düğüm = kök;

	return VFS_HATA_YOK;
}

int32_t vfs_ayır(const char *nokta)
{
	if (!nokta)
		return VFS_HATA_GEÇERSİZ;

	vfs_bağlama_t *önceki = BOŞ;
	vfs_bağlama_t *geçerli = bağlama_listesi;

	while (geçerli) {
		if (dizge_karşılaştır(
			geçerli->bağlama_noktası, nokta) == 0) {
			if (önceki)
				önceki->sonraki = geçerli->sonraki;
			else
				bağlama_listesi = geçerli->sonraki;
			yigin_serbest(geçerli);
			return VFS_HATA_YOK;
		}
		önceki  = geçerli;
		geçerli = geçerli->sonraki;
	}
	return VFS_HATA_BULUNAMADI;
}

int64_t vfs_oku(
	const char *yol, uint64_t konum,
	boyut_t boyut, void *tampon)
{
	vfs_düğüm_t *düğüm = vfs_yol_çöz(yol);
	if (!düğüm)
		return VFS_HATA_BULUNAMADI;
	if (!düğüm->işlemler || !düğüm->işlemler->oku)
		return VFS_HATA_GEÇERSİZ;
	return düğüm->işlemler->oku(düğüm, konum, boyut, tampon);
}

int64_t vfs_yaz(
	const char *yol, uint64_t konum,
	boyut_t boyut, const void *tampon)
{
	vfs_düğüm_t *düğüm = vfs_yol_çöz(yol);
	if (!düğüm)
		return VFS_HATA_BULUNAMADI;
	if (!düğüm->işlemler || !düğüm->işlemler->yaz)
		return VFS_HATA_GEÇERSİZ;
	return düğüm->işlemler->yaz(
		düğüm, konum, boyut, tampon);
}

int32_t vfs_oluştur(const char *yol, uint32_t tür)
{
	if (!yol || dizge_uzunluk(yol) < 2)
		return VFS_HATA_GEÇERSİZ;

	char üst_yol[VFS_YOL_AZAMI];
	dizge_kopyala(üst_yol, yol, VFS_YOL_AZAMI);

	const char *son_eğik = BOŞ;
	for (boyut_t i = 0; üst_yol[i]; i++)
		if (üst_yol[i] == '/')
			son_eğik = &üst_yol[i];

	if (!son_eğik)
		return VFS_HATA_GEÇERSİZ;

	const char *dosya_adı = son_eğik + 1;
	if (son_eğik == üst_yol)
		üst_yol[1] = '\0';
	else
		üst_yol[son_eğik - üst_yol] = '\0';

	vfs_düğüm_t *üst = vfs_yol_çöz(üst_yol);
	if (!üst)
		return VFS_HATA_BULUNAMADI;
	if (!üst->işlemler || !üst->işlemler->oluştur)
		return VFS_HATA_GEÇERSİZ;

	return üst->işlemler->oluştur(üst, dosya_adı, tür);
}

int32_t vfs_sil(const char *yol)
{
	if (!yol || dizge_uzunluk(yol) < 2)
		return VFS_HATA_GEÇERSİZ;

	char üst_yol[VFS_YOL_AZAMI];
	dizge_kopyala(üst_yol, yol, VFS_YOL_AZAMI);

	const char *son_eğik = BOŞ;
	for (boyut_t i = 0; üst_yol[i]; i++)
		if (üst_yol[i] == '/')
			son_eğik = &üst_yol[i];

	if (!son_eğik)
		return VFS_HATA_GEÇERSİZ;

	const char *dosya_adı = son_eğik + 1;
	if (son_eğik == üst_yol)
		üst_yol[1] = '\0';
	else
		üst_yol[son_eğik - üst_yol] = '\0';

	vfs_düğüm_t *üst = vfs_yol_çöz(üst_yol);
	if (!üst)
		return VFS_HATA_BULUNAMADI;
	if (!üst->işlemler || !üst->işlemler->sil)
		return VFS_HATA_GEÇERSİZ;

	return üst->işlemler->sil(üst, dosya_adı);
}

int32_t vfs_stat(
	const char *yol, uint64_t *boyut, uint32_t *tür)
{
	vfs_düğüm_t *düğüm = vfs_yol_çöz(yol);
	if (!düğüm)
		return VFS_HATA_BULUNAMADI;
	if (!düğüm->işlemler || !düğüm->işlemler->stat)
		return VFS_HATA_GEÇERSİZ;
	return düğüm->işlemler->stat(düğüm, boyut, tür);
}

vfs_düğüm_t *vfs_kök_al(void)
{
	return kök_düğüm;
}

void vfs_başlat(void)
{
	bağlama_listesi = BOŞ;
	kök_düğüm       = BOŞ;
}
