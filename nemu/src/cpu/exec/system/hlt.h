#ifndef __HLT_H__
#define __HLT_H__
#include "cpu/exec/helper.h"

make_helper(hlt) {
	return cpu.INTR;
}
#endif
