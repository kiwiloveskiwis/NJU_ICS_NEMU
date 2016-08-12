#ifndef __OUT_H__
#define __OUT_H__

#include "cpu/exec/helper.h"
#include "../include/device/port-io.h"

make_helper(out_a2i_b);
make_helper(out_a2i_v);
make_helper(out_a2d_b);
make_helper(out_a2d_v);

#endif
