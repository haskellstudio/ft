
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "asmType.h"
#include <stdio.h>

extern OP opArray[39];


const int stackSize = 1024;

inline void printstatus(float eax, float ebx, int esp, int pc, float stack[])
{
	
	DBG("eax = "<< eax <<"\nebx = "<<ebx<<"\nesp = "<< esp << "\neip = "<< pc);
	DBG("stack begin:");
	for (int i = esp; i < stackSize; i++)
	{
		DBG(i << "       "<<stack[i] );
	}
	DBG("stack end: \n\n");
	DBG(".");
	DBG(".");
	DBG(".");
	DBG(".");
}


inline float vm(float bytecode[])
{
	float stack[stackSize];
	int codeSize = bytecode[0];

	int pc = 0;
	int esp = stackSize; // no value
	int op = 0;
	float eax = 0;
	float ebx = 0;


	for (pc = 1; pc < codeSize; )
	{
		printstatus(eax, ebx, esp, pc, stack);
		op = bytecode[pc++];
		// int args = opArray[op].nargs;

		if (op == mov_eax_xxx)
		{
			eax = bytecode[pc++];
		}
		else if (op == jmp)
		{
			pc = bytecode[pc];
		}
		else if (op == jz)
		{
			int targetPc = bytecode[pc++];
			if (eax == 0)
			{
				printf("\n eax == 0 jump to %d\n", targetPc);
				pc = targetPc;
			}
			else
			{
				printf("\n eax != 0 do not jump\n");
				//do nothing
			}
		}
		else if (op == lea_eax_mesp_xx)
		{
			eax = esp + bytecode[pc++];
		}
		else if (op == push_eax)
		{
			stack[--esp] = eax;
		}
		else if (op == mov_eax_addr)
		{
			eax = stack[(int)eax];
		}
		else if (op == mov_eax_byte_addr)
		{

		}
		else if (op == ret)
		{
			pc = stack[esp];
			esp++;
		}
		else if (op == pop_ebx_mov_mebx_eax)
		{
			ebx = stack[esp++];
			stack[(int)ebx] = eax;
		}
		else if (op == add_esp_xx)
		{
			esp += bytecode[pc++];
		}
		else if (op == pop_ebx_imul_eax_ebx)
		{
			ebx = stack[esp++];
			eax = eax * ebx;
		}
		else if (op == pop_ebx_fdiv_ebx_eax)
		{
			ebx = stack[esp++];
			eax = ebx / eax;
		}
		else if (op == pop_ebx_mod_ebx_eax)
		{
			ebx = stack[esp++];
			eax = (int)ebx % (int)eax;
		}
		else if (op == pop_ebx_logic_and_ebx_eax)
		{
			ebx = stack[esp++];
			if (eax != 0 && ebx != 0)
				eax = 1;
			else
				eax = 0;
		}
		else if (op == pop_ebx_logic_or_ebx_eax)
		{
			ebx = stack[esp++];
			if (eax == 0 && ebx == 0)
				eax = 0;
			else
				eax = 1;
		}
		else if (op == pop_ebx_bit_and_ebx_eax)
		{
			ebx = stack[esp++];
			eax = (int)eax & (int)ebx;
		}
		else if (op == pop_ebx_bit_or_ebx_eax)
		{
			ebx = stack[esp++];
			eax = (int)eax | (int)ebx;
		}
		else if (op == pop_ebx_bit_xor_ebx_eax)
		{
			ebx = stack[esp++];
			eax = (int)eax ^ (int)ebx;
		}
		else if (op == pop_ebx_shl_ebx_eax)
		{
			ebx = stack[esp++];
			eax = (int)ebx << (int)eax;
		}
		else if (op == pop_ebx_shr_ebx_eax)
		{
			ebx = stack[esp++];
			eax = (int)ebx >> (int)eax;
		}
		else if (op == pop_ebx_add_eax_ebx)
		{
			ebx = stack[esp++];
			eax = eax + ebx;
		}
		else if (op == pop_ebx_sub_ebx_eax)
		{
			ebx = stack[esp++];
			eax = ebx - eax;
		}
		else if (op == pop_ebx_equal_eax_ebx)
		{
			ebx = stack[esp++];
			if (eax == ebx)
			{
				eax = 1;
			}
			else
				eax = 0;
		}
		else if (op == pop_ebx_not_equal_eax_ebx)
		{
			ebx = stack[esp++];
			if (eax != ebx)
			{
				eax = 1;
			}
			else
				eax = 0;
		}
		else if (op == pop_ebx_ebx_less_than_ebx)
		{
			ebx = stack[esp++];
			if (ebx < eax)
			{
				eax = 1;
			}
			else
				eax = 0;
		}
		else if (op == pop_ebx_ebx_greate_than_ebx)
		{
			ebx = stack[esp++];
			if (ebx > eax)
			{
				eax = 1;
			}
			else
				eax = 0;
		}
		else if (op == pop_ebx_ebx_greate_EQUAL_than_ebx)
		{
			ebx = stack[esp++];
			if (ebx >= eax)
			{
				eax = 1;
			}
			else
				eax = 0;
		}
		else if (op == pop_ebx_ebx_less_EQUAL_than_ebx)
		{
			ebx = stack[esp++];
			if (ebx <= eax)
			{
				eax = 1;
			}
			else
				eax = 0;
		}

		else if (op == call_eax)
		{
			stack[--esp] = pc;
			pc = eax;

		}
		else if (op == halt)
		{
			printf("virtual machine exit with return value is %f\n", eax);
			break;
		}
		else if (op == call_addr)
		{

			stack[--esp] = pc + 1; // + 1 because call has 1 argument

			pc = bytecode[pc];
		}
		else if (op == negative_eax)
		{
			eax = -eax;
		}
		else if (op == mov_eax_ValueAtEax_negative_eax)
		{
			eax = stack[(int)eax];
			eax = -eax;
		}
		else if (op == bit_negative_eax)
		{
			eax = ~(int)eax;
		}
		else if (op == not_eax)
		{
			eax = !eax;
		}
		else if (op == mov_eax_ValueAtEax_bit_negative_eax)
		{
			eax = stack[(int)eax];
			eax = ~(int)eax;
		}
		else if (op == mov_eax_ValueAtEax_not_eax)
		{
			eax = stack[(int)eax];
			eax = !eax;
		}

	}


	return eax;
}

