#ifndef __cli_h__
#define __cli_h__
#include "cpu/exec/helper.h"
make_helper(cli) {
	cpu.IF = 0;
	return 1;
}

make_helper(sti) {
	cpu.IF = 1;
	return 1;
}


#endif
