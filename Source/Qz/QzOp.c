#include "Qz/QzOp.h" 

const char *QzOpName(QzOp op) 
{
    switch (op) 
    {
        case QZ_OP_RETURN: return "return"; 
        case QZ_OP_RETURN_NONE: return "return_none"; 
        case QZ_OP_ADD: return "add"; 
        case QZ_OP_SUB: return "sub"; 
        case QZ_OP_MUL: return "mul"; 
        case QZ_OP_DIV: return "div"; 
        case QZ_OP_POW: return "pow"; 
        case QZ_OP_NEG: return "neg"; 
        case QZ_OP_PUSH_NONE: return "push_none"; 
        case QZ_OP_LOAD_CONST: return "load_const"; 
        case QZ_OP_LOAD_NAME: return "load_name"; 
        case QZ_OP_STORE_LOCAL: return "store_local"; 
        case QZ_OP_STORE_NAME: return "store_name"; 
        case QZ_OP_PRINT: return "print"; 
        case QZ_OP_INVALID: return "invalid"; 
        case QZ_OP_DUP: return "dup"; 
        case QZ_OP_POP: return "pop"; 
        case QZ_OP_EQUAL: return "equal"; 
        case QZ_OP_NEQUAL: return "nequal"; 
        case QZ_OP_LESS: return "less"; 
        case QZ_OP_LEQUAL: return "lequal"; 
        case QZ_OP_GREATER: return "greater"; 
        case QZ_OP_GEQUAL: return "gequal"; 
        case QZ_OP_PUSH_ENV: return "push_env"; 
        case QZ_OP_POP_ENV: return "pop_env"; 
        case QZ_OP_MAKE_FUNCTION: return "make_function"; 
        case QZ_OP_CALL: return "call"; 
        default: return "unknown"; 
    }
}
