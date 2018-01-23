/*
  ==============================================================================

    c.h
    Created: 23 Jan 2018 11:14:38am
    Author:  secci

  ==============================================================================
*/

#pragma once

#include <stdexcept>
#include <stdarg.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;
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
	OP(8.0, 0.0, "pop ebx; \n          mov [ebx], eax"),
	OP(9.0, 1.0, "add esp ,"),
	OP(10.0, 0.0, "pop ebx; \n         imul eax, ebx"),
	OP(11.0, 0.0, "pop ebx; \n         add eax, ebx"),
	OP(12.0, 0.0, "call eax"),
	OP(13.0, 0.0, "halt"),
	OP(14.0, 1.0, "jz "),
	OP(15.0, 0.0, "pop ebx  \n          cmp eax, ebx"),
	OP(16.0, 0.0, "pop ebx  \n          cmp eax, ebx    \n         not eax"),
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



#define MAXSYMBOLS 4096
#define MAXTOKSZ 256

#define MAXCODESZ 4096

static struct sym {
	char type;
	char name[MAXTOKSZ];
	int  nParams;
	char params[64][64];
	float stackAddr;  // local var address    or   function arg address
	float faddr; // code address for fuction . ex: main = 29
	int value;
	sym()
	{
		stackAddr = faddr = 0;
		value = 0;
		type = 0;
	}
} sym[MAXSYMBOLS];


class Lan
{

public:

	 char tok[MAXTOKSZ]; /* current token */
	
	 int _nextc;          /* next char to be pushed into token */
	 int linenum ;
	 int _debug;
	 char context[MAXTOKSZ];
	 int lastIsReturn ;
	 struct sym *currFunction;
	 int stack_pos;

	 int _srcPos;
	 int _sympos;

	 char PDBsymble[MAXCODESZ][512] ;
	 char PDBFunctionSymble[MAXCODESZ][512] = { 0 };
public:

	void init()
	{
		
		_debug = 0;
		linenum = 1;

		lastIsReturn = 0;
		currFunction = nullptr;
		bin.Zero();
		_src = "";
		_srcPos = 0;
		_sympos = 0;
		stack_pos = 0;
		_srcbuf = nullptr;
		_lastErrorInfo = "";

		strcpy(context, "");

		for (int i = 0; i < MAXCODESZ; i++)
		{
			PDBsymble[i][0] = 0;
		}

	}
	bool genStart()
	{
		bin.writeFloat(0);   // code size 

		bin.writeFloat(opArray[call_addr].op);

		ffixme_offset = bin.GetPosition()/4;

		bin.writeFloat(-1);

		bin.writeFloat(halt);

		return true;
	}

	void getAsmStrAndBin(juce::Array<float> &opRes, juce::String &asmStr)
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

