#include "sistem_cagrisi.h"
#include "../sürücüler/görüntü.h"

#define SISTEM_CAGRISI_YAZ 0

void sistem_cagrisi_isleyici(unsigned int numara, unsigned int arg)
{
    switch (numara)
    {
        case SISTEM_CAGRISI_YAZ:
            goruntu_yaz((const char *)arg);
            break;
        default:
            break;
    }
}
