#include "trap.h"

int main() {
	long long a = -1364082006;
	long long ans = 1860719719092984036LL;
	nemu_assert(a * a == ans);

	HIT_GOOD_TRAP;

	return 0;
}

