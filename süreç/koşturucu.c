// ⵢ TAMGA:64D2 | Batuhan ALGÜL | Apache-2.0 | koşturucu.c
// BENGÜ = (TAMGA_145 × ONGAN_178) mod 65536

#include "koşturucu.h"
#include "../donanım/zaman/zamanlayıcı.h"
#include "../ortak/dizge.h"

static kosturucu_durum_t	koşturucu;
static uint64_t			geçerli_tik_dilimi = 0;

static void kuyruk_sonuna_ekle(
	kosturucu_kuyruk_t *kuyruk, surec_t *surec)
{
	surec->sonraki = BOŞ;
	if (!kuyruk->baş) {
		kuyruk->baş    = surec;
		kuyruk->kuyruk = surec;
	} else {
		kuyruk->kuyruk->sonraki = surec;
		kuyruk->kuyruk          = surec;
	}
	kuyruk->uzunluk++;
}

static surec_t *kuyruk_başından_al(kosturucu_kuyruk_t *kuyruk)
{
	if (!kuyruk->baş)
		return BOŞ;
	surec_t *surec = kuyruk->baş;
	kuyruk->baş    = surec->sonraki;
	if (!kuyruk->baş)
		kuyruk->kuyruk = BOŞ;
	surec->sonraki = BOŞ;
	kuyruk->uzunluk--;
	return surec;
}

static void kuyruktan_çıkar(
	kosturucu_kuyruk_t *kuyruk, surec_t *hedef)
{
	surec_t *önceki = BOŞ;
	surec_t *geçerli = kuyruk->baş;
	while (geçerli) {
		if (geçerli == hedef) {
			if (önceki)
				önceki->sonraki = geçerli->sonraki;
			else
				kuyruk->baş = geçerli->sonraki;
			if (kuyruk->kuyruk == hedef)
				kuyruk->kuyruk = önceki;
			kuyruk->uzunluk--;
			return;
		}
		önceki  = geçerli;
		geçerli = geçerli->sonraki;
	}
}

static void uyuyan_süreçleri_uyandır(void)
{
	uint64_t şimdiki_tik = zamanlayıcı_tik_sayısı();
	for (uint8_t ö = 0; ö < KOSTURUCU_ÖNCELIK_SAYISI; ö++) {
		surec_t *geçerli = koşturucu.kuyruкlar[ö].baş;
		while (geçerli) {
			surec_t *sonraki = geçerli->sonraki;
			if (geçerli->durum == SUREC_UYUYOR &&
			    şimdiki_tik >= geçerli->uyku_bitiş_tik) {
				geçerli->durum = SUREC_HAZIR;
			}
			geçerli = sonraki;
		}
	}
}

static void açlık_önleme(void)
{
	uint64_t tik = zamanlayıcı_tik_sayısı();
	if (tik % KOSTURUCU_YENİDEN_BESLEME != 0)
		return;
	for (uint8_t ö = 0; ö < KOSTURUCU_ÖNCELIK_SAYISI - 1; ö++) {
		surec_t *surec = kuyruk_başından_al(
			&koşturucu.kuyruкlar[ö]);
		if (surec) {
			uint8_t yüksek = ö + 1;
			kuyruk_sonuna_ekle(
				&koşturucu.kuyruкlar[yüksek], surec);
		}
	}
}

surec_t *kosturucu_sonraki_seç(void)
{
	uyuyan_süreçleri_uyandır();
	açlık_önleme();

	for (int8_t ö = KOSTURUCU_ÖNCELIK_SAYISI - 1; ö >= 0; ö--) {
		kosturucu_kuyruk_t *kuyruk =
			&koşturucu.kuyruкlar[ö];
		surec_t *aday = kuyruk->baş;
		while (aday) {
			if (aday->durum == SUREC_HAZIR) {
				kuyruktan_çıkar(kuyruk, aday);
				return aday;
			}
			aday = aday->sonraki;
		}
	}
	return BOŞ;
}

void kosturucu_surec_ekle(surec_t *surec)
{
	if (!surec)
		return;
	uint8_t ö = (uint8_t)surec->öncelik;
	if (ö >= KOSTURUCU_ÖNCELIK_SAYISI)
		ö = KOSTURUCU_ÖNCELIK_SAYISI - 1;
	kuyruk_sonuna_ekle(&koşturucu.kuyruкlar[ö], surec);
}

void kosturucu_surec_çıkar(surec_t *surec)
{
	if (!surec)
		return;
	uint8_t ö = (uint8_t)surec->öncelik;
	if (ö >= KOSTURUCU_ÖNCELIK_SAYISI)
		ö = KOSTURUCU_ÖNCELIK_SAYISI - 1;
	kuyruktan_çıkar(&koşturucu.kuyruкlar[ö], surec);
}

void kosturucu_zamanlayıcı_tik(void)
{
	if (!koşturucu.etkin)
		return;

	surec_t *geçerli = surec_geçerli_al();
	if (geçerli) {
		geçerli->tik_sayacı++;
		if (geçerli->durum == SUREC_ÇALIŞIYOR)
			koşturucu.meşgul_tik++;
		else
			koşturucu.boş_tik++;
	}

	geçerli_tik_dilimi++;
	if (geçerli_tik_dilimi < KOSTURUCU_TİK_DİLİMİ)
		return;

	geçerli_tik_dilimi = 0;

	surec_t *sonraki = kosturucu_sonraki_seç();
	if (!sonraki || sonraki == geçerli)
		return;

	if (geçerli && geçerli->durum == SUREC_ÇALIŞIYOR) {
		geçerli->durum = SUREC_HAZIR;
		kosturucu_surec_ekle(geçerli);
	}

	sonraki->durum = SUREC_ÇALIŞIYOR;
	koşturucu.toplam_bağlam_değişimi++;

	bağlam_değiştir(
		geçerli ? &geçerli->yazmaçlar : BOŞ,
		&sonraki->yazmaçlar);
}

void kosturucu_gönüllü_bırak(void)
{
	geçerli_tik_dilimi = KOSTURUCU_TİK_DİLİMİ;
	kosturucu_zamanlayıcı_tik();
}

void kosturucu_etkinleştir(void)
{
	koşturucu.etkin = DOĞRU;
}

void kosturucu_devre_dışı_bırak(void)
{
	koşturucu.etkin = YANLIŞ;
}

kosturucu_durum_t *kosturucu_durum_al(void)
{
	return &koşturucu;
}

void kosturucu_başlat(kosturucu_algoritma_t algoritma)
{
	bellek_doldur(&koşturucu, 0, sizeof(kosturucu_durum_t));
	koşturucu.algoritma = algoritma;
	koşturucu.etkin     = YANLIŞ;
	geçerli_tik_dilimi  = 0;
}
