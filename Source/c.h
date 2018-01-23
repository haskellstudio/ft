/*
  ==============================================================================

    c.h
    Created: 23 Jan 2018 11:14:38am
    Author:  secci

  ==============================================================================
*/

#pragma once



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


OP opArray[39] = {
	OP(0.0, 0.0, "nop"),
	OP(1.0, 1.0, "mov eax, "),
	OP(2.0, 1.0, "jmp "),
	OP(3.0, 1.0, "lea eax, [esp"),
	OP(4.0, 0.0, "push eax"),
	OP(5.0, 0.0, "mov eax, [eax]"),
	OP(6.0, 0.0, "unused"),
	OP(7.0, 0.0, "ret"),
	OP(8.0, 0.0, "pop ebx; \n         mov [ebx], eax"),
	OP(9.0, 1.0, "add esp ,"),
	OP(10.0, 0.0, "pop ebx; \n         imul eax, ebx"),
	OP(11.0, 0.0, "pop ebx; \n         add eax, ebx"),
	OP(12.0, 0.0, "call eax"),
	OP(13.0, 0.0, "halt"),
	OP(14.0, 1.0, "jz "),
	OP(15.0, 0.0, "pop ebx  \n         cmp eax, ebx"),
	OP(16.0, 0.0, "pop ebx  \n         cmp eax, ebx    \n         not eax"),
	OP(17.0, 1.0, "call "),
	OP(18.0, 0.0, "pop ebx \n           cmp ebx, eax \n           jl xxx")    ,
	OP(19.0, 0.0, "pop ebx \n           cmp ebx, eax \n           jg xxx") ,
	OP(20.0, 0.0, "pop ebx \n           sub ebx, eax") ,
	OP(21.0, 0.0, "pop ebx \n           fdiv ebx, eax") ,
	OP(22.0, 0.0, "pop ebx \n           mod ebx, eax") ,
	OP(23.0, 0.0, "pop ebx \n           logic_and ebx, eax") ,
	OP(24.0, 0.0, "pop ebx \n           logic_or ebx, eax") ,
	OP(25.0, 0.0, "pop ebx \n           bit_and ebx, eax") ,
	OP(26.0, 0.0, "pop ebx \n           bit_or ebx, eax"),
	OP(27.0, 0.0, "pop ebx \n           bit_xor ebx, eax"),
	OP(28.0, 0.0, "pop ebx \n           shl ebx, eax"),
	OP(29.0, 0.0, "pop ebx \n           shr ebx, eax"),
	OP(30.0, 0.0, "neg eax"),   // -3  3 -3 3
	OP(31.0, 0.0, "mov eax, [eax]\n           neg eax"),
	OP(32.0, 0.0, "bit neg eax "),   // 3  ~3
	OP(33.0, 0.0, "mov eax, [eax]\n           bit neg eax"),
	OP(34.0, 0.0, "not eax"),        // 3 0 1 0 1
	OP(35.0, 0.0, "mov eax, [eax]\n           not eax"),

	OP(36.0, 0.0, "pop ebx \n           cmp ebx, eax \n           jg xxx")    ,
	OP(37.0, 0.0, "pop ebx \n           cmp ebx, eax \n           jl xxx") ,
	OP(38.0, 0.0, "pop ebx \n           mov eax, [ebx+ \n") ,

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




} VM_CODE;


class Lan
{
public:

	bool genStart(String& res)
	{
		bin.writeFloat(0);   // code size 

		bin.writeFloat(opArray[call_addr].op);

		ffixme_offset = bin.GetPosition();

		bin.writeFloat(-1);

		bin.writeFloat(halt);

		return true;
	}

	void getAsmStrAndBin(juce::Array<float> &opRes, String &asmStr)
	{
		asmStr = "asm code: \n";

		bin.Seek(0, SEEK_ORIGIN::SO_BEGINNING);
		float op;
		while (bin.readFloat(op))
		{
			opRes.add(op);
		}

		bin.Seek(0, SEEK_ORIGIN::SO_BEGINNING);
		bin.readFloat(op); // skip bin size
		bool isData = false;
		while (bin.readFloat(op))
		{
			int codeaddr = bin.GetPosition()/4-1;
			
			asmStr += opArray[int(op)].name;
			for (int j = 0; j < opArray[int(op)].nargs; j++)
			{
				float arg;
				bin.readFloat(arg);
				asmStr += String(arg) + " ";
			}

			asmStr += "\n";
			if (int(op) == ret)
			{
				asmStr += "\n\n";
			}
/*			if (int(op) == lea_eax_mesp_xx)
			{
				if (bin[i + j] == 0.0)
				{
					printf("]");
				}
				else
				{
					printf(" + %.1f]", bin[i + j]);
				}
			}
			else if (int(op) == jmp)
			{
				printf("%.0f", bin[i + j]);

				int targetAddr = bin[i + j];

				struct sym * s = sym_find_name(targetAddr, 'F');
				if (s)
				{
					printf("                   %s", s->name);
				}
			}*/

		}


	}

	bool compile(juce::String &Src, juce::Array<float> &opRes, String & compileResult, String & asmStr )
	{
		juce::String res;
		bin.Zero();
		if (!genStart(res))
		{
			compileResult = "fail in genStart!";
			getAsmStrAndBin(opRes, asmStr);
			return false;
		}


		getAsmStrAndBin(opRes, asmStr);
		compileResult = "compile sucess!";

		return true;
	}


protected:
	CMemoryStream bin;
	int ffixme_offset{ 0 };  // call main, the main address 

};