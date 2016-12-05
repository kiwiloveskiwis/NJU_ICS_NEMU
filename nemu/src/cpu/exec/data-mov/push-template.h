#include "cpu/exec/template-start.h"

#define instr push

//  10014b:	ff 71 fc				pushl  -0x4(%ecx)	(push_rm_v)
//  100070:	55						push   %ebp			(push_r_v) passed
//  100158:	6a 63					push   $0x63		(push_i_b)
//  10015c:	68 80 12 10 00			push   $0x101280	(push_i_v)
static void do_execute() {
	if(DATA_BYTE == 1) {
		cpu.esp -= 4;
		swaddr_write(cpu.esp, 4, (int32_t) (int8_t) op_src->val, R_SS);
	}
	// WATCH OUT!
	else {
		cpu.esp -= DATA_BYTE;
		swaddr_write(cpu.esp, DATA_BYTE, op_src->val, R_SS);
	}
	print_asm_template1();
}

	make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
	make_instr_helper(r)
	make_instr_helper(rm)
#endif


#include "cpu/exec/template-end.h"
