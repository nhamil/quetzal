#pragma once 

#ifndef QZENV_H 
#define QZENV_H 

#include "Qz/QzCommon.h" 
#include "Qz/QzTable.h" 
#include "Qz/QzValue.h" 

typedef struct _qzenv QzEnv; 

struct _qzenv 
{
    QzEnv *parent; 
    QzTable names; 
    QzTable levels; 
    qzuint refCount; 
};

QzEnv *QzNewEnv(QzEnv *parent); 

void QzFreeEnv(QzEnv *self); 

void QzEnvAddRef(QzEnv *self); 

void QzEnvReleaseRef(QzEnv *self); 

QzValue QzEnvLoadName(QzEnv *self, const char *name); 

qzbool QzEnvStoreName(QzEnv *self, const char *name, QzValue value); 

void QzEnvStoreLocal(QzEnv *self, const char *name, QzValue value); 

#endif 
