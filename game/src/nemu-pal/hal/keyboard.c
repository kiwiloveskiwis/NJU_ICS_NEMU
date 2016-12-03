#include "hal.h"

#define NR_KEYS 18

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

/* Only the following keys are used in NEMU-PAL. */
static const int keycode_array[] = {
	K_UP, K_DOWN, K_LEFT, K_RIGHT, K_ESCAPE,
	K_RETURN, K_SPACE, K_PAGEUP, K_PAGEDOWN, K_r,
	K_a, K_d, K_e, K_w, K_q,
	K_s, K_f, K_p
};

static int key_state[NR_KEYS];

void
keyboard_event(void) {
	/* TODO: Fetch the scancode and update the key states. */
	uint8_t key_code = in_byte(0x60);
	bool release = key_code > 0x80;
	int i;
	for (i = 0; i < NR_KEYS; i++) {
		if(key_state[i] == KEY_STATE_RELEASE) key_state[i] = KEY_STATE_EMPTY;
		if (!release && keycode_array[i] == key_code) {
			switch(key_state[i]){
				case KEY_STATE_EMPTY : 
				case KEY_STATE_RELEASE:
					key_state[i] = KEY_STATE_PRESS;
					break;
				case KEY_STATE_PRESS :
					key_state[i] = KEY_STATE_WAIT_RELEASE;
					break;
				case KEY_STATE_WAIT_RELEASE:
				default: // do nothing
					break;
			}
		} else if (release && keycode_array[i] == key_code - 0x80) {
			key_state[i] = KEY_STATE_RELEASE;
		}
	}// for loop ends
}


static inline int
get_keycode(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return keycode_array[index];
}

static inline int
query_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	return key_state[index];
}

static inline void
release_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_WAIT_RELEASE;
}

static inline void
clear_key(int index) {
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_EMPTY;
}

bool 
process_keys(void (*key_press_callback)(int), void (*key_release_callback)(int)) {
	cli();
	/* TODO: Traverse the key states. Find a key just pressed or released.
	 * If a pressed key is found, call `key_press_callback' with the keycode.
	 * If a released key is found, call `key_release_callback' with the keycode.
	 * If any such key is found, the function return true.
	 * If no such key is found, the function return false.
	 * Remember to enable interrupts before returning from the function.
	 */
	int i;
	bool found = false;
	for(i = 0; i < NR_KEYS; i++) {
		if(key_state[i] == KEY_STATE_PRESS) {
			key_press_callback(get_keycode(i));
			found = true;
		} else if (key_state[i] == KEY_STATE_RELEASE) {
			key_release_callback(get_keycode(i));
			found = true;
		}
	}
	sti();
	return found;
}
