#include "cpu/exec/helper.h"


#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

make_helper_v(push_r)
make_helper_v(push_rm)
make_helper_v(push_i)

make_helper(pushad) {
#define pushfrom(reg) cpu.esp -= 4; swaddr_write(cpu.esp, 4, reg, R_SS);
	uint32_t temp = cpu.esp;
	pushfrom(cpu.eax)
	pushfrom(cpu.ecx)
	pushfrom(cpu.edx)
	pushfrom(cpu.ebx)
	pushfrom(temp)
	pushfrom(cpu.ebp)
	pushfrom(cpu.esi)
	pushfrom(cpu.edi)
	return 1;
#undef pushfrom
}


