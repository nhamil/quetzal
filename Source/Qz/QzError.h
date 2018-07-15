#pragma once 

#ifndef QZERROR_H 
#define QZERROR_H 

#include "Qz/QzCommon.h" 

typedef enum _qzerror  
{
    QZ_NO_ERROR = 0, 
    QZ_ASSERTION_FAILED, 
    QZ_OUT_OF_MEMORY, 
    QZ_UQZNOWN_ERROR
} QzError;

const char *QzErrorMessage(QzError err); 

void QzThrowError(QzError err); 
void QzThrowMsgError(QzError err, const char *msg); 

#endif 
