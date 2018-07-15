#include "Qz/QzError.h" 

#include <stdio.h> 
#include <stdlib.h> 

const char *QzErrorMessage(QzError err) 
{
    switch (err) 
    {
        case QZ_NO_ERROR: return "No error"; 
        case QZ_ASSERTION_FAILED: return "Assertion failed"; 
        case QZ_OUT_OF_MEMORY: return "Out of memory"; 
        case QZ_UQZNOWN_ERROR: 
        default: 
            return "Unknown error"; 
    }
}

void QzThrowError(QzError err) 
{
    printf("QUETZAL FATAL ERROR (%d): %s\n", err, QzErrorMessage(err)); 

    exit(-err); 
}

void QzThrowMsgError(QzError err, const char *msg) 
{
    printf("QUETZAL FATAL ERROR (%d): %s - %s\n", err, QzErrorMessage(err), msg); 

    exit(-err); 
}
