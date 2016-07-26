#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;
	cpu.CF = (((long long)op_dest->val - (long long)op_src->val ) >> 32) & 1;
	cpu.OF = (MSB(op_dest->val) ^ MSB(op_src->val)) && (MSB(op_src->val) == MSB(result));
	update_PZS(result)

	print_asm_template2();
}

	make_instr_helper(i2a)
	make_instr_helper(i2rm)
	make_instr_helper(r2rm)
	make_instr_helper(rm2r)
#if DATA_BYTE == 2 || DATA_BYTE == 4
	make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
