#include "trap.h"
#include "FLOAT.h"

/* solve 0.5x^2 + 0.5x - 0.5 = 0 */

int main() {
	FLOAT x = f2F(0.5);
	FLOAT a = sqrt(x);
	a++;

	HIT_GOOD_TRAP;
	return 0;
}


