#ifndef __cbw_H__
#define __cbw_H__
#include "cpu/exec/helper.h"

make_helper(cbw) {
	if(op_src->size == 16) {
		reg_w(R_AX) = (int16_t) (int8_t) reg_b(R_AL);
	}else reg_l(R_EAX) = (int32_t)(int16_t)reg_w(R_AX);
	print_asm("cbw");
	return 1;

}
#endif
