#pragma once 

#ifndef QZTABLE_H
#define QZTABLE_H 

#include "Qz/QzCommon.h" 

typedef struct _qztableentry QzTableEntry;

typedef struct _qztable 
{
    QzTableEntry **buckets; 
    qzuint capacity; 
    qzuint size; 
    qzuint elemSize; 
} QzTable;

void QzCreateTable(QzTable *self, qzuint elemSize); 

void QzDestroyTable(QzTable *self); 

qzbool QzTableGet(const QzTable *self, const char *key, void *value); 

qzbool QzTablePut(QzTable *self, const char *key, const void *value); 

qzbool QzTableRemove(QzTable *self, const char *key); 

void QzTableClear(QzTable *self); 

#endif 
