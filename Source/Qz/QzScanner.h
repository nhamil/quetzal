#pragma once 

#ifndef QZSCANNER_H 
#define QZSCANNER_H 

#include "Qz/QzCommon.h" 
#include "Qz/QzBuffer.h" 

typedef enum _qztokentype 
{
    QZ_TOKEN_INVALID = 0, 
    QZ_TOKEN_EOF, 

    QZ_TOKEN_IDENTIFIER, 
    QZ_TOKEN_NUMBER, 
    QZ_TOKEN_STRING, 

    QZ_TOKEN_NONE, 
    QZ_TOKEN_PRINT, 
    QZ_TOKEN_MODULE, 
    QZ_TOKEN_LOAD, 
    QZ_TOKEN_USE, 
    QZ_TOKEN_AS, 
    QZ_TOKEN_CLASS, 
    QZ_TOKEN_VAR, 
    QZ_TOKEN_FUNCTION, 
    QZ_TOKEN_RETURN, 
    QZ_TOKEN_IF, 
    QZ_TOKEN_ELIF, 
    QZ_TOKEN_ELSE, 
    QZ_TOKEN_FOR, 
    QZ_TOKEN_WHILE, 
    QZ_TOKEN_NOT, 
    QZ_TOKEN_AND, 
    QZ_TOKEN_OR, 
    QZ_TOKEN_CONST, 
    QZ_TOKEN_STATIC, 

    QZ_TOKEN_ADD, 
    QZ_TOKEN_ADD_EQUAL, 
    QZ_TOKEN_INCR, 
    QZ_TOKEN_SUB, 
    QZ_TOKEN_SUB_EQUAL, 
    QZ_TOKEN_DECR, 
    QZ_TOKEN_MUL, 
    QZ_TOKEN_MUL_EQUAL, 
    QZ_TOKEN_DIV, 
    QZ_TOKEN_DIV_EQUAL, 
    QZ_TOKEN_EXP, 
    QZ_TOKEN_EXP_EQUAL, 

    QZ_TOKEN_PAREN_OPEN, 
    QZ_TOKEN_PAREN_CLOSE, 

    QZ_TOKEN_BRACE_OPEN, 
    QZ_TOKEN_BRACE_CLOSE, 

    QZ_TOKEN_BRACKET_OPEN, 
    QZ_TOKEN_BRACKET_CLOSE, 

    QZ_TOKEN_EQUAL, 
    QZ_TOKEN_EEQUAL, 
    QZ_TOKEN_NEQUAL, 
    QZ_TOKEN_LESS, 
    QZ_TOKEN_LEQUAL, 
    QZ_TOKEN_GREATER, 
    QZ_TOKEN_GEQUAL, 

    QZ_TOKEN_PERIOD, 
    QZ_TOKEN_COMMA, 
    QZ_TOKEN_SEMICOLON 
} QzTokenType;

typedef struct _qzscanner 
{
    const char *code; 
    qzulong length; 
    qzulong curIndex; 
    qzuint curLine; 
} QzScanner;

typedef struct _qztoken 
{
    QzTokenType type; 
    union 
    {
        const char *text; 
        qzdouble number; 
    }; 
    qzuint length; 
    qzuint line; 
} QzToken;

static inline QzToken QzMakeToken(QzTokenType type, const char *text, qzuint length, qzuint line) 
{
    QzToken token; 
    token.type = type; 
    token.text = text; 
    token.length = length; 
    token.line = line; 
    return token; 
}

void QzCreateScanner(QzScanner *self, qzulong length, const char *code); 

void QzDestroyScanner(QzScanner *self); 

QzToken QzScannerNext(QzScanner *self); 

void QzScannerFillBuffer(QzScanner *self, QzBuffer *buffer); 

#endif 
