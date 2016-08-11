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
#define pushfrom(reg) cpu.esp -= 4; swaddr_write(cpu.esp, 4, reg, R_SS);
	pushfrom(cpu.EFLAGS)
	cpu.IF = 0;
	cpu.TF = 0;
	pushfrom(cpu.sr[R_CS].val)
	pushfrom(cpu.eip)

	uint8_t aim = instr_fetch(eip + 1, 1);
	raise_intr(aim);
	return 0; //doesnt matter
#undef pushfrom
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}
