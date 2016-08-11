#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

extern void raise_intr(uint8_t NO);
extern int push_i_l(swaddr_t eip);
make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");
	return 1;
}

make_helper(int_i_b) {
	uint8_t aim = instr_fetch(eip + 1, 1);
	raise_intr(aim);
	return 0; // doesnt matter 
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}
