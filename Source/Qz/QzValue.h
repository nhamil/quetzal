#pragma once 

#ifndef QZVALUE_H 
#define QZVALUE_H 

#include "Qz/QzCommon.h" 

typedef enum _qzvaluetype 
{
    QZ_VALUE_INVALID = 0, 
    QZ_VALUE_NONE, 
    QZ_VALUE_NUMBER, 
    QZ_VALUE_STRING, 
    QZ_VALUE_BOOL, 
    QZ_VALUE_OBJECT 
} QzValueType;

typedef struct _qzvalue 
{
    QzValueType type; 
    union 
    {
        qzdouble number; 
        const char *string; 
        qzbool boolean; 
        void *object; 
    };
} QzValue;

static inline QzValue QzMakeValueInvalid(void) 
{
    QzValue value; 
    value.type = QZ_VALUE_INVALID; 
    value.boolean = QZ_FALSE; 
    return value; 
}

static inline QzValue QzMakeValueNone() 
{
    QzValue value; 
    value.type = QZ_VALUE_NONE; 
    value.object = NULL; 
    return value; 
}

static inline QzValue QzMakeValueBool(qzbool boolean) 
{
    QzValue value; 
    value.type = QZ_VALUE_BOOL; 
    value.boolean = boolean; 
    return value; 
}

static inline QzValue QzMakeValueNumber(qzdouble number) 
{
    QzValue value; 
    value.type = QZ_VALUE_NUMBER; 
    value.number = number; 
    return value; 
}

static inline QzValue QzMakeValueString(const char *string) 
{
    QZ_ASSERT_VAL(string, QzMakeValueInvalid()); 

    QzValue value; 
    value.type = QZ_VALUE_STRING; 
    value.string = string; 
    return value; 
}

static inline QzValue QzMakeValueObject(void *object) 
{
    QZ_ASSERT_VAL(object, QzMakeValueInvalid()); 

    QzValue value; 
    value.type = QZ_VALUE_OBJECT; 
    value.object = object; 
    return value; 
}

#endif 
