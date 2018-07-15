#pragma once 

#ifndef QZVM_H 
#define QZVM_H 

#include "Qz/QzCommon.h" 
#include "Qz/QzBlock.h" 
#include "Qz/QzEnv.h" 
#include "Qz/QzTable.h" 
#include "Qz/QzValue.h" 

#define QZ_MAX_STACK (256) 
#define QZ_MAX_FRAMES (128) 
#define QZ_MAX_ENVS (256) 

typedef struct _qzframe 
{
    const QzBlock *block; 
    QzEnv *env; 
    qzuint pc; 
} QzFrame;

typedef struct _qzvm 
{
    QzValue stack[QZ_MAX_STACK]; 
    QzFrame frames[QZ_MAX_FRAMES]; 
    const QzBlock *block; 
    QzEnv *env; 
    qzuint sp; 
    qzuint fp; 
    qzuint ep; 
    qzuint pc; 
} QzVM; 

QzVM *QzNewVM(void); 

void QzFreeVM(QzVM *vm); 

qzbool QzVMRunFile(QzVM *vm, const char *file); 

void QzVMRunCode(QzVM *vm, const char *code); 

void QzVMRunBlock(QzVM *vm, const QzBlock *block); 

#endif 
