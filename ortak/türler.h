// ⵢ TAMGA:4001 | Batuhan ALGÜL | Apache-2.0 | türler.h
// BENGÜ = (TAMGA_145 × ONGAN_113) mod 65536

#ifndef ORTAK_TURLER_H
#define ORTAK_TURLER_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef uint8_t bayt;
typedef uint16_t kelime;
typedef uint32_t çift_kelime;
typedef uint64_t uzun_kelime;

typedef uint8_t doğruluk;
typedef uint32_t bayrak;
typedef uint64_t boyut;
typedef uint64_t sanal_adres;
typedef uint64_t gerçek_adres;

#define DOĞRU 1
#define YANLIŞ 0
#define BOŞ ((void*)0)

#endif
