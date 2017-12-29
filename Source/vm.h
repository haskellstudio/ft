//
//  vm.h
//  xcodeConsoleTest
//
//  Created by 朱飞 on 29/12/2017.
//  Copyright © 2017 soho. All rights reserved.
//

#ifndef vm_h
#define vm_h

#include <stdio.h>



#define DEFAULT_STACK_SIZE      1000

typedef enum {
    NOOP    = 0,
    IADD    = 1,   // int add
    ISUB    = 2,
    IMUL    = 3,
    ILT     = 4,   // int less than
    IEQ     = 5,   // int equal
    BR      = 6,   // branch
    BRT     = 7,   // branch if true
    BRF     = 8,   // branch if true
    ICONST  = 9,   // push constant integer
    LOAD    = 10,  // load from local context
    GLOAD   = 11,  // load from global memory
    STORE   = 12,  // store in local context
    GSTORE  = 13,  // store in global memory
    PRINT   = 14,  // print stack top
    POP     = 15,  // throw away top of stack
    CALL    = 16,  // call function at address with nargs,nlocals
    RET     = 17,  // return value from function
    HALT    = 18
} VM_CODE;


typedef struct {
    char name[8];
    int nargs;
} VM_INSTRUCTION;



static VM_INSTRUCTION vm_instructions[] = {
    { "noop",   0 },    // 0
    { "iadd",   0 },    // 1
    { "isub",   0 },    // 2
    { "imul",   0 },    // 3
    { "ilt",    0 },    // 4
    { "ieq",    0 },    // 5
    { "br",     1 },    // 7
    { "brt",    1 },    // 8
    { "brf",    1 },    // 9
    { "iconst", 1 },    // 10
    { "load",   1 },
    { "gload",  1 },
    { "store",  1 },
    { "gstore", 1 },
    { "print",  0 },
    { "pop",    0 },
    { "call",   3 },
    { "ret",    0 },
    { "halt",   0 }
};






typedef struct {
    int stack[1024];      //.stack

    int ip;
    int sp ;
    int fp;

    int startIp;

    int  *code;             //.text
    int code_size;

    int globals[512];      // .data
    int nglobals ;
} VM;

void  vm_create(VM* vm, int *code, int code_size, int nglobals, int startIp)
{
    vm->code = code;
    vm->code_size = code_size;

    vm->nglobals = nglobals;

    vm->startIp = startIp;
    vm->ip = startIp;

    vm->fp = -1;
    vm->sp = -1;
}






void vm_print_instr(int *code, int ip)
{
    int opcode = code[ip];
    VM_INSTRUCTION *inst = &vm_instructions[opcode];
    switch (inst->nargs) {
        case 0:
            printf("%04d:  %-20s", ip, inst->name);
            break;
        case 1:
            printf("%04d:  %-10s%-10d", ip, inst->name, code[ip + 1]);
            break;
        case 2:
            printf("%04d:  %-10s%d,%10d", ip, inst->name, code[ip + 1], code[ip + 2]);
            break;
        case 3:
            printf("%04d:  %-10s%d,%d,%-6d", ip, inst->name, code[ip + 1], code[ip + 2], code[ip + 3]);
            break;
    }
}

void vm_print_stack(int *stack, int count)
{
    printf("stack=[");
    for (int i = 0; i <= count; i++) {
        printf(" %d", stack[i]);
    }
    printf(" ]\n");
}

void vm_print_data(int *globals, int count)
{
    printf("Data memory:\n");
    for (int i = 0; i < count; i++) {
        printf("%04d: %d\n", i, globals[i]);
    }
}





void vm_exec(VM *vm, int trace)
{
    // registers
    int ip = vm->startIp;         // instruction pointer register
    int sp = vm->sp;         // stack pointer register
    
    
    int a = 0;
    int b = 0;
    int addr = 0;
    int offset = 0;

    int opcode = vm->code[ip];
    
//    int false = 0;
//    int true = 1;
    
    while (opcode != HALT && ip < vm->code_size) {
        if (trace) vm_print_instr(vm->code, ip);
        ip++; //jump to next instruction or to operand
        switch (opcode) {
            case IADD:
                b = vm->stack[sp--];           // 2nd opnd at top of stack
                a = vm->stack[sp--];           // 1st opnd 1 below top
                vm->stack[++sp] = a + b;       // push result
                break;
            case ISUB:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = a - b;
                break;
            case IMUL:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = a * b;
                break;
            case ILT:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = (a < b) ? 1 : 0;
                break;
            case IEQ:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = (a == b) ? 1 : 0;
                break;
            case BR:
                ip = vm->code[ip];
                break;
            case BRT:
                addr = vm->code[ip++];
                if (vm->stack[sp--] == 1) ip = addr;
                break;
            case BRF:
                addr = vm->code[ip++];
                if (vm->stack[sp--] == 0) ip = addr;
                break;
            case ICONST:
                vm->stack[++sp] = vm->code[ip++];  // push operand
                break;
            case LOAD: // load local or arg
                offset = vm->code[ip++];
                //vm->stack[++sp] = vm->call_stack[callsp].locals[offset];
                break;
            case GLOAD: // load from global memory
                addr = vm->code[ip++];
                vm->stack[++sp] = vm->globals[addr];
                break;
            case STORE:
                offset = vm->code[ip++];
              //  vm->call_stack[callsp].locals[offset] = vm->stack[sp--];
                break;
            case GSTORE:
                addr = vm->code[ip++];
                vm->globals[addr] = vm->stack[sp--];
                break;
            case PRINT:
                printf("%d\n", vm->stack[sp--]);
                break;
            case POP:
                --sp;
                break;
            case CALL:
            {
                // expects all args on stack
                addr = vm->code[ip++];            // index of target function
                int nargs = vm->code[ip++];     // how many args got pushed
                int nlocals = vm->code[ip++];     // how many locals to allocate
              //  ++callsp; // bump stack pointer to reveal space for this call
              //  vm_context_init(&vm->call_stack[callsp], ip, nargs+nlocals);
                // copy args into new context
                for (int i=0; i<nargs; i++) {
                //    vm->call_stack[callsp].locals[i] = vm->stack[sp-i];
                }
                sp -= nargs;
                ip = addr;        // jump to function
                break;
            }
            case RET:
              //  ip = vm->call_stack[callsp].returnip;
             //   callsp--; // pop context
                break;
            default:
                printf("invalid opcode: %d at ip=%d\n", opcode, (ip - 1));
               // exit(1);
        }
        if (trace) vm_print_stack(vm->stack, sp);
        opcode = vm->code[ip];
    }
    if (trace) vm_print_data(vm->globals, vm->nglobals);
}



#endif /* vm_h */
