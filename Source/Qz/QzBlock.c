#include "Qz/QzBlock.h" 

#include "Qz/QzMemory.h" 

void QzCreateBlock(QzBlock *self) 
{
    QZ_ASSERT(self); 

    QzCreateBuffer(&self->constants, sizeof (QzValue)); 
    QzCreateBuffer(&self->names, sizeof (const char *)); 
    QzCreateBuffer(&self->opcodes, sizeof (qzubyte)); 
    QzCreateBuffer(&self->lines, sizeof (qzuint)); 
} 

void QzDestroyBlock(QzBlock *self) 
{
    QZ_ASSERT(self); 

    QzDestroyBuffer(&self->constants); 
    QzDestroyBuffer(&self->names); 
    QzDestroyBuffer(&self->opcodes); 
    QzDestroyBuffer(&self->lines); 
}

QzBlock *QzNewBlock(void)
{ 
    QzBlock *self = QzMalloc(1, sizeof (QzBlock)); 
    QzCreateBlock(self); 
    return self; 
}

void QzFreeBlock(QzBlock *self) 
{
    QZ_ASSERT(self); 
    QzDestroyBlock(self); 
    QzFree(self); 
}

QzOp QzBlockGetOp(const QzBlock *self, qzuint index) 
{
    QZ_ASSERT_VAL(self, QZ_OP_INVALID); 

    qzubyte op; 
    return QzBufferGet(&self->opcodes, index, &op) ? op : QZ_OP_INVALID; 
}

qzuint QzBlockGetLine(const QzBlock *self, qzuint index) 
{
    QZ_ASSERT_VAL(self, 0); 

    qzuint line; 
    return QzBufferGet(&self->lines, index, &line) ? line : 0; 
} 

QzValue QzBlockGetConstant(const QzBlock *self, qzuint index) 
{
    QZ_ASSERT_VAL(self, QzMakeValueInvalid()); 

    QzValue value; 
    return QzBufferGet(&self->constants, index, &value) ? value : QzMakeValueInvalid(); 
}

const char *QzBlockGetName(const QzBlock *self, qzuint index) 
{
    QZ_ASSERT_VAL(self, NULL); 

    const char *name; 
    return QzBufferGet(&self->names, index, &name) ? name : NULL; 
}

void QzBlockWriteOp(QzBlock *self, QzOp op, qzuint line) 
{
    QZ_ASSERT(self); 

    QzBufferWrite(&self->opcodes, &op); 
    QzBufferWrite(&self->lines, &line); 
}

qzuint QzBlockAddConstant(QzBlock *self, QzValue value) 
{
    QZ_ASSERT_VAL(self, 0); 

    return QzBufferWrite(&self->constants, &value); 
}

qzuint QzBlockAddName(QzBlock *self, const char *name) 
{
    QZ_ASSERT_VAL(self, 0); 
    
    return QzBufferWrite(&self->names, &name); 
}
