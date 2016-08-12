#include "cpu/exec/template-start.h"
#define instr call

static void do_execute(){
		DATA_TYPE inc_adr = op_src->val ;
		cpu.esp -= DATA_BYTE;
		swaddr_write(cpu.esp, DATA_BYTE, cpu.eip + DATA_BYTE + 1, R_SS);
		cpu.eip += inc_adr;
		print_asm("call\t0x%x", cpu.eip + 1 + DATA_BYTE);
}
make_instr_helper(i)

make_helper(concat(call_rm_,SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, cpu.eip + len + 1, R_SS);

	cpu.eip = op_src->val - len - 1;

	print_asm_template1();
	return len + 1;
}
#include "cpu/exec/template-end.h"
