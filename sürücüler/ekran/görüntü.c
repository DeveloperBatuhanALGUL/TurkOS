// ⵢ TAMGA:4FDD | Batuhan ALGÜL | Apache-2.0 | görüntü.c
// BENGÜ = (TAMGA_145 × ONGAN_141) mod 65536

#include "görüntü.h"
#include "../../ortak/dizge.h"

static uint16_t	*gea_tampon = (uint16_t *)GEA_TAMPON_ADRES;
static uint8_t	geçerli_sütun = 0;
static uint8_t	geçerli_satır = 0;
static uint8_t	geçerli_renk  = GEA_VARSAYILAN_RENK;

static uint16_t gea_hücre_oluştur(char karakter, uint8_t renk)
{
	return (uint16_t)karakter | ((uint16_t)renk << 8);
}

static void gea_kaydır(void)
{
	for (uint8_t s = 1; s < GEA_SATIR_SAYISI; s++) {
		for (uint8_t c = 0; c < GEA_SÜTUN_SAYISI; c++) {
			gea_tampon[(s - 1) * GEA_SÜTUN_SAYISI + c] =
				gea_tampon[s * GEA_SÜTUN_SAYISI + c];
		}
	}
	for (uint8_t c = 0; c < GEA_SÜTUN_SAYISI; c++) {
		gea_tampon[(GEA_SATIR_SAYISI - 1) * GEA_SÜTUN_SAYISI + c] =
			gea_hücre_oluştur(' ', geçerli_renk);
	}
}

void görüntü_başlat(void)
{
	geçerli_renk  = GEA_VARSAYILAN_RENK;
	geçerli_sütun = 0;
	geçerli_satır = 0;
	görüntü_temizle();
}

void görüntü_temizle(void)
{
	for (uint16_t i = 0; i < GEA_HÜCRE_SAYISI; i++)
		gea_tampon[i] = gea_hücre_oluştur(' ', geçerli_renk);
	geçerli_sütun = 0;
	geçerli_satır = 0;
}

void görüntü_renk_ayarla(uint8_t renk)
{
	geçerli_renk = renk;
}

void görüntü_konum_ayarla(uint8_t sütun, uint8_t satır)
{
	geçerli_sütun = sütun;
	geçerli_satır = satır;
}

void görüntü_yeni_satır(void)
{
	geçerli_sütun = 0;
	geçerli_satır++;
	if (geçerli_satır >= GEA_SATIR_SAYISI) {
		gea_kaydır();
		geçerli_satır = GEA_SATIR_SAYISI - 1;
	}
}

void görüntü_karakter_yaz(char karakter)
{
	if (karakter == '\n') {
		görüntü_yeni_satır();
		return;
	}
	if (karakter == '\r') {
		geçerli_sütun = 0;
		return;
	}
	gea_tampon[geçerli_satır * GEA_SÜTUN_SAYISI + geçerli_sütun] =
		gea_hücre_oluştur(karakter, geçerli_renk);
	geçerli_sütun++;
	if (geçerli_sütun >= GEA_SÜTUN_SAYISI)
		görüntü_yeni_satır();
}

void görüntü_dizge_yaz(const char *dizge)
{
	while (*dizge)
		görüntü_karakter_yaz(*dizge++);
}

void görüntü_sayı_yaz(uint64_t değer)
{
	char tampon[21];
	tam_sayı_dizgeye((int64_t)değer, tampon, sizeof(tampon));
	görüntü_dizge_yaz(tampon);
}

void görüntü_onaltılı_yaz(uint64_t değer)
{
	char tampon[17];
	onaltılı_dizgeye(değer, tampon, sizeof(tampon));
	görüntü_dizge_yaz(tampon);
}
