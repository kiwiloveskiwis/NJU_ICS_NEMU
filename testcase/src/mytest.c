#include "trap.h"


int main() {
	int n = 28, sum, i, k = 0;
	for(n = 28; n < 29; n ++) {
		sum = 0;
		for(i = 1; i < n; i ++) {
			if(n % i == 0) {
				sum += i;
			}
		}

		if(sum == n) {
			nemu_assert(n == 28);
			k ++;
		}
	}

	nemu_assert(k == 1);

	HIT_GOOD_TRAP;

	return 0;
}



