// ⵢ TAMGA:43F8 | Batuhan ALGÜL | Apache-2.0 | dosya_çağrı.c
// BENGÜ = (TAMGA_145 × ONGAN_120) mod 65536

#include "dizge_çağrı.h"
#include "../süreç/süreç.h"
#include "../sürücüler/ekran/görüntü.h"
#include "../ortak/dizge.h"

#define DT_AZAMI_YOL        256
#define DT_GEÇERSIZ         -1

#define DT_BAYRAK_OKU       (1 << 0)
#define DT_BAYRAK_YAZ       (1 << 1)
#define DT_BAYRAK_EKLE      (1 << 2)
#define DT_BAYRAK_OLUŞTUR   (1 << 3)
#define DT_BAYRAK_KES       (1 << 4)

#define DT_TÜR_NORMAL       0
#define DT_TÜR_AYGIT        1
#define DT_TÜR_BORU        2

typedef struct {
	int32_t		tanımlayıcı;
	uint32_t	bayraklar;
	uint32_t	tür;
	uint64_t	konum;
	uint64_t	boyut;
	char		yol[DT_AZAMI_YOL];
	uint8_t		açık;
} dosya_tanımlayıcı_t;

#define STANDART_GİRİŞ      0
#define STANDART_ÇIKIŞ      1
#define STANDART_HATA       2

static dosya_tanımlayıcı_t standart_aygıtlar[3] = {
	{ STANDART_GİRİŞ,  DT_BAYRAK_OKU, DT_TÜR_AYGIT,
	  0, 0, "/aygit/giris",  DOĞRU },
	{ STANDART_ÇIKIŞ,  DT_BAYRAK_YAZ, DT_TÜR_AYGIT,
	  0, 0, "/aygit/cikis",  DOĞRU },
	{ STANDART_HATA,   DT_BAYRAK_YAZ, DT_TÜR_AYGIT,
	  0, 0, "/aygit/hata",   DOĞRU },
};

static dosya_tanımlayıcı_t *tanımlayıcı_bul(
	surec_t *surec, int32_t dt)
{
	if (dt < 0 || dt >= SUREC_AZAMI_DOSYA)
		return BOŞ;
	if (dt < 3)
		return &standart_aygıtlar[dt];
	return (dosya_tanımlayıcı_t *)surec->dosyalar[dt];
}

static int32_t boş_tanımlayıcı_bul(surec_t *surec)
{
	for (int32_t i = 3; i < SUREC_AZAMI_DOSYA; i++) {
		if (!surec->dosyalar[i])
			return i;
	}
	return DT_GEÇERSIZ;
}

static int64_t dç_yazi(const dç_çerçeve_t *çerçeve)
{
	int32_t		dt    = (int32_t)çerçeve->arg0;
	const char	*tampon = (const char *)çerçeve->arg1;
	boyut_t		boyut = (boyut_t)çerçeve->arg2;

	if (!tampon || !boyut)
		return -22;

	surec_t *geçerli = surec_geçerli_al();
	dosya_tanımlayıcı_t *dtt =
		tanımlayıcı_bul(geçerli, dt);

	if (!dtt || !dtt->açık)
		return -9;
	if (!(dtt->bayraklar & DT_BAYRAK_YAZ))
		return -13;

	if (dt == STANDART_ÇIKIŞ || dt == STANDART_HATA) {
		for (boyut_t i = 0; i < boyut; i++)
			görüntü_karakter_yaz(tampon[i]);
		return (int64_t)boyut;
	}

	return -38;
}

static int64_t dç_oku(const dç_çerçeve_t *çerçeve)
{
	int32_t		dt    = (int32_t)çerçeve->arg0;
	char		*tampon = (char *)çerçeve->arg1;
	boyut_t		boyut = (boyut_t)çerçeve->arg2;

	if (!tampon || !boyut)
		return -22;

	surec_t *geçerli = surec_geçerli_al();
	dosya_tanımlayıcı_t *dtt =
		tanımlayıcı_bul(geçerli, dt);

	if (!dtt || !dtt->açık)
		return -9;
	if (!(dtt->bayraklar & DT_BAYRAK_OKU))
		return -13;

	return -38;
}

static int64_t dç_aç(const dç_çerçeve_t *çerçeve)
{
	const char	*yol    = (const char *)çerçeve->arg0;
	uint32_t	bayrak  = (uint32_t)çerçeve->arg1;

	if (!yol)
		return -22;

	surec_t *geçerli = surec_geçerli_al();
	if (!geçerli)
		return -11;

	int32_t dt = boş_tanımlayıcı_bul(geçerli);
	if (dt == DT_GEÇERSIZ)
		return -24;

	dosya_tanımlayıcı_t *yeni =
		yigin_ayır(sizeof(dosya_tanımlayıcı_t));
	if (!yeni)
		return -12;

	yeni->tanımlayıcı = dt;
	yeni->bayraklar   = bayrak;
	yeni->tür         = DT_TÜR_NORMAL;
	yeni->konum       = 0;
	yeni->boyut       = 0;
	yeni->açık        = DOĞRU;
	dizge_kopyala(yeni->yol, yol, DT_AZAMI_YOL);

	geçerli->dosyalar[dt] = yeni;
	return (int64_t)dt;
}

static int64_t dç_kapat(const dç_çerçeve_t *çerçeve)
{
	int32_t dt = (int32_t)çerçeve->arg0;

	if (dt < 3)
		return -9;

	surec_t *geçerli = surec_geçerli_al();
	if (!geçerli)
		return -11;

	dosya_tanımlayıcı_t *dtt =
		(dosya_tanımlayıcı_t *)geçerli->dosyalar[dt];
	if (!dtt || !dtt->açık)
		return -9;

	dtt->açık = YANLIŞ;
	yigin_serbest(dtt);
	geçerli->dosyalar[dt] = BOŞ;
	return 0;
}

static int64_t dç_stat(const dç_çerçeve_t *çerçeve)
{
	int32_t dt = (int32_t)çerçeve->arg0;

	surec_t *geçerli = surec_geçerli_al();
	dosya_tanımlayıcı_t *dtt =
		tanımlayıcı_bul(geçerli, dt);

	if (!dtt || !dtt->açık)
		return -9;

	return (int64_t)dtt->boyut;
}

void dosya_çağrılarını_kaydet(void)
{
	dizge_çağrı_işleyici_kaydet(DÇ_YAZI,  dç_yazi);
	dizge_çağrı_işleyici_kaydet(DÇ_OKU,   dç_oku);
	dizge_çağrı_işleyici_kaydet(DÇ_AÇ,    dç_aç);
	dizge_çağrı_işleyici_kaydet(DÇ_KAPAT, dç_kapat);
	dizge_çağrı_işleyici_kaydet(DÇ_STAT,  dç_stat);
}
