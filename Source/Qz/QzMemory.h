#pragma once 

#ifndef QZMEMORY_H 
#define QZMEMORY_H 

#include "Qz/QzCommon.h" 

void *QzMalloc(qzsize num, qzsize size); 

void *QzCalloc(qzsize num, qzsize size); 

void *QzRealloc(void *mem, qzsize num, qzsize size); 

void QzFree(void *mem); 

#endif 
