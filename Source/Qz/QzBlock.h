#pragma once 

#ifndef QZBLOCK_H 
#define QZBLOCK_H 

#include "Qz/QzBuffer.h" 
#include "Qz/QzCommon.h" 
#include "Qz/QzOp.h" 
#include "Qz/QzValue.h" 

typedef struct _qzblock 
{
    QzBuffer constants; 
    QzBuffer names; 
    QzBuffer opcodes; 
    QzBuffer lines; 
} QzBlock;

QzBlock *QzNewBlock(void); 

void QzFreeBlock(QzBlock *self); 

void QzCreateBlock(QzBlock *self); 

void QzDestroyBlock(QzBlock *self); 

QzOp QzBlockGetOp(const QzBlock *self, qzuint index); 

qzuint QzBlockGetLine(const QzBlock *self, qzuint index); 

QzValue QzBlockGetConstant(const QzBlock *self, qzuint index); 

const char *QzBlockGetName(const QzBlock *self, qzuint index); 

void QzBlockWriteOp(QzBlock *self, QzOp op, qzuint line); 

qzuint QzBlockAddConstant(QzBlock *self, QzValue value); 

qzuint QzBlockAddName(QzBlock *self, const char *name); 

#endif 
