#pragma once 

#ifndef QZOP_H 
#define QZOP_H 

#include "Qz/QzCommon.h" 
#include "Qz/QzValue.h" 

typedef enum _qzop 
{
    QZ_OP_INVALID = 0, 
    QZ_OP_RETURN, 
    QZ_OP_RETURN_NONE, 

    QZ_OP_PUSH_NONE, 
    QZ_OP_LOAD_CONST, 
    QZ_OP_LOAD_NAME, 
    QZ_OP_STORE_LOCAL, 
    QZ_OP_STORE_NAME, 

    QZ_OP_ADD, 
    QZ_OP_SUB, 
    QZ_OP_MUL, 
    QZ_OP_DIV, 
    QZ_OP_POW, 
    QZ_OP_NEG, 

    QZ_OP_PRINT, 

    QZ_OP_DUP, 
    QZ_OP_POP, 

    QZ_OP_EQUAL, 
    QZ_OP_NEQUAL, 
    QZ_OP_LESS, 
    QZ_OP_LEQUAL, 
    QZ_OP_GREATER, 
    QZ_OP_GEQUAL, 

    QZ_OP_PUSH_ENV, 
    QZ_OP_POP_ENV, 

    QZ_OP_MAKE_FUNCTION, 
    QZ_OP_CALL, 
} QzOp;

typedef struct _qzopbuffer 
{
    qzubyte *buffer; 
    qzuint *line; 
    qzuint size; 
    qzuint _capacity; 
} QzOpBuffer;

const char *QzOpName(QzOp op); 

#endif 
