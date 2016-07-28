#include "trap.h"
#include "FLOAT.h"

/* solve 0.5x^2 + 0.5x - 0.5 = 0 */

int main() {
	FLOAT a = f2F(0.5);
	nemu_assert(a == (1 << 15));


	HIT_GOOD_TRAP;
	return 0;
}
