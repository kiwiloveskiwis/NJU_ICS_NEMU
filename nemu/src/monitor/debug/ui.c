#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "common.h"

#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
extern void printsh();
extern bool print_cache(hwaddr_t addr);


/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}
static int cmd_bt(char *args) {
	int index = 0;
	int current_ebp = cpu.ebp;
	printf("#%d\t 0x%x (current eip) \n", index++, cpu.eip);

#define readoff(off) swaddr_read(current_ebp + off, 4, R_SS)
	while(current_ebp) {
		if(current_ebp + 20 < 0x8000000) {
			printf("#%d\t 0x%x\t ", index++, readoff(4));
			printf("Stored: %x %x %x %x \n", readoff(8), readoff(12), readoff(16), readoff(20));
		}
		current_ebp = swaddr_read(current_ebp, 4, R_SS);
		if(index >= 10) {
			Log("BackTrace overflow!");
			break;
		}
	}	
#undef readoff
	return 0;
}	

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}
static int cmd_cache(char *args) {	
	int addr;
	sscanf(args, "%x", &addr);
	bool succ = print_cache(addr);
	if(!succ) Log("Cache Not found!");
	return 0;
}

static int cmd_si(char *args) {
	char *arg = strtok(NULL, " ");
	if(arg == NULL)	cpu_exec(1);
	else cpu_exec(atoi(arg));
	return 0;
}

static int cmd_info(char *args) {
	int i;
	char *arg = strtok(NULL, " ");
	if(arg == NULL) printf("\"info\" must be followed by the name of an info command.\n");
	else if (!strcmp(arg, "r")) {
		for(i = R_EAX; i <= R_EDI; i++) {
			printf("%s\t0x%x \n", regsl[i], cpu.gpr[i]._32);	
		}
		printf("eip\t0x%x\t \n", cpu.eip);
		printf("CF\t%d\tPF\t%d\tZF\t%d\tSF\t%d\n",cpu.CF, cpu.PF, cpu.ZF, cpu.SF);
		printf("IF\t%d\tDF\t%d\tOF\t%d\n",cpu.IF, cpu.DF, cpu.OF);
	} else if (!strcmp(arg, "w")) {
		dispWP();
	}
	else printf("Undefined info command:'%s'.\n", arg); 
	return 0;
}

static int cmd_x(char *args) {
	char *arg1 = strtok(NULL, " ");
	char *arg2 = strtok(NULL, " ");
	if(!arg1 || !arg2) {
		Log("Invalid arguments!");
		return 0;
	}
	int length = atoi(arg1);
	unsigned addr;
	sscanf(arg2, "%x", &addr); 
	int i = 0;
	for(i = 0; i < length; i++){
		printf("0x%x:\t%08x\n", addr + 4 * i, swaddr_read(addr + 4 * i, 4, R_DS));
	}
	return 0;
}

static int cmd_p(char *args) {
	bool succ = true;
	bool *success = &succ;
	int ans = expr(args, success); //TODO success == false?
	if(!succ) Log("cmd_p failed!");
	printf("%s:\t0x%x\t%d\n", args, ans, ans);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_w(char *args) {
	WP * newwp = new_wp();
	strcpy(newwp->str, args);
	bool succ = true;
	newwp->value = expr(newwp->str, &succ); 
	printf("New watchpoint %d:\t value = 0x%x\n", newwp->NO, newwp->value);
	if(succ) Log("Hardware watchpoint %d: %s",newwp->NO, args);
	else {
		Log("Unable to set watchpoint! Invalid expression.");
		free_wp(newwp);
	}
	return 0;
}
static int cmd_d (char *args) {
	int idx = atoi(args);
	bool succ = deleWP(idx);
	if (succ) Log("Watchpoint %d deleted!", idx);
	else Log("Watchpoint to be deleted Not Found!");
	return 0;
}

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "cache", "Display the cache's concents which contains ADDR", cmd_cache},
	{ "info", "Print information(r/w)", cmd_info}, 
	{ "si", "Execute one machine instruction", cmd_si},
	{ "bt", "Print backtrace of all stack frames", cmd_bt},
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "x", "Examine memory: x LENGTH ADDRESS.", cmd_x},
	{ "p", "Print value of expression EXP.", cmd_p},
	{ "w", "Set a watchpoint for an expression EXP.", cmd_w},
	{ "d", "Delete a given watchpoint with its NO", cmd_d},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s \t-\t %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	printsh();
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
