#ifndef __LGDT_H__
#define __LGDT_H__
#include"cpu/exec/helper.h"

extern int decode_rm_l(swaddr_t eip);
// 0F 01 /2		LGDT m16&32		 Load m into GDTR

make_helper(lgdt){
	int len = decode_rm_l(eip + 1);
	lnaddr_t addr = op_src->addr; 
	cpu.gdtr_limit = lnaddr_read(addr, 2);
	if(op_src->size == 16) cpu.gdtr_base = lnaddr_read(addr + 2, 3);
	else cpu.gdtr_base = lnaddr_read(addr + 2, 4);
	print_asm("lgdt\t" "%s", op_src->str);
	return len + 1;
}

make_helper(lidt){
	int len = decode_rm_l(eip + 1);
	lnaddr_t addr = op_src->addr; 
	cpu.gdtr_limit = lnaddr_read(addr, 2);
	if(op_src->size == 16) cpu.idtr_base = lnaddr_read(addr + 2, 3);
	else cpu.idtr_base = lnaddr_read(addr + 2, 4);
	print_asm("lidt\t" "%s", op_src->str);
	return len + 1;
}

#endif
