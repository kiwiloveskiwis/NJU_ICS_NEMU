#include "trap.h"

long long mul(long long a,long long b) {
	long long ans = a*b;
	return ans;
}

int a[] = { -1364082006, 1157693227, -2022600529, 301212728};
long long ans[] = { 1860719719092984036LL, -1579188499418773362LL, 2758992986934981174LL,
	-410878862242972368LL, 1340253607841673529LL, -2341550933349917083LL,
	348711935091793256LL, 4090912899911079841LL, -609233022994333112LL, 90729107509201984LL};


int main() {
	long long b = 1860719719092984036LL;
	nemu_assert(mul(a[0], a[0]) == b);
//	nemu_assert(mul(a[0], a[0]) == ans[0]);
	
	HIT_GOOD_TRAP;

	return 0;
}


