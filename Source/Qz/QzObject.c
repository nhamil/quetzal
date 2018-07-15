#include "Qz/QzObject.h" 

#include "Qz/QzMemory.h" 

void QzCreateObject(QzObject *self, QzObjectType type) 
{
    QZ_ASSERT(self); 

    self->type = type; 
}

void QzDestroyObject(QzObject *self) 
{
    QZ_ASSERT(self); 
}

QzFunction *QzNewFunction(void) 
{
    QzFunction *self = QzMalloc(1, sizeof (QzFunction)); 

    QzCreateObject(&self->obj, QZ_OBJECT_FUNCTION); 
    QzCreateBlock(&self->block); 

    return self; 
}

void QzFreeFunction(QzFunction *self) 
{
    QZ_ASSERT(self); 

    QzDestroyObject(&self->obj); 
    QzDestroyBlock(&self->block); 
    QzFree(self); 
}
