#include "cpu/exec/template-start.h"

#define instr rol

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest = (dest << count) | (dest >> (DATA_BYTE * 8 - count));
	OPERAND_W(op_dest, dest);
	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"

