#include "cpu/exec/helper.h"

#define DATA_BYTE 2 
#include "movsx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4 
#include "movsx-template.h"
#undef DATA_BYTE

make_helper_v(movsb_rm2r);
make_helper_v(movsw_rm2r);
