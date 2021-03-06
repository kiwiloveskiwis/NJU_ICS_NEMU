#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE
make_helper_v(jmp_i)
make_helper_v(jmp_rm)
// ea 0b 01 bd 0b 		jmp obbd:010b
make_helper(ljmp) {
	cpu.sr[R_CS].val = instr_fetch(eip + 5, 2);	
	cpu.sr[R_CS].cached = false;
	cpu.eip = instr_fetch(eip + 1, 4) - 7;
	print_asm("ljmp\t %x:%x", cpu.eip + 7, cpu.sr[R_CS].val);
	return 7;
}

