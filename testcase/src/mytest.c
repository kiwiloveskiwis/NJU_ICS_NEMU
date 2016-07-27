#include "trap.h"

#define N 1

struct dummy {
	int pad1[N];
	char pad2[N];
} d;

struct dummy fun(struct dummy a) {
	return a;
}

int main() {
	d.pad1[0] = 128;
	d.pad2[0] = 0;
	struct dummy t = fun(d);// got wrong

	nemu_assert(t.pad1[0] ==  128);


	HIT_GOOD_TRAP;

	return 0;
}

