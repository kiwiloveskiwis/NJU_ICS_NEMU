#include "trap.h"

int A[10];
int b;

int main() {
	A[0] = -2;
	A[1] = -1;
	A[2] = 2;
	A[3] = 3;

	b = A[3];
	A[4] = b - A[1] + A[3] + A[0] + b;

	nemu_assert(A[3] == 3);
	nemu_assert(A[4] == 8);
	nemu_assert(b == 3);

	HIT_GOOD_TRAP;

	return 0;
}

