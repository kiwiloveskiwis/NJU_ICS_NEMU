#include "trap.h"
#include <string.h>



int main() {
	nemu_assert(strcmp(strcat("a","b"), "ab") == 0);

//passed

	HIT_GOOD_TRAP;
	return 0;
}

