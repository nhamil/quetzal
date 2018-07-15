#pragma once 

#ifndef QZBUFFER_H 
#define QZBUFFER_H 

#include "Qz/QzCommon.h" 

typedef struct _qzbuffer 
{
    void *data; 
    qzulong elemSize; 
    qzuint size; 
    qzuint capacity; 
} QzBuffer;

void QzCreateBuffer(QzBuffer *self, qzulong elemSize); 

void QzDestroyBuffer(QzBuffer *self); 

qzuint QzBufferWrite(QzBuffer *self, const void *data); 

qzbool QzBufferGet(const QzBuffer *self, qzuint index, void *data); 

#endif 
