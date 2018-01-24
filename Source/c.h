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


#define TYPE_NUM     0

#define TYPE_INTVAR  2
#define TYPE_GLOBAL     3


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

	 int ptokpos;
	 int pnextc;
	 int ppos;
	 char ptok[MAXTOKSZ];

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

		ptokpos = 0;
		pnextc = -1;
		ppos = 0;

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


	int pfgetc()
	{
		if (ppos < strlen(_srcbuf))
		{
			char c = _srcbuf[ppos];
			ppos++;
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

	void preadchr() {
		if (ptokpos == MAXTOKSZ - 1) {
			ptok[ptokpos] = '\0';
			error("[line %d] Token too long: %s\n", linenum, tok);
		}
		ptok[ptokpos++] = pnextc;
		pnextc = pfgetc();

	}


	bool peekToken(char * sToCompare, int apos)
	{
		ppos = apos;
		pnextc = pfgetc();
		for (;;) {
			/* skip spaces */
			while (isspace(pnextc)) {
				pnextc = pfgetc();
			}
			/* try to read a literal token */
			ptokpos = 0;
			while (isalnum(pnextc) || pnextc == '_') {
				preadchr();
			}
			/* if it's not a literal token */
			if (ptokpos == 0) {
				while (pnextc == '<' || pnextc == '=' || pnextc == '>' || pnextc == '!' || pnextc == '&' || pnextc == '|') {
					preadchr();
				}
			}
			/* if it's not special chars that looks like an operator */
			if (ptokpos == 0) {
				/* try strings and chars inside quotes */
				if (pnextc == '\'' || pnextc == '"') {
					char c = pnextc;
					preadchr();
					while (pnextc != c) {
						preadchr();
					}
					preadchr();
				}
				else if (pnextc == '/') { // skip comments
					preadchr();
					if (pnextc == '*') {      // support comments of the form '/**/'
						pnextc = pfgetc();

						while (pnextc != '/') {
							while (pnextc != '*') {
								pnextc = pfgetc();

							}
							pnextc = pfgetc();

						}
						pnextc = pfgetc();

						continue;
					}
					else if (pnextc == '/') { // support comments of the form '//'
						while (pnextc != '\n') {
							pnextc = pfgetc();

						}
						pnextc = pfgetc();

						continue;
					}
				}
				else if (pnextc != EOF) {
					/* otherwise it looks like a single-char symbol, like '+', '-' etc */
					preadchr();
				}
			}
			break;
		}
		ptok[ptokpos] = '\0';
		if (_debug) {
			printf("peek TOKEN: %s\n", ptok);
		}
		if (strcmp(ptok, sToCompare) == 0)
		{
			return true;
		}
		else
			return false;
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
			error("how on earth can arrive here? what the sym->faddr");
		}
		
	}
	 void gen_ret() {
	
		stack_pos = stack_pos - 1;   // pop the return address on the stack

		bin.writeFloat(opArray[ret].op);
	}

	void gen_pop(int n) {
		 if (n > 0) {
			 stack_pos = stack_pos - n;
			 snprintf(PDBsymble[bin.GetPosition()/4], 512-1, "free local variable stack space for %d space", n);
			 bin.writeFloat(add_esp_xx);
			 bin.writeFloat(float(n));
		 }
	}

	 void gen_const(float n) {
		snprintf(PDBsymble[bin.GetPosition()/4], 512-1, "%f is a constant", n);
		bin.writeFloat(opArray[mov_eax_xxx].op);
		bin.writeFloat(n);
	}

	 void gen_push() {
		
		 stack_pos = stack_pos + 1;

		 bin.writeFloat(opArray[push_eax].op);
	 }

	  void gen_call() {
		 bin.writeFloat(opArray[call_eax].op);
	 }



	 void gen_unref(int type) {
		 if (type == TYPE_INTVAR) {
			 bin.writeFloat(opArray[mov_eax_addr].op);
		 }
		 else  {
			 error("not implement in gen_unref function!");
		 }
	 }

	 void gen_stack_addr(int addr) {
		 bin.writeFloat(opArray[lea_eax_mesp_xx].op);
		 bin.writeFloat( float(addr));
	 }


	 void gen_sym_addr(struct sym *sym) {
		
		 int n = sym->stackAddr;

		

		 int fn = sym->faddr;
		 if (sym->type == 'F')
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "function %s addr to eax", sym->name);
		 }
		 else if (sym->type == 'G')
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "global %s addr to eax", sym->name);
		 }

		// bin[binpos++] = opArray[mov_eax_xxx].op;
		 bin.writeFloat(opArray[mov_eax_xxx].op);
		// bin[binpos++] = float(fn);
		 bin.writeFloat(float(fn));
	 }


	 void gen_array(vector<float> & ar) {
		 int n = ar.size();
		 for (int i = n - 1; i >= 0; i--)
		 {
			 gen_const(ar[i]);
			 gen_push();
		 }
		 gen_stack_addr(0);
	 }
	 void gen_array(char *array, int size) {
		 int i = size;
		 char *tok = array;
		
		 for (; i >= 0; i -= 1) {
			 gen_const(tok[i]);
			 gen_push();
		 }
		 /* put token address on stack */
		 gen_stack_addr(0);
	 }

	 struct sym *sym_find(char *s) {
		 int i;
		 struct sym *symbol = NULL;

		 for (i = 0; i < _sympos; i++) {
			 if (strcmp(sym[i].name, s) == 0) {
				 symbol = &sym[i];
			 }
		 }
		 return symbol;
	 }


	 int prim_expr(struct sym *as) {
		 int type = TYPE_NUM;

		 peekToken("what erver", _srcPos - 1);

		 if (isdigit(tok[0])  /*|| tok[0] == '-'*/) {
			 float n = parse_immediate_value();
			 gen_const(n);

			 as->value = n;
			 as->type = 'C';
			 strcpy(as->name, tok);

		 }
		 else if (isalpha(tok[0])) {
			 char symName[MAXTOKSZ];
			 struct sym *s;

			 strcpy(symName, context);
			 strcat(symName, "_");
			 strcat(symName, tok);


			 s = sym_find(symName);  // find symbol in local context..
			 if (s == NULL) {
				 strcpy(symName, "_");
				 strcat(symName, tok);
				 s = sym_find(symName);  // find symbol in global context..
				 if (s == NULL) {
					 // symbol not found... this is an error...
					 error("[line %d] Undeclared symbol: %s\n", linenum, tok);
				 }
			 }

			 printf("SYM: %s\n", symName);
			 if (s->type == 'L') {
				 // Local Symbol
				 // *as = *s;
				 as->faddr = s->faddr;
				 as->stackAddr = s->stackAddr;
				 as->type = s->type;
				 strcpy(as->name, s->name);
				 sprintf(PDBsymble[bin.getFloatPos()], "%s addr to eax", s->name);
				 if (s->stackAddr < 0)
				 {
					 if (currFunction == nullptr)
					 {
						 error("arg cannot compute");
					 }
					 int i = -currFunction->nParams;
					 i = i - 3;
					 i = i - s->stackAddr;                   // adjuct stack order .
					 gen_stack_addr(stack_pos - i - 1);
				 }
				 else
					 gen_stack_addr(stack_pos - s->stackAddr - 1);
				 type = TYPE_INTVAR;
			 }
			 else {

				 as->faddr = s->faddr;
				 as->stackAddr = s->stackAddr;
				 as->type = s->type;

				 strcpy(as->name, s->name);


				 // Other Symbols (Global)
				 gen_sym_addr(s);
				 type = TYPE_GLOBAL;
			 }

		 }
		 else if (accept("(")) {
			 struct sym lls;
			 type = expr(&lls);
			 expect(__LINE__, ")");
			 return type;
		 }
		 else if (tok[0] == '{')
		 {

			 if (accept("{"))
			 {
				 int arrNum = 0;
				 vector<float> ar;
				 for (;;) {

					 char * stop;
					 float a = strtof(tok, &stop);
					 if (stop == tok)
						 error("[line %d] Invalid symbol: %s\n", linenum, tok);
					 /*gen_const(a);
					 gen_push();*/
					 arrNum++;
					 ar.push_back(a);
					 readtok();
					 if (accept("}")) {
						 gen_array(ar);
						 as->faddr = 0;
						 as->stackAddr = 0;
						 as->type = 'A';  // array
						 type = TYPE_NUM;
						 return type;
					 }
					 expect(__LINE__, ",");
				 }
			 }
		 }
		 else if (tok[0] == '"') {
			 char nameTok[201] = { 0 };

			 strncpy(nameTok, tok, 200);

			 int i, j;
			 i = 0; j = 1;
			 while (tok[j] != '"') {
				 if (tok[j] == '\\' && tok[j + 1] == 'x') {
					 char s[3] = { tok[j + 2], tok[j + 3], 0 };
					 uint8_t n = strtol(s, NULL, 16);
					 tok[i++] = n;
					 j += 4;
				 }
				 else {
					 tok[i++] = tok[j++];
				 }
			 }
			 tok[i] = 0;
			 //if (i % 2 == 0) {
			 //    i++;
			 //    tok[i] = 0;
			 //}
			 gen_array(tok, i);
			 type = TYPE_NUM;


			 as->faddr = 0;
			 as->stackAddr = 0;
			 as->type = 'A';  // array

			 strcpy(as->name, "addr of array ");
			 strcat(as->name, nameTok);

		 }

		 else {
			 error("[line %d] Unexpected primary expression: %s\n", linenum, tok);
		 }
		 readtok();
		 return type;
	 }

	 int postfix_expr(struct sym *s) {
		 int type = prim_expr(s);

		 if (
			 (type == TYPE_INTVAR || type == TYPE_GLOBAL) && accept("[")) {
			 if (type == TYPE_INTVAR)
				 binary(TYPE_INTVAR, &Lan::expr_ex,  s, pop_ebx_add_eax_ebx);
			 if (type == TYPE_GLOBAL)
				 binary(TYPE_NUM, &Lan::expr_ex,  s, pop_ebx_add_eax_ebx);
			 //expr(s);
			 expect(__LINE__, "]");
			 type = TYPE_INTVAR;
		 }
		 else if (accept("(")) {
			 int prev_stack_pos = stack_pos;
			 sprintf(PDBsymble[bin.getFloatPos()], "store function %s addr to stack", s->name);
			 gen_push(); /* store function address */
			 int call_addr = stack_pos - 1;
			 if (accept(")") == 0) {
				 struct sym lls;
				 expr(&lls);
				 int argnum = 1;
				 sprintf(PDBsymble[bin.getFloatPos()], "arg%d %s to stack", argnum, lls.name);
				 gen_push();
				 while (accept(",")) {
					 struct sym lls2;
					 expr(&lls2);
					 argnum++;
					 sprintf(PDBsymble[bin.getFloatPos()], "arg%d %s to stack", argnum, lls2.name);
					 gen_push();
				 }
				 expect(__LINE__, ")");
			 }
			 type = TYPE_NUM;

			 sprintf(PDBsymble[bin.getFloatPos()], "function %s addr to eax", s->name);
			 gen_stack_addr(stack_pos - call_addr - 1);
			 gen_unref(TYPE_INTVAR);

			 sprintf(PDBsymble[bin.getFloatPos()], "call %s ", s->name);
			 gen_call();

			 if (currFunction) {
				 //            sprintf(PDBsymble[binpos], "free %d args stack space for function %s", currFunction->nParams, s->name);
				 //
				 //            bin[binpos++] = opArray[add_esp_xx].op;
				 //
				 //            bin[binpos++] = float(currFunction->nParams);


				// gen_call_cleanup(currFunction->nParams);
			 }
			 else {
				 error("[line %d] Error: unexpected function exit\n", linenum);
			 }
			 /* remove function address and args */

			 sprintf(PDBsymble[bin.getFloatPos()], "free %d args stack space and 1 ( store function addr to eax )for function %s", currFunction->nParams, s->name);
			 //bin[binpos++] = opArray[add_esp_xx].op;
			 bin.writeFloat(opArray[add_esp_xx].op);
			// bin[binpos++] = float(stack_pos - prev_stack_pos);
			 bin.writeFloat(float(stack_pos - prev_stack_pos));

			 // gen_pop(stack_pos - prev_stack_pos);
			 stack_pos = prev_stack_pos;
		 }
		 return type;
	 }


	 int prefix_expr(struct sym *s)
	 {
		 int type;

		 while (peek("-") || peek("~") || peek("!"))
		 {
			 if (accept("-"))
			 {
				 type = prefix_expr(s);
				 if (type == TYPE_NUM)
				 {
					// bin[binpos++] = opArray[negative_eax].op;
					 bin.writeFloat(opArray[negative_eax].op);
				 }
				 else
				 {
					
					 bin.writeFloat(opArray[mov_eax_ValueAtEax_negative_eax].op);
				 }
				 type = TYPE_NUM;
				 return type;
			 }

			 else if (accept("~"))
			 {
				 type = prefix_expr(s);
				 if (type == TYPE_NUM)
				 {					
					 bin.writeFloat(opArray[bit_negative_eax].op);
				 }
				 else
				 {
					 bin.writeFloat(opArray[mov_eax_ValueAtEax_bit_negative_eax].op);
				 }
				 type = TYPE_NUM;
				 return type;
			 }
			 else if (accept("!"))
			 {
				 type = prefix_expr(s);
				 if (type == TYPE_NUM)
				 {
					
					 bin.writeFloat(opArray[not_eax].op);
				 }
				 else
				 {
					 bin.writeFloat(opArray[mov_eax_ValueAtEax_not_eax].op);
				 }
				 type = TYPE_NUM;
				 return type;
			 }

		 }
		 type = postfix_expr(s);

		 return type;
	 }

	 int arithmetic_multi_divide_mod_expr(struct sym * s)
	 {
		 int type = prefix_expr(s);

		 while (peek("/") || peek("*") || peek("%"))
		 {
			 if (accept("/")) { // expression '/'
				 type = binary(type, &Lan::postfix_expr,   s, pop_ebx_fdiv_ebx_eax);
			 }
			 else if (accept("*")) { // expression '*'
				 type = binary(type, &Lan::postfix_expr,   s, pop_ebx_imul_eax_ebx);
			 }
			 else if (accept("%")) { // expression '%'
				 type = binary(type, &Lan::postfix_expr,  s, pop_ebx_mod_ebx_eax);
			 }
		 }
		 return type;
	 }


	  int add_expr(struct sym *s) {
		 int type = arithmetic_multi_divide_mod_expr(s);
		 while (peek("+") || peek("-")) {
			 if (accept("+")) {
				 type = binary(type, &Lan::arithmetic_multi_divide_mod_expr,  s, pop_ebx_add_eax_ebx);
			 }
			 else if (accept("-")) {
				 type = binary(type, &Lan::arithmetic_multi_divide_mod_expr,  s, pop_ebx_sub_ebx_eax);
			 }
		 }
		 return type;
	 }
	  int shift_expr(struct sym *s) {
		 int type = add_expr(s);
		 while (peek("<<") || peek(">>")) {
			 if (accept("<<")) {
				 type = binary(type, &Lan::add_expr, s, pop_ebx_shl_ebx_eax);   // 1
			 }
			 else if (accept(">>")) {
				 type = binary(type, &Lan::add_expr,   s, pop_ebx_shr_ebx_eax);   //2
			 }
		 }
		 return type;
	 }

	  int rel_expr(struct sym *s) {
		 int type = shift_expr(s);
		 while (peek("<")) {
			 if (accept("<")) {
				 type = binary(type, &Lan::shift_expr,   s, pop_ebx_ebx_less_than_ebx);
			 }
		 }

		 while (peek(">")) {
			 if (accept(">")) {
				 type = binary(type, &Lan::shift_expr,   s, pop_ebx_ebx_greate_than_ebx);
			 }
		 }

		 while (peek(">=") || peek("<=")) {
			 if (accept(">=")) {
				 type = binary(type, &Lan::rel_expr,   s, pop_ebx_ebx_greate_EQUAL_than_ebx);
			 }
			 else if (accept("<=")) {
				 type = binary(type, &Lan::rel_expr,   s, pop_ebx_ebx_less_EQUAL_than_ebx);
			 }
		 }
		 return type;
	 }

	 int eq_expr(struct sym *s) {
		 int type = rel_expr(s);
		 while (peek("==") || peek("!=")) {
			 if (accept("==")) {
				 type = binary(type, &Lan::rel_expr, s, pop_ebx_equal_eax_ebx);
			 }
			 else if (accept("!=")) {
				 type = binary(type, &Lan::rel_expr, s, pop_ebx_not_equal_eax_ebx);
			 }
		 }
		 return type;
	 }


	 int bitwise_expr(struct sym * s) {
		 int type = eq_expr(s);

		 while (peek("|") || peek("&") || peek("^") /*|| peek("/") || peek("*") || peek("%")*/) {
			 if (accept("|")) {        // expression '|'
				 type = binary(type, &Lan::eq_expr,   s, pop_ebx_bit_or_ebx_eax);  // 3
			 }
			 else if (accept("&")) { // expression '&'
				 type = binary(type, &Lan::eq_expr,   s, pop_ebx_bit_and_ebx_eax); // 4
			 }
			 else if (accept("^")) { // expression '^'
				 type = binary(type, &Lan::eq_expr,   s, pop_ebx_bit_xor_ebx_eax); // 5
			 }
		 }
		 return type;
	 }

	// typedef int (Lan::*pClassFun)(struct sym * s);

	 int binary(int type, int(Lan::*f)(struct sym * s)/*pClassFun f*/,   struct sym *s, int binaryOp = -1) {
		 if (type != TYPE_NUM) {

			 gen_unref(type);
		 }

		 sprintf(PDBsymble[bin.getFloatPos()], "push %s to stack", s->name);

		 gen_push();


		 struct sym lss;
		 type = (this->*f)(&lss);

		 if (type != TYPE_NUM) {
			 sprintf(PDBsymble[bin.getFloatPos()], "%s value to eax", lss.name);
			 gen_unref(type);
		 }

		 if (binaryOp == pop_ebx_imul_eax_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s * %s ", s->name, lss.name, s->name);

			// bin[binpos++] = pop_ebx_imul_eax_ebx;
			 bin.writeFloat(pop_ebx_imul_eax_ebx);
			 strcat(s->name, "*");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_fdiv_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s / %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_fdiv_ebx_eax;
			 bin.writeFloat(pop_ebx_fdiv_ebx_eax);
			 strcat(s->name, "/");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_mod_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s % %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_mod_ebx_eax;
			 bin.writeFloat(pop_ebx_mod_ebx_eax);
			 strcat(s->name, "%");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_logic_and_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s && %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_logic_and_ebx_eax;
			 bin.writeFloat(pop_ebx_logic_and_ebx_eax);
			 strcat(s->name, "&&");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_logic_or_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s || %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_logic_or_ebx_eax;
			 bin.writeFloat(pop_ebx_logic_or_ebx_eax);
			 strcat(s->name, "||");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_bit_and_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s & %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_bit_and_ebx_eax;
			 bin.writeFloat(pop_ebx_bit_and_ebx_eax);

			 strcat(s->name, "&");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_bit_or_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s | %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_bit_or_ebx_eax;
			 bin.writeFloat(pop_ebx_bit_or_ebx_eax);
			 strcat(s->name, "|");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_bit_xor_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s xor %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_bit_xor_ebx_eax;
			 bin.writeFloat(pop_ebx_bit_xor_ebx_eax);
			 strcat(s->name, "xor");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_shl_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s shl %s ", s->name, s->name, lss.name);

			// bin[binpos++] = pop_ebx_shl_ebx_eax;
			 bin.writeFloat(pop_ebx_shl_ebx_eax);
			 strcat(s->name, "shl");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_shr_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s shr %s ", s->name, s->name, lss.name);

		//	 bin[binpos++] = pop_ebx_shr_ebx_eax;
			 bin.writeFloat(pop_ebx_shr_ebx_eax);
			 strcat(s->name, "shr");
			 strcat(s->name, lss.name);
		 }

		 else if (binaryOp == pop_ebx_add_eax_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s + %s ", s->name, lss.name, s->name);

			// bin[binpos++] = pop_ebx_add_eax_ebx;
			 bin.writeFloat(pop_ebx_add_eax_ebx);
			 strcat(s->name, "+");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_sub_ebx_eax)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s - %s ", s->name, lss.name, s->name);

		//	 bin[binpos++] = pop_ebx_sub_ebx_eax;
			 bin.writeFloat(pop_ebx_sub_ebx_eax);
			 strcat(s->name, "-");
			 strcat(s->name, lss.name);
		 }

		 else if (binaryOp == pop_ebx_equal_eax_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s == %s ", s->name, lss.name, s->name);

		//	 bin[binpos++] = pop_ebx_equal_eax_ebx;
			 bin.writeFloat(pop_ebx_equal_eax_ebx);
			 strcat(s->name, " == ");
			 strcat(s->name, lss.name);
		 }


		 else if (binaryOp == pop_ebx_not_equal_eax_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s != %s ", s->name, lss.name, s->name);

			// bin[binpos++] = pop_ebx_not_equal_eax_ebx;
			 bin.writeFloat(pop_ebx_not_equal_eax_ebx);
			 strcat(s->name, " != ");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_ebx_less_than_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s < %s ", s->name, lss.name, s->name);

			// bin[binpos++] = pop_ebx_ebx_less_than_ebx;
			 bin.writeFloat(pop_ebx_ebx_less_than_ebx);
			 strcat(s->name, " < ");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_ebx_greate_than_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s > %s ", s->name, lss.name, s->name);

			// bin[binpos++] = pop_ebx_ebx_greate_than_ebx;
			 bin.writeFloat(pop_ebx_ebx_greate_than_ebx);
			 strcat(s->name, " > ");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_ebx_greate_EQUAL_than_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s >= %s ", s->name, lss.name, s->name);

			// bin[binpos++] = pop_ebx_ebx_greate_EQUAL_than_ebx;
			 bin.writeFloat(pop_ebx_ebx_greate_EQUAL_than_ebx);
			 strcat(s->name, " >= ");
			 strcat(s->name, lss.name);
		 }
		 else if (binaryOp == pop_ebx_ebx_less_EQUAL_than_ebx)
		 {
			 sprintf(PDBsymble[bin.getFloatPos()], "pop %s to ebx,  eax = %s >= %s ", s->name, lss.name, s->name);

			// bin[binpos++] = pop_ebx_ebx_less_EQUAL_than_ebx;
			 bin.writeFloat(pop_ebx_ebx_less_EQUAL_than_ebx);
			 strcat(s->name, " <= ");
			 strcat(s->name, lss.name);
		 }
		 else
		 {
			 error(" %s not implement, the stack can not balence in function binary\n");
		 }
		 
		 stack_pos = stack_pos - 1; /* assume that buffer contains a "pop" */
		 return TYPE_NUM;
	 }

	 int logic_expr(struct sym * s) {
		 int type = bitwise_expr(s);

		 while (peek("||") || peek("&&")  /*|| peek("/") || peek("*") || peek("%")*/) {
			 if (accept("||")) {        // expression '|'
				 type = binary(type, &Lan::bitwise_expr,   s, pop_ebx_logic_or_ebx_eax);  // 6
			 }
			 else if (accept("&&")) { // expression '&'
				 type = binary(type, &Lan::bitwise_expr,  s, pop_ebx_logic_and_ebx_eax);  // 7
			 }

		 }
		 return type;
	 }

	 int expr_ex(struct sym * as) {

		 if (as == nullptr)
		 {
			 error(" expr s is null\n");
		 }

		 int type = logic_expr(as);
		 if (type != TYPE_NUM)
		 {
			 //if(accept)
			 //int a = 2;
			 //if (peek("]") && peekToken("=", spos - 1))
			 //{
				// a = 3;
			 //}
			 if (accept("="))
			 {
				 printf("HERE 1=\n");
				 if (as->faddr != 0)
				 {
					 sprintf(PDBsymble[bin.getFloatPos()], "push %s's addr to stack", as->name);
				 }
				 gen_push();
				 struct sym lls;
				 expr(&lls);
				 if (type == TYPE_INTVAR)
				 {
					 if (as->faddr != 0)
					 {
						 sprintf(PDBsymble[bin.getFloatPos()], "assign value %s to %s", lls.name, as->name);
					 }
					
				//	 bin[binpos++] = opArray[pop_ebx_mov_mebx_eax].op;
					 bin.writeFloat(opArray[pop_ebx_mov_mebx_eax].op);

				 }
				 else
				 {
					 error("can not arrive here in expr_ex function");


				 }
				 stack_pos = stack_pos - 1; // assume ASSIGN contains pop
				 type = TYPE_NUM;
			 }
			 else
			 {
				 if (as->faddr != 0)
				 {
					 sprintf(PDBsymble[bin.getFloatPos()], "%s value to eax", as->name);
				 }
				 //gen_unref(type);
			 }
		 }
		 return type;
	 }



	 int expr(struct sym * as) {

		 if (as == nullptr)
		 {
			 error(" expr s is null\n");
		 }

		 int type = logic_expr(as);
		 if (type != TYPE_NUM)
		 {
			 //if(accept)
			 //int a = 2;
			 //if (peek("]") && peekToken("=", spos - 1))
			 //{
			 //    a = 3;
			 //}
			 if (accept("="))
			 {
				 printf("HERE 1=\n");
				 if (as->faddr != 0)
				 {
					 sprintf(PDBsymble[bin.GetPosition()/4], "push %s's addr to stack", as->name);
				 }
				 gen_push();
				 struct sym lls;
				 expr(&lls);
				 if (type == TYPE_INTVAR)
				 {
					 if (as->faddr != 0)
					 {
						 sprintf(PDBsymble[bin.GetPosition()/4], "assign value %s to %s", lls.name, as->name);
					 }
					
					 bin.writeFloat(opArray[pop_ebx_mov_mebx_eax].op);
					

				 }
				 else
				 {
					 error("can not arrive here in expr function");
				 }
				 stack_pos = stack_pos - 1; // assume ASSIGN contains pop
				 type = TYPE_NUM;
			 }
			 else
			 {
				 if (as->faddr != 0)
				 {
					 sprintf(PDBsymble[bin.GetPosition()/4], "%s value to eax", as->name);
				 }
				 gen_unref(type);
			 }
		 }
		 return type;
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
		
			 if (peekToken("[", _srcPos - 1))   // int a[10];
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
					 var->faddr = bin.GetPosition()/4;
					 var->stackAddr = stack_pos - nItem;

				 }
			 }
			 else  // can not be a = {0 0 0 } , because i only in initialize an array and
			 {
				 var->faddr = bin.GetPosition() / 4;

				 sprintf(PDBsymble[bin.GetPosition() / 4], "alloc %s   on stack, stack_pos : %d", var->name, var->stackAddr);
				 readtok();
				 if (accept("="))
				 {
					 struct sym lls;
					 expr(&lls);   // int a = b ....
				 }
				 gen_push();
				 var->stackAddr = stack_pos - 1;

			 }
			 expect(__LINE__, ";");
			 return;
		 }

		 if (accept("if")) {
			 expect(__LINE__, "(");
			 struct sym lls3;
			 expr(&lls3);
		


			 sprintf(PDBsymble[bin.GetPosition()/4], "if eax == 0  then jump to begin of else block");
		
			 bin.writeFloat(opArray[jz].op);
			
			 bin.writeFloat(-1.0);

			 int patch_if_jz = bin.GetPosition()/4 - 1;


			 int p1 = bin.GetPosition()/4;
			 expect(__LINE__, ")");
			 int prev_stack_pos = stack_pos;

			 //statement();
			 sprintf(PDBsymble[bin.GetPosition() / 4], "begin of if block");
			 if (accept("{")) {
				 while (accept("}") == 0) {
					 statement();
				 }
			 }
			 else
			 {
				 statement();
			 }


			  
			 int p2 = bin.GetPosition() / 4;
			   
			 sprintf(PDBsymble[bin.GetPosition() / 4], "end of if block, jump to else block end");
			 
			 bin.writeFloat(opArray[jmp].op);
			 bin.writeFloat(float(-1));
			 int patch_esle_block_end = bin.GetPosition() / 4 - 1;
			// bin[patch_if_jz] = binpos;

			 bin.writeFloatInPos(bin.GetPosition() / 4, patch_if_jz);
			 *((float*)bin.GetMemory() + patch_if_jz) = bin.GetPosition()/4;
			

			 if (accept("else")) {
				 stack_pos = prev_stack_pos;  // the local var alloc in if {} block will not keep .
				 sprintf(PDBsymble[bin.GetPosition() / 4], "begin of else block");
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
			 
			// bin[patch_esle_block_end] = bin.GetPosition() / 4;
			 bin.writeFloatInPos(bin.GetPosition() / 4, patch_esle_block_end);
			 return;
		 }
		 if (accept("while")) {
			 expect(__LINE__, "(");
			 int p1 = bin.GetPosition() / 4;
			 float jmp_from_while_end = bin.GetPosition() / 4;

			
			 struct sym lls4;
			 expr(&lls4);
			




			 sprintf(PDBsymble[bin.GetPosition() / 4], "while eval to false , then jmp");
			 
			 bin.writeFloat(opArray[jz].op);
			
			 bin.writeFloat(-1);
			 int here_need_jmp_to_while_end = bin.GetPosition() / 4 - 1;

			 int p2 = bin.GetPosition() / 4;


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

			 

			 sprintf(PDBsymble[bin.GetPosition() / 4], "jump to start of while");

			 bin.writeFloat(opArray[jmp].op);
			
			 bin.writeFloat(jmp_from_while_end);
			 //gen_patch(code + bin.GetPosition() / 4, p1);
			
			// gen_patch(code + p2, bin.GetPosition() / 4);

			// bin[here_need_jmp_to_while_end] = binpos;
			 bin.writeFloatInPos(bin.GetPosition() / 4, here_need_jmp_to_while_end);
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
			 gen_ret();
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