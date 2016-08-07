#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
extern make_helper(decode_rm2r_v);

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)

make_helper(mov_crx2r) {
	uint8_t m = instr_fetch(eip + 1, 1);
	int src = (m >> 3) & 0x7;
	int dst = m & 0x7;
	switch(src) {
		case 0	: reg_l(dst) = cpu.cr0.val;
				  break;
		default	: Log("mov_crx2r not valid!");	
	}
	print_asm("mov_crx2r" " %%%s%d, %%%s", "cr",src, regsl[dst]);
	return 2;
}

make_helper(mov_r2crx) {
	uint8_t m = instr_fetch(eip + 1, 1);
	int src = m & 0x7;
	int dst = (m >> 3) & 0x7;
	switch(dst) {
		case 0	: cpu.cr0.val = reg_l(src);
				  break;
		default	: Log("mov_crx2r not valid!");	
	}
	print_asm("mov_r2crx" " %%%s, %%%s%d", regsl[src], "cr", dst);
	return 2;
}

make_helper(mov_rm2sreg) {
	int len = decode_rm2r_w(eip + 1);	// op_src = rm. op_dst = reg
	cpu.sr[op_dest->reg].val = op_src->val;	
	cpu.sr[op_dest->reg].cached = false;
	return len + 1;
}
