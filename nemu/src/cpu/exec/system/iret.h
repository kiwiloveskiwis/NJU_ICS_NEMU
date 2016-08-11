#ifndef __iret_h__
#define __iret_h__
#include "cpu/exec/helper.h"
void raise_intr(uint8_t NO);
make_helper(iret) {

	print_asm_template1();
	
	return 0; 
}




#endif
