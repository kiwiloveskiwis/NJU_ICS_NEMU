#include "nemu.h"
#include "common.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

extern uint32_t find_var_addr(char * name, bool* succ);

enum {
	NOTYPE = 256, EQ, NUM, NEQ, HEX, DEC, AND, NOT, REG, OR, PTR, NEG, VAR

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\$\\w+", REG},
	{"\\(", '('},
	{"\\)", ')'},
   	{"==", EQ},						// equal
	{"!=", NEQ},	
	{"!", NOT},
	{"&&", AND},
	{"\\|{2}",OR},
	{"0x[0-9a-fA-F]+", HEX},
	{"[a-zA-z_][_a-zA-z0-9]+", VAR},
	{"[0-9]+", DEC}
	
};
	
#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];
uint32_t eval(uint32_t p, uint32_t q);
uint32_t getDomin(uint32_t p, uint32_t q);
bool checkParen(uint32_t p, uint32_t q);

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	
	int position = 0;
	int i ;
	regmatch_t pmatch;
	nr_token = 0;
	for (i = 0; i < 32; i++)
		memset(tokens[i].str, '\0', 32);
	while(e[position] != '\0') {
		if(nr_token >= 32) Log("Exceed max token size!");
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				/*	
					Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", \
					i, rules[i].regex, position, substr_len, substr_len, substr_start);
					*/
				position += substr_len;
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
#define PTYPE tokens[nr_token - 1].type
				bool unary = nr_token == 0 || (PTYPE == '+') || (PTYPE == '-') || (PTYPE == '*') || (PTYPE == '/' ) \
							 || (PTYPE == NEG) || (PTYPE == PTR) || (PTYPE == NOT) || (PTYPE == '(');
#undef PTYPE
				switch(rules[i].token_type) {
					case NOTYPE : break;	
					case HEX : 
					case DEC :
								  tokens[nr_token].type = rules[i].token_type;
								  if (substr_len < 32)
									  memcpy(tokens[nr_token].str, substr_start, substr_len);
								  else {
									  Log("Element exceeds max length!");
									  return false;
								  }
								  nr_token++;
								  break;
					case VAR :	
								  tokens[nr_token].type = rules[i].token_type;
								  memcpy(tokens[nr_token].str, substr_start, substr_len );
								  nr_token++;
								  break;
					case REG : 
								  tokens[nr_token].type = rules[i].token_type;
								  memcpy(tokens[nr_token].str, substr_start + 1, substr_len - 1 );
								  nr_token++;
								  break;
					case '*' : 
								  if(unary) {
									  tokens[nr_token++].type = PTR;
									  break;
								  }
					case '-' :
								  if(unary) {
									  tokens[nr_token++].type = NEG;
									  break;
								  }
					case '(' :
					case ')' :
					case '+' :  
					case '/' :
					case EQ	 :
					case NEQ :
					case AND :
					case OR  :
					case NOT :
								  tokens[nr_token++].type = rules[i].token_type;
								  break;
					default: panic("please implement me");
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	
	// Log("Now the nr_token is %d", nr_token);	
	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	return eval(0, nr_token - 1);
	return 0;
}

uint32_t eval(uint32_t p, uint32_t q) {
	//Log("p == %d,q == %d",p, q);
	if (p > q) {
		Log("p > q!");
		assert(0);
	}
	else if (p == q) {
		int temp = 0;
		char * tempstr = tokens[p].str;
		bool succ;
		switch (tokens[p].type) {
			case DEC : return atoi(tokens[p].str);
			case HEX : 
					   sscanf(tempstr, "%x", &temp);
					   return temp;
			case VAR :	succ = true; 
						uint32_t addr = find_var_addr(tempstr, &succ);
						if (!succ) Log("Varable Not Found!");
						return addr;
			case REG :
					   for(temp = R_EAX; temp <= R_EDI; temp++) {
						   if(!strcmp(tempstr, regsl[temp])) return cpu.gpr[temp]._32;
						   if(!strcmp(tempstr, "eip")) return cpu.eip;
					   }
					   Log("REG Not Found!");
					   printf("%s " ,tempstr);
					   break;
			default  : Log("Type Not Found!");
					   break;
		}
		Log("%s, Type Not Found!", tempstr);
	}
	else if (checkParen(p, q)) return eval(p + 1, q - 1);
	else {
		uint32_t domin = getDomin(p, q);	
		if (domin == p) switch (tokens[domin].type) {
			case NOT : return !eval(p + 1, q);
			case NEG : return -eval(p + 1, q);
			case PTR : return swaddr_read(eval(p + 1, q), 4);
		}
		uint32_t left = eval(p, domin - 1);
		uint32_t right = eval(domin + 1, q);
		switch (tokens[domin].type) {
			case '*' : return left * right; 
			case '/' : return left / right;
			case '+' : return left + right;
			case '-' : return left - right;
			case NEQ : return left != right; 
			case EQ  : return left == right;
			case AND : return left && right;
			case OR  : return left || right;
		}
	}
	return 0;
}

uint32_t getDomin (uint32_t p, uint32_t q) {
	int check = 0;
	int i = p+1;
	int min = p;
	
	for(;i < q; i++) {
		if(tokens[i].type == '(')check++;
		if(tokens[i].type == ')')check--;
		if(check != 0) continue;
#define EQUAL(name) tokens[i].type == name
		if(EQUAL('*') || EQUAL('+') || EQUAL('-') || EQUAL('/')){
			if (min != p && tokens[min].type > 256) break;
			switch (tokens[i].type) {
				case '-' :
				case '+' : min = i; break;
				case '*' : 
				case '/' : 
						   if ( min == p || tokens[min].type == '*' || tokens[min].type == '/') min = i;
						   break;
				default : break;
			}
		} // EQ NEQ OR AND are prior domins
		else if (EQUAL(EQ) || EQUAL(OR) || EQUAL(AND) || EQUAL(NEQ)) {
			if (min != p && tokens[min].type > 256) break; // the first of these is the dominant token
			min = i;
			break;
		}
#undef EQUAL
	}
	// if min == p then domin must be an unary operator
	return min;
}

bool checkParen(uint32_t p, uint32_t q) {
	if (tokens[p].type != '(' || tokens[q].type != ')') return false;
	int i = 0;
	int check = 0;
	for (i = p + 1; i < q; i++) {
		if (tokens[i].type == '(') check++;
		if (tokens[i].type == ')') check--;
		if (check < 0) return false;
	}
	if (check != 0) Log("Mismatch parentheses!");
	return true;
}
			
