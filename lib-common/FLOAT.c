#include "FLOAT.h"

typedef unsigned int uint32_t;

typedef union{ // little-endian!!
	struct{
		uint32_t frac   :   23 ;
		uint32_t exp    :   8 ;
		uint32_t sign   :   1 ;

	};
}myfloat;

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	return ((unsigned long long)a * b ) >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	int sign = 1 * (a < 0) * (b < 0);
	a = Fabs(a);
	b = Fabs(b);
	nemu_assert( b != 0);
	FLOAT result = a / b;
	int i = 0;
	for (i = 0; i < 16; i++) {
		a <<= 1;
		result <<= 1;
		if (a >= b) {
			a -= b;
			result++;
		}
	}
	return sign * result;
}

FLOAT f2F(float s) {
	myfloat a = * (myfloat *) &s;
	unsigned int x = (a.exp) ? a.frac + (1 << 23) : a.exp;
	int exp = a.exp - (127 + 7);
	if (exp < 0) x >>= - exp;
	if (exp > 0) x <<= exp ;
	return a.sign? -x :x;
}

FLOAT Fabs(FLOAT a) {
	return (a >= 0) ? a: -a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}


