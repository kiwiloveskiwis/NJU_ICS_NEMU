#ifndef __PUSH_H__
#define __PUSH_H__

make_helper(push_i_v);


#endif


/*
 * PUSH -- Push Operand onto the Stack
 * Opcode	Instruction Clocks Description
 * FF/6		PUSH m16	5	Push memory word
 * FF /6	PUSH m32	5	Push memory dword
 * 50 + /r	PUSH r16	2	Push register word
 * 50 + /r	PUSH r32	2	Push register dword
 * 6A		PUSH imm8	2	Push immediate byte
 * 68		PUSH imm16	2	Push immediate word
 * 68		PUSH imm32	2	Push immediate dword
 * 0E		PUSH CS		2	Push CS
 * 16		PUSH SS		2	Push SS
 * 1E		PUSH DS		2	Push DS
 * 06		PUSH ES		2	Push ES
 * 0F A0	PUSH FS		2	Push FS
 * OF A8	PUSH GS		2	Push GS
 */
