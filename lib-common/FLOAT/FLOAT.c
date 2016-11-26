#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b)  {
	long long la = (long long)a;
	long long lb = (long long)b;
	union {
		long long l;
		FLOAT i[2];
	}u;
	u.l = la * lb;
	return ((u.i[0] >> 16) & 0xffff) + ((u.i[1] & 0xffff) << 16);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	int i;
	FLOAT x = (a / b) << 16;
	a -= (a / b) * b;
	a <<= 1;
	for(i = 1; i <= 16; i++) {
		x += (a / b) << (16 - i);
		a -= (a / b) * b;
		a <<= 1;
	}
	return x;
}

FLOAT f2F(float a) {
	int i = 0;
	void *p = (void *)&a;
	int *bp = (int *)p;
	int b = *bp;
	unsigned msb = (b & 0x80000000) >> 31;
	unsigned e = (b & 0x7f800000) >> 23;
	unsigned val = 0x00800000 | (b & 0x007fffff);
	e = (e - 134) & 0xff; //-134=-127+16-23
	if(e >> 7){
		e = (~(e - 1)) & 0xff;
		val >>= e;
	}else {
		val <<= e;
	}
	i = (msb) ? -val : val;
	return i;
}

FLOAT Fabs(FLOAT a) {
	return (a < 0) ? -a : a;
}

FLOAT sqrt(FLOAT x)  {
	FLOAT dt, t = int2F(2);
	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));
	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	FLOAT t2, dt, t = int2F(2);
	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));
	return t;
}


