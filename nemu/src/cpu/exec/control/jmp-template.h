#include "cpu/exec/template-start.h"

#define instr jmp
static void do_execute(){
		int inc_adr = op_src->val ;
		cpu.eip += inc_adr;
		if(DATA_BYTE == 2) cpu.eip = ((cpu.eip + DATA_BYTE + 1) & 0x0000ffff) - DATA_BYTE - 1;
		// TODO : right way?
		print_asm("jmp\t$0x%x", cpu.eip + DATA_BYTE + 1);
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat5(instr,_,rm,_,SUFFIX))
{
	int len=concat4(decode_,rm,_,SUFFIX)(eip+1);
	cpu.eip=op_src->val-len-1;
	print_asm_template1();
	return len+1;
}
#endif

#include "cpu/exec/template-end.h"


