#include "Qz/QzEnv.h" 

#include "Qz/QzMemory.h" 

static void CreateEnv(QzEnv *self, QzEnv *parent) 
{
    self->parent = parent; 
    if (self->parent) QzEnvAddRef(self->parent); 
    
    QzCreateTable(&self->names, sizeof (QzValue)); 
    QzCreateTable(&self->levels, sizeof (QzEnv *)); 
    self->refCount = 1; 
}

static void DestroyEnv(QzEnv *self) 
{
    QzDestroyTable(&self->names); 
    QzDestroyTable(&self->levels); 
}

static qzbool FindLevel(QzEnv *self, const char *name, QzEnv **out) 
{ 
    QzEnv *cur; 

    if (QzTableGet(&self->levels, name, &cur)) 
    {
        if (out) *out = cur; 
        return QZ_TRUE; 
    }

    cur = self; 

    while (cur) 
    {
        if (QzTableGet(&cur->names, name, NULL)) 
        {
            QzTablePut(&self->levels, name, &cur); 
            if (out) *out = cur; 
            return QZ_TRUE; 
        }

        cur = cur->parent; 
    }

    return QZ_FALSE; 
}

QzEnv *QzNewEnv(QzEnv *parent) 
{
    QzEnv *env = QzMalloc(1, sizeof (QzEnv)); 
    CreateEnv(env, parent); 
    return env; 
}

void QzFreeEnv(QzEnv *self) 
{
    QZ_ASSERT(self); 

    DestroyEnv(self); 
    QzFree(self); 
}

void QzEnvAddRef(QzEnv *self) 
{
    QZ_ASSERT(self); 
    self->refCount++; 
}

void QzEnvReleaseRef(QzEnv *self) 
{
    QZ_ASSERT(self); 
    self->refCount--; 

    if (!self->refCount) 
    {
        QzFreeEnv(self); 
    }
}

QzValue QzEnvLoadName(QzEnv *self, const char *name) 
{
    QZ_ASSERT_VAL(self && name, QzMakeValueInvalid()); 

    QzValue out; 
    QzEnv *level; 

    if (FindLevel(self, name, &level) && QzTableGet(&level->names, name, &out))
    {
        return out; 
    }

    return QzMakeValueInvalid(); 
} 

qzbool QzEnvStoreName(QzEnv *self, const char *name, QzValue value) 
{
    QZ_ASSERT_VAL(self && name, QZ_FALSE); 

    QzEnv *level; 
    
    if (FindLevel(self, name, &level)) 
    {
        QzEnvStoreLocal(level, name, value); 
    }
    else 
    {
        QzEnvStoreLocal(self, name, value); 
    }
}

void QzEnvStoreLocal(QzEnv *self, const char *name, QzValue value) 
{
    QZ_ASSERT(self && name); 

    QzTablePut(&self->names, name, &value); 
    QzTablePut(&self->levels, name, &self); 
}
