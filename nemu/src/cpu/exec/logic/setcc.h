#define DATA_BYTE 1
#include "cpu/exec/template-start.h"
#include "cpu/exec/helper.h"

#define set_helper(instr, condition)\
make_helper(concat3(set, instr,_b)) {\
	      int len = decode_rm_b(eip + 1); \
			        if(condition) OPERAND_W(op_src, 1);\
						      else OPERAND_W(op_src, 0); \
								        return len + 1;\
} \

	set_helper(a, !cpu.CF && !cpu.ZF)
	set_helper(ae, !cpu.CF)
	set_helper(b, cpu.CF)
	set_helper(be, cpu.CF || cpu.ZF)
	set_helper(e, cpu.ZF)
	set_helper(o, cpu.OF)
	set_helper(no, !cpu.OF)
	set_helper(ne, !cpu.ZF)
	set_helper(s, cpu.SF)
	set_helper(ns, !cpu.SF)
	set_helper(p, cpu.PF)
	set_helper(np, !cpu.PF)
	set_helper(l, cpu.SF != cpu.OF)
	set_helper(le, (cpu.SF != cpu.OF) || cpu.ZF)
	set_helper(g, !cpu.ZF && cpu.SF == cpu.OF)
	set_helper(ge, cpu.SF == cpu.OF)

#include "cpu/exec/template-end.h"
#undef DATA_BYTE

