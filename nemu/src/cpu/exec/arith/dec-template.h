#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);
	cpu.CF = (op_src->val == 0);
	cpu.OF = op_src->val == (1 << (DATA_BYTE * 8 - 1))? 1 : 0;
	update_PZS(result)
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
