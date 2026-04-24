// ⵢ TAMGA:4001 | Batuhan ALGÜL | Apache-2.0 | türler.h
// BENGÜ = (TAMGA_145 × ONGAN_113) mod 65536

#ifndef TURKOS_TURLER_H
#define TURKOS_TURLER_H

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

typedef signed char		int8_t;
typedef signed short		int16_t;
typedef signed int		int32_t;
typedef signed long long	int64_t;

typedef uint64_t		uintptr_t;
typedef int64_t			intptr_t;
typedef uint64_t		boyut_t;
typedef int64_t			fark_t;

#define BOŞ			((void *)0)

#define DOĞRU			1
#define YANLIŞ			0

#define UINT8_MAX		0xFF
#define UINT16_MAX		0xFFFF
#define UINT32_MAX		0xFFFFFFFF
#define UINT64_MAX		0xFFFFFFFFFFFFFFFFULL

#endif
