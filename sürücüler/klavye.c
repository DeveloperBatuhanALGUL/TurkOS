#include "klavye.h"
#include "görüntü.h"
#include "../ortak/bağlantı.h"
#include "../donanım/kkt/kesme_kapisi.h"

static const char taramakod_tablosu[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static void klavye_isleyici(void)
{
    unsigned char taramakod = port_byte_oku(0x60);

    if (taramakod < 128)
    {
        char karakter = taramakod_tablosu[taramakod];
        if (karakter)
        {
            char dizi[2] = { karakter, 0 };
            goruntu_yaz(dizi);
        }
    }
}

void klavye_baslat(void)
{
    kesme_yoneticisi_kaydet(33, klavye_isleyici);
}
