#pragma once

#include "stdio.h"
struct OP
{
	float op;
	float nargs;
	char name[64];
	OP(float o, float a, char * n)
	{
		op = o;
		nargs = a;
		strcpy(name, n);
	}
};

typedef enum {
	NOOP = 0,
	mov_eax_xxx = 1,
	jmp = 2,
	lea_eax_mesp_xx = 3,
	push_eax = 4,
	mov_eax_addr = 5,
	mov_eax_byte_addr = 6,
	ret = 7,
	pop_ebx_mov_mebx_eax = 8,
	add_esp_xx = 9,
	pop_ebx_imul_eax_ebx = 10,
	pop_ebx_add_eax_ebx = 11,
	call_eax = 12,
	halt = 13,
	jz = 14,
	pop_ebx_equal_eax_ebx = 15,
	pop_ebx_not_equal_eax_ebx = 16,
	call_addr = 17,
	pop_ebx_ebx_less_than_ebx = 18,
	pop_ebx_ebx_greate_than_ebx = 19,
	pop_ebx_sub_ebx_eax = 20,
	pop_ebx_fdiv_ebx_eax = 21,
	pop_ebx_mod_ebx_eax = 22,
	pop_ebx_logic_and_ebx_eax = 23,
	pop_ebx_logic_or_ebx_eax = 24,
	pop_ebx_bit_and_ebx_eax = 25,
	pop_ebx_bit_or_ebx_eax = 26,
	pop_ebx_bit_xor_ebx_eax = 27,
	pop_ebx_shl_ebx_eax = 28,
	pop_ebx_shr_ebx_eax = 29,
	negative_eax = 30,
	mov_eax_ValueAtEax_negative_eax = 31,
	bit_negative_eax = 32,
	mov_eax_ValueAtEax_bit_negative_eax = 33,
	not_eax = 34,
	mov_eax_ValueAtEax_not_eax = 35,
	pop_ebx_ebx_greate_EQUAL_than_ebx = 36,
	pop_ebx_ebx_less_EQUAL_than_ebx = 37,
	pop_ebx_mov_mebx_eax_with_offset = 38,
	call_api = 39,



} VM_CODE;

