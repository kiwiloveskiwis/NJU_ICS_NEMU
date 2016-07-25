#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"


typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	struct watchpoint *prev;
	char str[128]; // Dont use char * str!!!
	uint32_t value;
	/* TODO: Add more members if necessary */

} WP;

WP* new_wp();
void free_wp(WP *wp);
void dispWP();
#endif
