#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

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

static int cmd_c(char *args) {
	cpu_exec(-1);
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
	} else if (!strcmp(arg, "w")) {
		dispWP();
	}
	else printf("Undefined info command:'%s'.\n", arg); 
	return 0;
}

static int cmd_x(char *args) {
	char *arg1 = strtok(NULL, " ");
	char *arg2 = strtok(NULL, " ");
	int length = atoi(arg1);
	int addr;
	sscanf(arg2, "%x", &addr); 
	int i = 0;
	for(i = 0; i < length; i++){
		printf("0x%x:\t%08x\n", addr + 4 * i, swaddr_read(addr + 4 * i, 4 ));
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
	// Log("new wp's str is %s", newwp->str);
	bool succ = true;
	newwp->value = expr(newwp->str, &succ); 
	if(succ) Log("Hardware watchpoint %d: %s",newwp->NO, args);
	else {
		Log("Unable to set watchpoint! Invalid expression.");
		free_wp(newwp);
	}
	return 0;
}

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Execute one machine instruction", cmd_si},
	{ "info", "Print information", cmd_info}, 
	{ "x", "Examine memory: x LENGTH ADDRESS.", cmd_x},
	{ "p", "Print value of expression EXP.", cmd_p},
	{ "w", "Set a watchpoint for an expression.", cmd_w},
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
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
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
