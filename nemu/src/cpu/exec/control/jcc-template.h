#include "cpu/exec/template-start.h"

#define jcc_condition(condition) static void do_execute() {\
	if(condition) { cpu.eip += (DATA_TYPE) op_src->val;\
		print_asm_template1(); } }\
	make_instr_helper(i)

#define instr ja
jcc_condition(!cpu.CF && !cpu.ZF)
#undef instr

#define instr jae
jcc_condition(!cpu.CF)
#undef instr
 
#define instr jb
jcc_condition(cpu.CF)
#undef instr
 
#define instr jbe
jcc_condition(cpu.CF || cpu.ZF)
#undef instr
 
#define instr je
jcc_condition(cpu.ZF)
#undef instr

#define instr jo
jcc_condition(cpu.OF)
#undef instr
 
#define instr jno
jcc_condition(!cpu.OF)
#undef instr
 
#define instr jne
jcc_condition(!cpu.ZF)
#undef instr
 
#define instr js
jcc_condition(cpu.SF)
#undef instr
 
#define instr jns
jcc_condition(!cpu.SF)
#undef instr
 
#define instr jp
jcc_condition(cpu.PF)
#undef instr
 
#define instr jnp
jcc_condition(!cpu.PF)
#undef instr
 
#define instr jl
jcc_condition(cpu.SF != cpu.OF)
#undef instr
 
#define instr jle
jcc_condition((cpu.SF != cpu.OF) || cpu.ZF)
#undef instr
 
#define instr jg
jcc_condition(!cpu.ZF && cpu.SF == cpu.OF)
#undef instr
 
#define instr jge
jcc_condition(cpu.SF == cpu.OF)
#undef instr
 
#define instr jcxz
jcc_condition(!cpu.ecx)
#undef instr

#include "cpu/exec/template-end.h"

