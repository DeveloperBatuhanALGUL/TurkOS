// ⵢ TAMGA:3AE8 | Batuhan ALGÜL | Apache-2.0 | bellek_çağrı.c
// BENGÜ = (TAMGA_145 × ONGAN_104) mod 65536

#include "dizge_çağrı.h"
#include "../süreç/süreç.h"
#include "../bellek/sanal_bellek.h"
#include "../bellek/çerçeve_yönetici.h"
#include "../bellek/sayfalama.h"

#define BÇAĞRI_EŞLE_ANONIM     (1 << 0)
#define BÇAĞRI_EŞLE_ÖZEL       (1 << 1)
#define BÇAĞRI_EŞLE_PAYLAŞIM   (1 << 2)
#define BÇAĞRI_EŞLE_SABİT      (1 << 3)

#define BÇAĞRI_KORUMA_OKU      (1 << 0)
#define BÇAĞRI_KORUMA_YAZ      (1 << 1)
#define BÇAĞRI_KORUMA_ÇALİŞ    (1 << 2)
#define BÇAĞRI_KORUMA_YOK      0

static uint64_t koruma_bayrak_çevir(uint64_t koruma)
{
	uint64_t bayrak = SAYFA_VAR_BİT | SAYFA_KULLANICI;
	if (koruma & BÇAĞRI_KORUMA_YAZ)
		bayrak |= SAYFA_YAZILABİLİR;
	if (!(koruma & BÇAĞRI_KORUMA_ÇALİŞ))
		bayrak |= SAYFA_YÜRÜTÜLEMEz;
	return bayrak;
}

static uintptr_t adres_uzayı_bul(
	sanal_adres_uzayı_t *uzay, boyut_t boyut)
{
	uintptr_t aday = uzay->kullanıcı_sonu;
	uintptr_t hizalı_boyut =
		(boyut + SAYFA_BOYUTU - 1) & ~(SAYFA_BOYUTU - 1);
	if (aday + hizalı_boyut > SANAL_KULLANICI_SONU)
		return 0;
	uzay->kullanıcı_sonu = aday + hizalı_boyut;
	return aday;
}

static int64_t dç_bellek_ayır(const dç_çerçeve_t *çerçeve)
{
	uintptr_t	istenen_adres = (uintptr_t)çerçeve->arg0;
	boyut_t		boyut         = (boyut_t)çerçeve->arg1;
	uint64_t	koruma        = çerçeve->arg2;
	uint64_t	eşleme        = çerçeve->arg3;

	if (!boyut)
		return -12;

	surec_t *geçerli = surec_geçerli_al();
	if (!geçerli || !geçerli->adres_uzayı)
		return -12;

	boyut_t hizalı_boyut =
		(boyut + SAYFA_BOYUTU - 1) & ~(SAYFA_BOYUTU - 1);

	uintptr_t hedef_adres = istenen_adres;
	if (!hedef_adres || !(eşleme & BÇAĞRI_EŞLE_SABİT))
		hedef_adres = adres_uzayı_bul(
			geçerli->adres_uzayı, hizalı_boyut);

	if (!hedef_adres)
		return -12;

	uint64_t bayrak = koruma_bayrak_çevir(koruma);
	boyut_t sayfa_sayısı = hizalı_boyut / SAYFA_BOYUTU;

	for (boyut_t i = 0; i < sayfa_sayısı; i++) {
		uintptr_t çerçeve = çerçeve_ayır();
		if (!çerçeve)
			return -12;
		bellek_doldur((void *)çerçeve, 0, SAYFA_BOYUTU);
		sanal_eşle(geçerli->adres_uzayı,
			hedef_adres + i * SAYFA_BOYUTU,
			çerçeve, SAYFA_BOYUTU, bayrak);
	}

	return (int64_t)hedef_adres;
}

