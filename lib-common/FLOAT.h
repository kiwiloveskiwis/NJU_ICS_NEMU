#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;
typedef union{ // little-endian!!
	struct{
		uint32_t frac   :   23 ;
		uint32_t exp    :   8 ;
		uint32_t sign   :   1 ;
	};
}myfloat;

static inline int F2int(FLOAT a) {
	return a / (1 << 16);
}

static inline FLOAT int2F(int a) {
	return a * (1 << 16);
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
	return a * b;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
	nemu_assert(b != 0);
	return a / b;
}

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

// used when calling printf/sprintf to format a FLOAT argument
#define FLOAT_ARG(f) (long long)f

void init_FLOAT_vfprintf(void);

#endif
