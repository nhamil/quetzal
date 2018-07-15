#include "Qz/QzBuffer.h" 

#include "Qz/QzMemory.h" 

#include <string.h> 

static inline char *Index(const QzBuffer *self, qzuint index) 
{
    return ((char *) self->data) + index * self->elemSize; 
}

void QzCreateBuffer(QzBuffer *self, qzulong elemSize) 
{
    QZ_ASSERT(self && elemSize); 

    self->data = QzMalloc(16, elemSize); 
    self->elemSize = elemSize; 
    self->size = 0; 
    self->capacity = 16; 
}

void QzDestroyBuffer(QzBuffer *self) 
{
    QZ_ASSERT(self); 

    QzFree(self->data); 
}

qzuint QzBufferWrite(QzBuffer *self, const void *data) 
{
    QZ_ASSERT_VAL(self, 0); 

    while (self->size >= self->capacity) 
    {
        self->capacity *= 2; 
        self->data = QzRealloc(self->data, self->capacity, self->elemSize); 
    }

    qzuint index = self->size++; 

    if (data) memcpy(Index(self, index), data, self->elemSize); 
    else memset(Index(self, index), 0, self->elemSize); 

    return index; 
}

qzbool QzBufferGet(const QzBuffer *self, qzuint index, void *data) 
{
    QZ_ASSERT_VAL(self, QZ_FALSE); 

    if (index >= self->size) return QZ_FALSE; 

    if (data) memcpy(data, Index(self, index), self->elemSize); 

    return QZ_TRUE; 
}