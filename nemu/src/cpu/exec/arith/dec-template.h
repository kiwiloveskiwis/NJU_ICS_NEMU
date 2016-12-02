#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	OPERAND_W(op_src, update_sub(op_src->val, 1));
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
