#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	OPERAND_W(op_dest, update_add(op_dest->val, op_src->val));
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

