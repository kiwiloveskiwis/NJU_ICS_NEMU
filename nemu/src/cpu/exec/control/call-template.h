#include "cpu/exec/template-start.h"

#define instr call
// DATA_BYTE can only be 2 or 4
static void do_execute(){
	if(op_src->type == OP_TYPE_IMM) {	
		DATA_TYPE inc_adr = op_src->val ;
		cpu.esp -= DATA_BYTE;
		swaddr_write(cpu.esp, DATA_BYTE, cpu.eip + DATA_BYTE + 1);
		cpu.eip += inc_adr;
		print_asm("call\t0x%x", cpu.eip + 1 + DATA_BYTE);
	}
	else {
		int off;
		if(op_src->type == OP_TYPE_REG) off = 2;
		else off = 3; // Same as jmp
		cpu.esp -= DATA_BYTE;
		MEM_W(cpu.esp, cpu.eip + off);	
		cpu.eip = op_src->val - off;

	}
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
