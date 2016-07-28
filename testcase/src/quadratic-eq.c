#include "trap.h"
#include "FLOAT.h"

/* solve 0.5x^2 + 0.5x - 0.5 = 0 */

int main() {
	FLOAT a = f2F(0.5);
	FLOAT b = f2F(0.5);
	FLOAT c = f2F(-0.5);

	FLOAT dt = F_mul_F(b, b) - F_mul_F(F_mul_int(a, 4), c);
	nemu_assert(dt ==0x14000);


	HIT_GOOD_TRAP;
	return 0;
}


