#include "cpu/exec/template-start.h"
#include "cpu/exec/helper.h"

#define loop_condition(name, cond) make_helper(concat(name, _b)) {  cpu.ecx--; \
	int len = concat(decode_i_, SUFFIX)(eip + 1); \
	int new_eip = cpu.eip + (DATA_TYPE_S)op_src->val; \
	if(cond) cpu.edi = new_eip; \
	print_asm("%s\t$0x%x",str(instr), new_eip + DATA_BYTE + 1);  \
	return len + 1;} 
	 
loop_condition(loop, 0);

loop_condition(loope, cpu.ZF && (DATA_TYPE) cpu.ecx);

loop_condition(loopne, !cpu.ZF && (DATA_TYPE) cpu.ecx);
#include "cpu/exec/template-end.h"
