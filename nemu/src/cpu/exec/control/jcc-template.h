#include "cpu/exec/template-start.h"

// WATCH OUT!!! do_execute() can't be redefined
// Problem : Why cmpl $0xc,0x8(%ebp) ???
#define STR(x) #x
#define jcc_condition(instr, condition) make_helper(concat3(instr, _i_, SUFFIX)) { \
	int len = concat(decode_i_, SUFFIX)(eip + 1); \
	if(condition) {  int shift = 32 - 8 * DATA_BYTE; \
		cpu.eip += ((int)op_src->val << shift) >> shift; \
		if(DATA_BYTE == 2) cpu.eip = ((cpu.eip + DATA_BYTE + 1) & 0x0000ffff) - DATA_BYTE - 1; \
		print_asm("%s\t$0x%x",str(instr), cpu.eip + DATA_BYTE + 1); } \
	return len + 1;}


jcc_condition(jae, !cpu.CF)
 
jcc_condition(jb, cpu.CF)
 
jcc_condition(jbe, cpu.CF || cpu.ZF)
 
jcc_condition(je, cpu.ZF)

jcc_condition(jo, cpu.OF)
 
jcc_condition(jno, !cpu.OF)
 
jcc_condition(jne, !cpu.ZF)
 
jcc_condition(js, cpu.SF)
 
jcc_condition(jns, !cpu.SF)
 
jcc_condition(jp, cpu.PF)
 
jcc_condition(jnp, !cpu.PF)
 
jcc_condition(jl, cpu.SF != cpu.OF)
 
jcc_condition(jle, (cpu.SF != cpu.OF) || cpu.ZF)
 
jcc_condition(jg, !cpu.ZF && cpu.SF == cpu.OF)
 
jcc_condition(jge, cpu.SF == cpu.OF)
 
jcc_condition(jcxz, !cpu.ecx)


jcc_condition(ja, !cpu.CF && !cpu.ZF)
#include "cpu/exec/template-end.h"

