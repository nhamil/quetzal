#pragma once 

#ifndef QZCOMMON_H 
#define QZCOMMON_H 

#include <stddef.h> 
#include <stdint.h> 

#define QZ_ASSERT(x) do { if (!(x)) QzThrowError(QZ_ASSERTION_FAILED); } while (0)  
#define QZ_ASSERT_VAL(x, val) do { if (!(x)) { QzThrowError(QZ_ASSERTION_FAILED); return val; } } while (0)  

#define QZ_ASSERT_MSG(x, msg) do { if (!(x)) QzThrowMsgError(QZ_ASSERTION_FAILED, msg); } while (0)  
#define QZ_ASSERT_VAL_MSG(x, val, msg) do { if (!(x)) { QzThrowMsgError(QZ_ASSERTION_FAILED, msg); return val; } } while (0)  

#define QZ_TRUE (1) 
#define QZ_FALSE (0) 

typedef int8_t qzbool; 

typedef int8_t qzbyte; 
typedef int16_t qzshort; 
typedef int32_t qzint; 
typedef int64_t qzlong; 

typedef uint8_t qzubyte; 
typedef uint16_t qzushort; 
typedef uint32_t qzuint; 
typedef uint64_t qzulong; 

typedef uint64_t qzsize; 

typedef float qzfloat; 
typedef double qzdouble; 

#include "Qz/QzError.h" 

#endif 
