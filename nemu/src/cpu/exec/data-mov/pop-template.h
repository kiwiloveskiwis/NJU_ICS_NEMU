#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	DATA_TYPE top_of_stack = swaddr_read(cpu.esp, DATA_BYTE);
	if(op_src->type == OP_TYPE_REG) REG(op_src->reg) = top_of_stack;
	else OPERAND_W(op_dest, top_of_stack);
	cpu.esp += DATA_BYTE;
	print_asm_template1();
}

	make_instr_helper(r)
	make_instr_helper(rm)

#include "cpu/exec/template-end.h"

