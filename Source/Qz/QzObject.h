#pragma once 

#ifndef QZOBJECT_H 
#define QZOBJECT_H 

#include "Qz/QzCommon.h" 
#include "Qz/QzBlock.h" 
#include "Qz/QzTable.h" 

typedef enum _qzobjecttype 
{
    QZ_OBJECT_INVALID = 0, 
    QZ_OBJECT_FUNCTION, 
    QZ_OBJECT_CLOSURE, 
} QzObjectType;

typedef struct _qzobject 
{
    QzObjectType type; 
} QzObject; 

typedef struct _qzfunction 
{
    QzObject obj; 
    QzBlock block; 
} QzFunction;

void QzCreateObject(QzObject *self, QzObjectType type); 

void QzDestroyObject(QzObject *self); 

QzFunction *QzNewFunction(void); 

void QzFreeFunction(QzFunction *self); 

#endif 
