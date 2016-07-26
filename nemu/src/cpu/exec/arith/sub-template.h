#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest, result);

	// update EFLAGS
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

/*	make_helper(concat(decode_si2rm_, SUFFIX)) {
		int len = decode_rm_internal(eip, op_dest, op_src2);	
		len += decode_si_b(eip + len);
		return len;
	}
*/
#include "cpu/exec/template-end.h"
