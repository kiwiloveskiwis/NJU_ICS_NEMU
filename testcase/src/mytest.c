#include "trap.h"

long long mul(long long a,long long b) {
	long long ans = a*b;
	return ans;
}

int a[] = { -1364082006, 0x2000000};
long long ans[] = { 1860719719092984036LL};
/*#define NR_DATA (sizeof(test_data) / sizeof(test_data[0]))  
 *   */ 

int main() {


	nemu_assert(mul(a[0], a[0]) == ans[0]);


	/* nemu_assert(loop == NR_DATA * (NR_DATA + 1) / 2);
	 * */
	HIT_GOOD_TRAP;

	return 0;
}


