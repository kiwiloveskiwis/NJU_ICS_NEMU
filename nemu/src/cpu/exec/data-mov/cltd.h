#ifndef __CLTD_H__
#define __CLTD_H__
#include "cpu/exec/helper.h"

make_helper(cltd) {
	if(op_src->size == 16) {
		if(cpu.gpr[R_AX]._16 >> 15) cpu.gpr[R_DX]._16 = 0xffff;
		else cpu.gpr[R_DX]._16 = 0;
	}else cpu.edx = (cpu.eax >> 31) ? 0xffffffff : 0;
	print_asm("cltd");
	return 1;

}
#endif