				if (int(op) == lea_eax_mesp_xx)
				{
					if (arg == 0.0)
					{
						asmStr += String("]");
					}
					else
					{
						asmStr += String(arg) + "]";
					}

				}
				else if (int(op) == jmp)
				{
					asmStr += String(arg);
/*				 
					struct sym * s = sym_find_name(arg, 'F');
					if (s)
					{
						asmStr += s->name;
					}
					*/
				}
				else
				{
					asmStr += String(arg) + " ";
				}
				
			}

			asmStr += "\n";
			if (int(op) == ret)
			{
				asmStr += "\n\n";
			}

		}


	}


	int fgetc()
	{
		if (_srcPos < strlen(_srcbuf))
		{
			char c = _srcbuf[_srcPos];
			_srcPos++;
			return c;
		}
		else
		{
			return -1;
		}
	}



	 void error(const char *fmt, ...) {
		char errorbuf[1024] = { 0 };
		va_list args;
		va_start(args, fmt);
		_vsnprintf(errorbuf, sizeof(errorbuf)-1, fmt, args);
		va_end(args);
		throw std::runtime_error(errorbuf);
	}


	bool readchr(int & tokpos) {
		if (tokpos == MAXTOKSZ - 1) {
			tok[tokpos] = '\0';
			error("[line %d] Token too long: %s\n", linenum, tok);
			return false;
		}
		tok[tokpos++] = _nextc;
		_nextc = fgetc();
		if ('\n' == _nextc) { linenum++; }
		return true;
	}
	bool wrong(int a, int b)
	{
		if (a == '!' && b == '!')
		{
			return true;
		}
		if (a == '<' && b == '!')
		{
			return true;
		}
		if (a == '<' && b == '&')
		{
			return true;
		}
		if (a == '<' && b == '|')
		{
			return true;
		}
		if (a == '=' && b == '>')
		{
			return true;
		}
		if (a == '=' && b == '!')
		{
			return true;
		}
		if (a == '=' && b == '&')
		{
			return true;
		}
		if (a == '=' && b == '|')
		{
			return true;
		}
		if (a == '>' && b == '|')
		{
			return true;
		}
		if (a == '>' && b == '&')
		{
			return true;
		}
		if (a == '>' && b == '|')
		{
			return true;
		}
		if (a == '!' && b == '&')
		{
			return true;
		}
		if (a == '!' && b == '|')
		{
			return true;
		}
		if (a == '&' && b == '|')
		{
			return true;
		}
		return false;
	}


	void readtok() {
		int tokpos = 0;
		for (;;) {
			/* skip spaces */
			while (isspace(_nextc)) {
				_nextc = fgetc();
				if ('\n' == _nextc) { linenum++; }
			}
			/* try to read a literal token */
			tokpos = 0;
			while (isalnum(_nextc) || _nextc == '_') {
				readchr(tokpos);
			}
			/* if it's not a literal token */
			if (tokpos == 0) {
				int oldc = _nextc;
				while (_nextc == '<' || _nextc == '=' || _nextc == '>' || _nextc == '!' || _nextc == '&' || _nextc == '|') {

					readchr(tokpos);
					if (wrong(oldc, _nextc))
						break;

				}
			}
			/* if it's not special chars that looks like an operator */
			if (tokpos == 0) {
				/* try strings and chars inside quotes */
				if (_nextc == '\'' || _nextc == '"') {
					char c = _nextc;
					readchr(tokpos);
					while (_nextc != c) {
						readchr(tokpos);
					}
					readchr(tokpos);
				}
				else if (_nextc == '/') { // skip comments
					readchr(tokpos);
					if (_nextc == '*') {      // support comments of the form '/**/'
						_nextc = fgetc();
						if ('\n' == _nextc) { linenum++; }
						while (_nextc != '/') {
							while (_nextc != '*') {
								_nextc = fgetc();
								if ('\n' == _nextc) { linenum++; }
							}
							_nextc = fgetc();
							if ('\n' == _nextc) { linenum++; }
						}
						_nextc = fgetc();
						if ('\n' == _nextc) { linenum++; }
						continue;
					}
					else if (_nextc == '/') { // support comments of the form '//'
						while (_nextc != '\n') {
							_nextc = fgetc();
							if ('\n' == _nextc) { linenum++; }
						}
						_nextc = fgetc();
						if ('\n' == _nextc) { linenum++; }
						continue;
					}
				}
				else if (_nextc != EOF) {
					/* otherwise it looks like a single-char symbol, like '+', '-' etc */
					readchr(tokpos);
				}
			}
			break;
		}
		tok[tokpos] = '\0';
		if (_debug) {
			printf("TOKEN: %s\n", tok);
		}
	}


	int peek(char *s) {
		return (strcmp(tok, s) == 0);
	}

	/* read the next token if the current token machtes the string */
	int accept(char *s) {
		if (peek(s)) {
			readtok();
			return 1;
		}
		return 0;
	}

	int typename_() {
		if (peek("int") || peek("char")) {
			readtok();
			while (accept("*"));
			return 1;
		}
		if (peek("void")) {  // skip 'void' token
			readtok();
		}
		return 0;
	}



	// ctx:  context
	// name: symbol name
	// type: symbol type
	//       L - local symbol
	//       F - function
	//       G - global
	//       U - undefined
	// addr: symbol address
	struct sym *sym_declare(char *ctx, char *name, char type, int stackpos = 0) {
		char sName[MAXTOKSZ];
		int  ii;

		strcpy(sName, ctx);
		strcat(sName, "_");
		strcat(sName, name);

		for (ii = 0; ii < _sympos; ii++) {
			if (0 == strcmp(sym[ii].name, sName)) {
				error("[line %d] variable redefined '%s'\n", linenum, name);
			}
		}

		strncpy(sym[_sympos].name, sName, MAXTOKSZ);
		sym[_sympos].type = type;
		sym[_sympos].stackAddr = stackpos;
		_sympos++;
		if (_sympos > MAXSYMBOLS) {
			error("[line %d] Too many symbols\n", linenum, tok);
		}
		return &sym[_sympos - 1];
	}


	float parse_immediate_value() {
		int currentTokLen = strlen(tok);
		int curGlobalPos = _srcPos;

		char* currentParsePos = _srcbuf + curGlobalPos - currentTokLen - 1;
		//strtof("3.141592", nullptr)
		char * stop;

		float a = strtof(currentParsePos, &stop);


		if (stop == currentParsePos)
			error("[line %d] Invalid symbol: %s\n", linenum, currentParsePos);
		else
		{
			_srcPos = stop - _srcbuf;
			_nextc = fgetc();
		}

		return a;
	}

	void expect(int srclinenum, char *s) {
		if (accept(s) == 0) {
			error("[line %d ; srcline %d] Error: expected '%s', but found: %s\n", linenum, srclinenum, s, tok);
		}
	}

	void gen_sym(struct sym *sym) {
		if (sym->type == 'G') {
			//sym->faddr = (bin.GetPosition()/4)-1;
		}
		error("how on earth can arrive here? what the sym->faddr");
	}
	 void gen_ret() {
	
		stack_pos = stack_pos - 1;   // pop the return address on the stack

		bin.writeFloat(opArray[ret].op);
	}




	  void statement() {
		 lastIsReturn = 0;
		 if (accept("{")) {
			 int prev_stack_pos = stack_pos;
			 while (accept("}") == 0) {
				 statement();
			 }
			 gen_pop(stack_pos - prev_stack_pos);
			 stack_pos = prev_stack_pos;
			 strcpy(context, "");

			 return;
		 }
		 if (typename_()) {
			 struct sym *var = sym_declare(context, tok, 'L', stack_pos);
			 printf("GENERATE_VAR %s_%s\n", context, tok);
		
			 if (peekToken("[", spos - 1))   // int a[10];
			 {
				 readtok();
				 if (accept("["))
				 {
					 char * stop = nullptr;
					 int nItem = std::strtof(tok, &stop);

					 if (nItem < 1)
					 {
						 error("[line %d] array number must great than 0: %s\n", linenum, tok);
					 }

					 if (stop == tok)
						 error("[line %d] array number is invalid: %s\n", linenum, tok);
					 else
					 {
						 for (int i = 0; i < nItem; i++)
						 {
							 gen_const(-1);
							 gen_push();
						 }
						 /* put token address on stack */
						 //gen_stack_addr(0);


					 }
					 readtok();
					 expect(__LINE__, "]");
					 var->faddr = binpos;
					 var->addr = stack_pos - nItem;

				 }
			 }
			 else  // can not be a = {0 0 0 } , because i only in initialize an array and
			 {
				 var->faddr = binpos;

				 sprintf(PDBsymble[binpos], "alloc %s   on stack, stack_pos : %d", var->name, var->addr);
				 readtok();
				 if (accept("="))
				 {
					 struct sym lls;
					 expr(&lls);   // int a = b ....
				 }
				 gen_push();
				 var->addr = stack_pos - 1;

			 }
			 expect(__LINE__, ";");
			 return;
		 }

		 if (accept("if")) {
			 expect(__LINE__, "(");
			 struct sym lls3;
			 expr(&lls3);
			 emit(GEN_JZ, GEN_JZSZ);


			 sprintf(PDBsymble[binpos], "if eax == 0  then jump to begin of else block");
			 bin[binpos++] = opArray[jz].op;
			 bin[binpos++] = float(-1);

			 int patch_if_jz = binpos - 1;


			 int p1 = codepos;
			 expect(__LINE__, ")");
			 int prev_stack_pos = stack_pos;

			 //statement();
			 sprintf(PDBsymble[binpos], "begin of if block");
			 if (accept("{")) {
				 while (accept("}") == 0) {
					 statement();
				 }
			 }
			 else
			 {
				 statement();
			 }


			 emit(GEN_JMP, GEN_JMPSZ);
			 int p2 = codepos;
			 gen_patch(code + p1, codepos);


			 sprintf(PDBsymble[binpos], "end of if block, jump to else block end");
			 bin[binpos++] = opArray[jmp].op;
			 bin[binpos++] = float(-1);
			 int patch_esle_block_end = binpos - 1;
			 bin[patch_if_jz] = binpos;



			 if (accept("else")) {
				 stack_pos = prev_stack_pos;  // the local var alloc in if {} block will not keep .
				 sprintf(PDBsymble[binpos], "begin of else block");
				 if (accept("{")) {
					 while (accept("}") == 0) {
						 statement();
					 }
				 }
				 else
				 {
					 statement();
				 }
			 }



			 stack_pos = prev_stack_pos;   // the local var alloc in if {}  or  else {} block will not keep .
			 gen_patch(code + p2, codepos);
			 bin[patch_esle_block_end] = binpos;
			 return;
		 }
		 if (accept("while")) {
			 expect(__LINE__, "(");
			 int p1 = codepos;
			 float jmp_from_while_end = binpos;

			 gen_loop_start();
			 struct sym lls4;
			 expr(&lls4);
			 emit(GEN_JZ, GEN_JZSZ);




			 sprintf(PDBsymble[binpos], "while eval to false , then jmp");

			 bin[binpos++] = opArray[jz].op;
			 bin[binpos++] = float(-1);
			 int here_need_jmp_to_while_end = binpos - 1;

			 int p2 = codepos;


			 expect(__LINE__, ")");


			 int prev_stack_pos = stack_pos;


			 if (accept("{")) {
				 while (accept("}") == 0) {
					 statement();
				 }
			 }
			 else
			 {
				 statement();
			 }

			 stack_pos = prev_stack_pos;

			 emit(GEN_JMP, GEN_JMPSZ);

			 sprintf(PDBsymble[binpos], "jump to start of while");
			 bin[binpos++] = opArray[jmp].op;
			 bin[binpos++] = jmp_from_while_end;

			 gen_patch(code + codepos, p1);
			 gen_patch(code + p2, codepos);

			 bin[here_need_jmp_to_while_end] = binpos;
			 return;
		 }
		 if (accept("return")) {
			 if (peek(";") == 0) {
				 struct sym lls5;
				 expr(&lls5);
			 }
			 expect(__LINE__, ";");
			 gen_pop(stack_pos); // remove all locals from stack (except return address)
			 lastIsReturn = 1;
			 if (currFunction == nullptr)
				 error("why there is ret when no funtion");
			 //else if (strcmp("_main", currFunction->name) == 0)
			 //{
			 //    gen_halt();
			 //}
			 //else
			 gen_ret(numPreambleVars);
			 return;
		 }
		 // we should process an expression...
		 struct sym lls6;
		 expr(&lls6);
		 expect(__LINE__, ";");
	 }
	void compile()
	{
		genStart();
		_nextc = fgetc();

		if ('\n' == _nextc)
		{
			linenum++;
		}

		readtok();

		juce::String res;
		

		while (tok[0] != 0) {
			if (typename_() == 0) {
				error("[line %d] Error: type name expected\n", linenum);
			}
			struct sym *var = sym_declare(context, tok, 'U');
			readtok();
			if (accept("["))
			{
				snprintf(PDBsymble[bin.GetPosition()/4],512, "%s  is a global array like a[xx]", var->name);

				float gf = parse_immediate_value();

				var->type = 'G';
				var->faddr = bin.GetPosition() / 4;
				for (int i = 0; i < (int)gf; i++)
				{
					bin.writeFloat(9999);
				}
				readtok();
				expect(__LINE__, "]");
				expect(__LINE__, ";");
				continue;
			}
			else if (accept("="))
			{
				if (tok[0] == '-')   // a = -1.2
				{
					sprintf(PDBsymble[bin.GetPosition()/4], "%s is a global negative value", var->name);
					readtok();
					float gf = parse_immediate_value();
					var->type = 'G';
					var->faddr = bin.GetPosition() / 4;
					bin.writeFloat( gf);
					readtok();
					expect(__LINE__, ";");
					continue;
				}
				else if (isdigit(tok[0]))
				{
					sprintf(PDBsymble[bin.GetPosition() / 4], "%s  is a global value", var->name);
					float gf = parse_immediate_value();

					var->type = 'G';
					var->faddr = bin.GetPosition() / 4;
					bin.writeFloat( gf);
					readtok();
					expect(__LINE__, ";");
					continue;

				}
			}
			else if (accept(";")) {
				 
					sprintf(PDBsymble[bin.GetPosition()/4], "%s is a global value with unintial, set to zero", var->name);

					var->type = 'G';
					var->faddr = bin.GetPosition() / 4;
					bin.writeFloat(8888); 
					var->faddr = bin.GetPosition() / 4;
					continue;
 
			}

			// global end



			expect(__LINE__, "(");
			int argc = 0;
			for (;;) {
				argc++;
				if (typename_() == 0) {
					break;
				}
				printf("GEN_PARM_VAR %s_%s\n", var->name, tok);
				struct sym * s = sym_declare(var->name, tok, 'L', -argc - 1);

				sprintf(var->params[argc - 1], "%s", /*s->name*/ tok);

				readtok();
				if (peek(")")) {
					break;
				}
				expect(__LINE__, ",");
			}
			expect(__LINE__, ")");
			if (accept(";") == 0) {
				if (strcmp(context, "") != 0) {
					error("[line %d] Error: here is funtion body, but no function name?\n", linenum);
				}
				stack_pos = 0;
				 
				var->type = 'F';
				var->nParams = argc;
				var->faddr = bin.GetPosition()/4;

				sprintf(PDBFunctionSymble[bin.GetPosition()/4], "function: %s ", var->name);
				if (var->nParams == 0)
				{
					strcat(PDBFunctionSymble[bin.GetPosition() / 4], "(void)");
				}
				else
				{
					for (int i = 0; i < var->nParams; i++)
					{
						if (i == 0)
						{
							strcat(PDBFunctionSymble[bin.GetPosition() / 4], "( ");
						}


						strcat(PDBFunctionSymble[bin.GetPosition() / 4], var->params[i]);

						if (i == var->nParams - 1)
						{
							strcat(PDBFunctionSymble[bin.GetPosition() / 4], " )");
						}
						else
						{
							strcat(PDBFunctionSymble[bin.GetPosition() / 4], ", ");
						}
					}
				}


				gen_sym(var);
				printf("FUNCTION: %s with %d params\n", var->name, argc);
				strcpy(context, var->name);
	

				currFunction = var;
				statement(); // function body
				if (!lastIsReturn) {
					gen_ret();   // issue a ret if user forgets to put 'return'
				}
			}


		}

		_lastErrorInfo = "compile success!";

	}


	bool compile(juce::String &src, juce::Array<float> &opRes, String & compileResult, String & asmStr )
	{
		init();
		_src = src;
		_srcbuf = _src.getCharPointer().getAddress();

		try
		{
			compile();
			getAsmStrAndBin(opRes, asmStr);
			compileResult = _lastErrorInfo;
			return true;
		}
		catch (std::runtime_error& e) {
			getAsmStrAndBin(opRes, asmStr);
			compileResult = e.what();
			return false;
		}
	}


protected:
	juce::String _src;

	char * _srcbuf;

	juce::String _lastErrorInfo;

	CMemoryStream bin;

	int ffixme_offset{ 0 };  // call main, the main address 

};