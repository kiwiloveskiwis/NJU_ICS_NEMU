#include "cpu/exec/template-start.h"

#define STR(x) #x
#define cmovcc_condition(instr, condition) make_helper(concat3(instr, _rm2r_, SUFFIX)) { \
	int len = concat(decode_rm2r_, SUFFIX)(eip + 1); \
	if(condition) OPERAND_W(op_dest, op_src->val);\
	print_asm_template2();\
	return len + 1;}

cmovcc_condition(cmova, !cpu.CF && !cpu.ZF)

cmovcc_condition(cmovae, !cpu.CF)
 
cmovcc_condition(cmovb, cpu.CF)
 
cmovcc_condition(cmovbe, cpu.CF || cpu.ZF)
 
cmovcc_condition(cmove, cpu.ZF)

cmovcc_condition(cmovo, cpu.OF)
 
cmovcc_condition(cmovno, !cpu.OF)
 
cmovcc_condition(cmovne, !cpu.ZF)
 
cmovcc_condition(cmovs, cpu.SF)
 
cmovcc_condition(cmovns, !cpu.SF)
 
cmovcc_condition(cmovp, cpu.PF)
 
cmovcc_condition(cmovnp, !cpu.PF)
 
cmovcc_condition(cmovl, cpu.SF != cpu.OF)
 
cmovcc_condition(cmovle, (cpu.SF != cpu.OF) || cpu.ZF)
 
cmovcc_condition(cmovg, !cpu.ZF && cpu.SF == cpu.OF)
 
cmovcc_condition(cmovge, cpu.SF == cpu.OF)
 
cmovcc_condition(cmovcxz, !cpu.ecx)

#include "cpu/exec/template-end.h"


