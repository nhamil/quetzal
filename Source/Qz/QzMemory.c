#include "Qz/QzMemory.h" 

#include <stdio.h> 
#include <stdlib.h> 

void *QzMalloc(qzsize num, qzsize size) 
{
    QZ_ASSERT_VAL_MSG(num && size, NULL, "Tried to allocate 0 bytes"); 

    void *m = malloc(num * size); 

    if (m) 
    {
        return m; 
    }
    else 
    {
        QzThrowError(QZ_OUT_OF_MEMORY); 
        return NULL; 
    }
}

void *QzCalloc(qzsize num, qzsize size) 
{
    QZ_ASSERT_VAL_MSG(num && size, NULL, "Tried to allocate 0 bytes"); 

    void *m = calloc(num, size); 

    if (m) 
    {
        return m; 
    }
    else 
    {
        QzThrowError(QZ_OUT_OF_MEMORY); 
        return NULL; 
    }
}

void *QzRealloc(void *mem, qzsize num, qzsize size) 
{
    QZ_ASSERT_VAL_MSG(num && size, NULL, "Tried to reallocate 0 bytes"); 

    void *m = realloc(mem, num * size); 

    if (m) 
    {
        return m; 
    }
    else 
    {
        QzThrowError(QZ_OUT_OF_MEMORY); 
        return NULL; 
    }
}

void QzFree(void *mem) 
{
    QZ_ASSERT_MSG(mem, "Tried to free null pointer"); 

    if (mem) free(mem); 
}
