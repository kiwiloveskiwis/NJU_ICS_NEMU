#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		if (i > 0) wp_pool[i].prev = &wp_pool[i - 1];
	}
	wp_pool[0].prev = NULL;
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

WP* new_wp(){
	if (free_ == NULL) {
		Log("No more free space for watchpoints available!");
		return NULL;
	}
	WP* temp = free_;
	free_ = free_->next; // free_.prev is unchanged (not necessary)
	temp->next = head;
	if(head != NULL) head->prev = temp;
	head = temp;	 // the last watch point is set to be the head
	head->prev = NULL;
	return temp;
}
void free_wp(WP *wp) {
	if (wp == NULL) {
		Log("The watchpoint to be freed is NULL!");
		return;
	}
	if(wp->prev != NULL) wp->prev->next = wp->next; // Not head
	if(wp->next != NULL) wp->next->prev = wp->prev; // Not tail	
	wp->next = free_;
	free_ = wp;
}

bool checkWP() {
	bool change = false;
	WP* tourwp = head;
	while(tourwp != NULL) {
		bool succ = true;
		uint32_t currVal = expr(tourwp->str, &succ);
		if(currVal != tourwp->value) {
			change = true;
			printf("Hardware watchpoint %d: %s\n \
					Old value = %x\n \
					New value = %x\n", tourwp->NO, tourwp->str, tourwp->value, currVal);
			tourwp->value = currVal;	
		}	
		tourwp = tourwp->next;
	}	
	return change;
}

/* TODO: Implement the functionality of watchpoint */
// TODO: implement cmd_w & (p w) in ui.c


