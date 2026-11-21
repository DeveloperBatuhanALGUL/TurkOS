#include "islemci.h"

static void islemci_kimlik_sor(unsigned int islev, unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    __asm__ volatile ("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(islev), "c"(0));
}

int islemci_pae_destekli(void)
{
    unsigned int a, b, c, d;
    islemci_kimlik_sor(1, &a, &b, &c, &d);
    return (d >> 6) & 1;
}

int islemci_nx_destekli(void)
{
    unsigned int a, b, c, d;
    islemci_kimlik_sor(0x80000000, &a, &b, &c, &d);
    if (a < 0x80000001)
        return 0;
    islemci_kimlik_sor(0x80000001, &a, &b, &c, &d);
    return (d >> 20) & 1;
}