static int64_t dç_bellek_serbest(const dç_çerçeve_t *çerçeve)
{
	uintptr_t adres = (uintptr_t)çerçeve->arg0;
	boyut_t   boyut = (boyut_t)çerçeve->arg1;

	if (!adres || !boyut)
		return -22;

	surec_t *geçerli = surec_geçerli_al();
	if (!geçerli || !geçerli->adres_uzayı)
		return -22;

	boyut_t hizalı_boyut =
		(boyut + SAYFA_BOYUTU - 1) & ~(SAYFA_BOYUTU - 1);
	boyut_t sayfa_sayısı = hizalı_boyut / SAYFA_BOYUTU;

	for (boyut_t i = 0; i < sayfa_sayısı; i++) {
		uintptr_t sa = adres + i * SAYFA_BOYUTU;
		uintptr_t ga = sanal_gerçek_adres_bul(
				geçerli->adres_uzayı, sa);
		if (ga)
			çerçeve_serbest(ga);
		sayfa_eşlemeyi_kaldır(sa);
	}

	sanal_eşlemeyi_kaldır(
		geçerli->adres_uzayı, adres, hizalı_boyut);
	return 0;
}

static int64_t dç_yeniden_boyutla(const dç_çerçeve_t *çerçeve)
{
	uintptr_t eski_adres  = (uintptr_t)çerçeve->arg0;
	boyut_t   eski_boyut  = (boyut_t)çerçeve->arg1;
	boyut_t   yeni_boyut  = (boyut_t)çerçeve->arg2;

	if (!eski_adres || !yeni_boyut)
		return -22;

	surec_t *geçerli = surec_geçerli_al();
	if (!geçerli || !geçerli->adres_uzayı)
		return -22;

	dç_çerçeve_t yeni_çerçeve = {
		.numara = DÇ_BELLEKAYİR,
		.arg0   = 0,
		.arg1   = yeni_boyut,
		.arg2   = BÇAĞRI_KORUMA_OKU | BÇAĞRI_KORUMA_YAZ,
		.arg3   = BÇAĞRI_EŞLE_ANONIM | BÇAĞRI_EŞLE_ÖZEL
	};
	int64_t yeni_adres = dç_bellek_ayır(¥i_çerçeve);
	if (yeni_adres < 0)
		return yeni_adres;

	boyut_t kopyala_boyut =
		eski_boyut < yeni_boyut ? eski_boyut : yeni_boyut;
	bellek_kopyala((void *)yeni_adres,
		(void *)eski_adres, kopyala_boyut);

	dç_çerçeve_t serbest_çerçeve = {
		.arg0 = eski_adres,
		.arg1 = eski_boyut
	};
	dç_bellek_serbest(&serbest_çerçeve);

	return yeni_adres;
}

static int64_t dç_bölüt_ayır(const dç_çerçeve_t *çerçeve)
{
	(void)çerçeve;
	uintptr_t çerçeve_adres = çerçeve_ayır();
	if (!çerçeve_adres)
		return -12;
	bellek_doldur((void *)çerçeve_adres, 0, SAYFA_BOYUTU);
	return (int64_t)çerçeve_adres;
}

static int64_t dç_bölüt_serbest(const dç_çerçeve_t *çerçeve)
{
	çerçeve_serbest((uintptr_t)çerçeve->arg0);
	return 0;
}

void bellek_çağrılarını_kaydet(void)
{
	dizge_çağrı_işleyici_kaydet(DÇ_BELLEKAYİR,      dç_bellek_ayır);
	dizge_çağrı_işleyici_kaydet(DÇ_BELLEKSERBESTLE,  dç_bellek_serbest);
	dizge_çağrı_işleyici_kaydet(DÇ_YENİDENBOYUTLA,   dç_yeniden_boyutla);
	dizge_çağrı_işleyici_kaydet(DÇ_BÖLÜT_AYİR,      dç_bölüt_ayır);
	dizge_çağrı_işleyici_kaydet(DÇ_BÖLÜT_SERBESTİ,  dç_bölüt_serbest);
}
