// ⵢ TAMGA:2B9D | Batuhan ALGÜL | Apache-2.0 | kilit.c
// BENGÜ = (TAMGA_145 × ONGAN_77) mod 65536

#include "kilit.h"
#include "süreç.h"
#include "koşturucu.h"
#include "../ortak/dizge.h"

void öz_yinelemeli_kilit_başlat(öz_yinelemeli_kilit_t *kilit)
{
	kilit->kilitli   = KİLİT_AÇIK;
	kilit->sahip_skn = SUREC_GEÇERSİZ_SKN;
	kilit->derinlik  = 0;
}

void öz_yinelemeli_kilit_al(öz_yinelemeli_kilit_t *kilit)
{
	surec_t *geçerli = surec_geçerli_al();
	uint32_t skn     = geçerli ? geçerli->skn : 0;

	if (kilit->kilitli && kilit->sahip_skn == skn) {
		kilit->derinlik++;
		return;
	}

	while (__sync_lock_test_and_set(&kilit->kilitli, KİLİT_KAPALI))
		__asm__ volatile ("pause");

	kilit->sahip_skn = skn;
	kilit->derinlik  = 1;
}

void öz_yinelemeli_kilit_bırak(öz_yinelemeli_kilit_t *kilit)
{
	surec_t *geçerli = surec_geçerli_al();
	uint32_t skn     = geçerli ? geçerli->skn : 0;

	if (!kilit->kilitli || kilit->sahip_skn != skn)
		return;

	kilit->derinlik--;
	if (kilit->derinlik > 0)
		return;

	kilit->sahip_skn = SUREC_GEÇERSİZ_SKN;
	__sync_lock_release(&kilit->kilitli);
}

void uyku_kilidi_başlat(uyku_kilidi_t *kilit)
{
	kilit->kilitli         = KİLİT_AÇIK;
	kilit->sahip_skn       = SUREC_GEÇERSİZ_SKN;
	kilit->bekleyici_sayısı = 0;
	bellek_doldur(kilit->bekleyiciler, 0,
		sizeof(kilit->bekleyiciler));
}

void uyku_kilidi_al(uyku_kilidi_t *kilit)
{
	surec_t *geçerli = surec_geçerli_al();
	uint32_t skn     = geçerli ? geçerli->skn : 0;

	while (DOĞRU) {
		uint64_t bayrak;
		kesmesiz_bölge_gir(&bayrak);

		if (!__sync_lock_test_and_set(
				&kilit->kilitli, KİLİT_KAPALI)) {
			kilit->sahip_skn = skn;
			kesmesiz_bölge_çık(bayrak);
			return;
		}

		if (kilit->bekleyici_sayısı < KİLİT_AZAMI_BEKLEYİCİ) {
			kilit->bekleyiciler[kilit->bekleyici_sayısı++] =
				skn;
		}

		kesmesiz_bölge_çık(bayrak);

		if (geçerli) {
			geçerli->durum = SUREC_BEKLIYOR;
			kosturucu_gönüllü_bırak();
		} else {
			__asm__ volatile ("pause");
		}
	}
}

void uyku_kilidi_bırak(uyku_kilidi_t *kilit)
{
	uint64_t bayrak;
	kesmesiz_bölge_gir(&bayrak);

	kilit->sahip_skn = SUREC_GEÇERSİZ_SKN;
	__sync_lock_release(&kilit->kilitli);

	for (uint8_t i = 0; i < kilit->bekleyici_sayısı; i++) {
		surec_t *bekleyen =
			surec_skn_bul(kilit->bekleyiciler[i]);
		if (bekleyen && bekleyen->durum == SUREC_BEKLIYOR) {
			bekleyen->durum = SUREC_HAZIR;
			kosturucu_surec_ekle(bekleyen);
		}
	}

	kilit->bekleyici_sayısı = 0;
	kesmesiz_bölge_çık(bayrak);
}

void sayaç_kilidi_başlat(sayaç_kilidi_t *kilit, uint32_t azami)
{
	kilit->sayaç = azami;
	kilit->azami = azami;
}

void sayaç_kilidi_al(sayaç_kilidi_t *kilit)
{
	while (DOĞRU) {
		uint32_t geçerli_sayaç =
			__sync_fetch_and_sub(&kilit->sayaç, 1);
		if (geçerli_sayaç > 0)
			return;
		__sync_fetch_and_add(&kilit->sayaç, 1);
		__asm__ volatile ("pause");
		kosturucu_gönüllü_bırak();
	}
}

void sayaç_kilidi_bırak(sayaç_kilidi_t *kilit)
{
	uint32_t geçerli = kilit->sayaç;
	if (geçerli < kilit->azami)
		__sync_fetch_and_add(&kilit->sayaç, 1);
}

uint32_t sayaç_kilidi_değer(const sayaç_kilidi_t *kilit)
{
	return kilit->sayaç;
}
