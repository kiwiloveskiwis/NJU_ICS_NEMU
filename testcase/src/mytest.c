#include "trap.h"


int cube(int n) {
	return n * n * n;
}

int main() {
	int n, n2, n1, n0;
	for(n = 100; n < 0x100; n ++) {
		n2 = n / 100;
		n1 = (n / 10) % 10;
		n0 = n % 10;

		if(n == cube(n2) + cube(n1) + cube(n0)) {
			nemu_assert(n == 153);
		}
	}


	HIT_GOOD_TRAP;

	return 0;
}

