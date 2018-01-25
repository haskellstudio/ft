/*
  ==============================================================================

    c.cpp
    Created: 24 Jan 2018 3:31:51pm
    Author:  secci

  ==============================================================================
*/

#include "c.h"

#include "asmType.h"
OP opArray[40] = {
	OP(0.0, 0.0, "nop"),
	OP(1.0, 1.0, "mov eax, "),
	OP(2.0, 1.0, "jmp "),
	OP(3.0, 1.0, "lea eax, [esp"),
	OP(4.0, 0.0, "push eax"),
	OP(5.0, 0.0, "mov eax, [eax]"),
	OP(6.0, 0.0, "unused"),
	OP(7.0, 0.0, "ret"),
	OP(8.0, 0.0, "pop ebx; \nmov [ebx], eax"),
	OP(9.0, 1.0, "add esp ,"),
	OP(10.0, 0.0, "pop ebx; \nimul eax, ebx"),
	OP(11.0, 0.0, "pop ebx; \nadd eax, ebx"),
	OP(12.0, 0.0, "call eax"),
	OP(13.0, 0.0, "halt"),
	OP(14.0, 1.0, "jz "),
	OP(15.0, 0.0, "pop ebx  \ncmp eax, ebx"),
	OP(16.0, 0.0, "pop ebx  \ncmp eax, ebx    \nnot eax"),
	OP(17.0, 1.0, "call "),
	OP(18.0, 0.0, "pop ebx \ncmp ebx, eax \njl xxx")    ,
	OP(19.0, 0.0, "pop ebx \ncmp ebx, eax \njg xxx") ,
	OP(20.0, 0.0, "pop ebx \nsub ebx, eax") ,
	OP(21.0, 0.0, "pop ebx \nfdiv ebx, eax") ,
	OP(22.0, 0.0, "pop ebx \nmod ebx, eax") ,
	OP(23.0, 0.0, "pop ebx \nlogic_and ebx, eax") ,
	OP(24.0, 0.0, "pop ebx \nlogic_or ebx, eax") ,
	OP(25.0, 0.0, "pop ebx \nbit_and ebx, eax") ,
	OP(26.0, 0.0, "pop ebx \nbit_or ebx, eax"),
	OP(27.0, 0.0, "pop ebx \nbit_xor ebx, eax"),
	OP(28.0, 0.0, "pop ebx \nshl ebx, eax"),
	OP(29.0, 0.0, "pop ebx \nshr ebx, eax"),
	OP(30.0, 0.0, "neg eax"),   // -3  3 -3 3
	OP(31.0, 0.0, "mov eax, [eax]\nneg eax"),
	OP(32.0, 0.0, "bit neg eax "),   // 3  ~3
	OP(33.0, 0.0, "mov eax, [eax]\nbit neg eax"),
	OP(34.0, 0.0, "not eax"),        // 3 0 1 0 1
	OP(35.0, 0.0, "mov eax, [eax]\nnot eax"),

	OP(36.0, 0.0, "pop ebx \ncmp ebx, eax \njg xxx")    ,
	OP(37.0, 0.0, "pop ebx \ncmp ebx, eax \njl xxx") ,
	OP(38.0, 0.0, "pop ebx \nmov eax, [ebx+ \n") ,
	OP(39.0, 1.0, "call ") ,
	
};
