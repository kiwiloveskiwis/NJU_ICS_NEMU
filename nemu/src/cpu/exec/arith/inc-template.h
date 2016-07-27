#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);
	cpu.CF = (op_src->val == -1);
	cpu.OF = (MSB(op_dest->val) ==  MSB(op_src->val)) && (MSB(op_src->val) != MSB(result));
	update_PZS(result)

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
