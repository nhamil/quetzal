#include "Qz/QzTable.h" 

#include "Qz/QzMemory.h" 

#include <stdio.h> 
#include <string.h> 

#define QZ_START_CAP (17) 
#define QZ_NEXT_CAP(x) (((x) - 1) * 2 + 1) 

#define QZ_MAX_LOAD_FACTOR (0.667) 

struct _qztableentry 
{
    QzTableEntry *next; 
    char *key; 
    char *value; 
    qzuint hash; 
};

static qzuint Hash(const char *key) 
{
    qzulong i; 
    qzuint hash = 0x811c9dc5; 

    if (!key) return hash; 

    while (*key) 
    {
        hash ^= *key; 
        hash *= 16777619; 

        key++; 
    }

    return hash; 
}

static QzTableEntry *NewEntry(const char *key, qzuint hash, qzuint valueSize, const void *value)  
{
    QzTableEntry *self = QzMalloc(1, sizeof (QzTableEntry)); 

    qzuint len = strlen(key); 
    char *keyCpy = QzMalloc(len + 1, 1); 
    memcpy(keyCpy, key, len); 
    keyCpy[len] = 0; 

    char *valueCpy = QzMalloc(1, valueSize); 
    if (value) memcpy(valueCpy, value, valueSize); 
    else memset(valueCpy, 0, valueSize); 

    self->next = NULL; 
    self->key = keyCpy; 
    self->value = valueCpy; 
    self->hash = hash; 
    return self; 
}

static void FreeEntries(QzTableEntry *self) 
{
    QzTableEntry *cur = self; 

    while (cur) 
    {
        QzTableEntry *next = cur->next; 
        QzFree(cur->key); 
        QzFree(cur->value); 
        QzFree(cur); 
        cur = next; 
    }
}

static qzbool EntryEqual(QzTableEntry *self, const char *key, qzuint hash) 
{
    if (self->hash != hash) return QZ_FALSE; 

    return strcmp(self->key, key) == 0; 
}

static QzTableEntry **ResizeBuckets(QzTableEntry **buckets, qzuint oldCap, qzuint newCap) 
{
    QzTableEntry **newBuckets = QzCalloc(newCap, sizeof (QzTableEntry *)); 

    if (!buckets) return newBuckets; 

    printf("Growing buckets from %d to %d\n", oldCap, newCap); 

    qzuint i; 
    for (i = 0; i < oldCap; i++) 
    {
        QzTableEntry *entry = buckets[i]; 
        
        while (entry) 
        {
            QzTableEntry *next = entry->next; 
            qzuint bucket = entry->hash % newCap; 

            printf("-- Reinserting '%s'\n", entry->key); 

            entry->next = newBuckets[bucket]; 
            newBuckets[bucket] = entry; 

            entry = next; 
        }
    }

    QzFree(buckets); 
    return newBuckets; 
}

static void FreeBuckets(QzTableEntry **buckets, qzuint cap) 
{
    qzuint i; 
    for (i = 0; i < cap; i++) 
    {
        FreeEntries(buckets[i]); 
    }
    QzFree(buckets); 
}

static void FindEntries(QzTableEntry *start, const char *key, qzuint hash, QzTableEntry **parent, QzTableEntry **out) 
{
    QzTableEntry *last = NULL; 
    QzTableEntry *entry = start; 

    while (entry) 
    {
        if (EntryEqual(entry, key, hash)) 
        {
            if (parent) *parent = last; 
            *out = entry; 
            return; 
        }

        last = entry; 
        entry = entry->next; 
    }

    if (parent) *parent = NULL; 
    *out = NULL; 
}

static void TryResize(QzTable *self) 
{
    if ((qzdouble) self->size / self->capacity > QZ_MAX_LOAD_FACTOR) 
    {
        qzuint cap = self->capacity; 
        qzuint newCap = QZ_NEXT_CAP(cap); 
        self->buckets = ResizeBuckets(self->buckets, cap, newCap); 
        self->capacity = newCap; 
    }
}

void QzCreateTable(QzTable *self, qzuint elemSize) 
{
    QZ_ASSERT(self); 
    QZ_ASSERT(elemSize); 

    self->elemSize = elemSize; 
    self->size = 0; 
    self->capacity = QZ_START_CAP; 
    self->buckets = ResizeBuckets(NULL, 0, QZ_START_CAP); 
}

void QzDestroyTable(QzTable *self) 
{
    QZ_ASSERT(self); 

    FreeBuckets(self->buckets, self->capacity); 
}

qzbool QzTableGet(const QzTable *self, const char *key, void *value) 
{
    QZ_ASSERT_VAL(self && key, QZ_FALSE); 

    qzuint hash = Hash(key); 
    qzuint bucket = hash % self->capacity; 

    QzTableEntry *entry; 
    FindEntries(self->buckets[bucket], key, hash, NULL, &entry); 

    if (entry) 
    {
        if (value) memcpy(value, entry->value, self->elemSize); 
        return QZ_TRUE; 
    }
    else return QZ_FALSE; 
}

qzbool QzTablePut(QzTable *self, const char *key, const void *value) 
{
    QZ_ASSERT_VAL(self && key, QZ_FALSE); 

    qzuint hash = Hash(key); 
    qzuint bucket = hash % self->capacity; 

    QzTableEntry *entry; 
    FindEntries(self->buckets[bucket], key, hash, NULL, &entry); 

    if (entry) 
    {
        if (value) memcpy(entry->value, value, self->elemSize); 
        else memset(entry->value, 0, self->elemSize); 
        return QZ_FALSE; 
    }
    else 
    {
        // create new entry 
        self->size++; 
        TryResize(self); 
        QzTableEntry *newEntry = NewEntry(key, hash, self->elemSize, value); 
        newEntry->next = self->buckets[bucket]; 
        self->buckets[bucket] = newEntry; 
        return QZ_TRUE; 
    }
}

qzbool QzTableRemove(QzTable *self, const char *key) 
{
    QZ_ASSERT_VAL(self && key, QZ_FALSE); 

    qzuint hash = Hash(key); 
    qzuint bucket = hash % self->capacity; 

    QzTableEntry *parent; 
    QzTableEntry *entry; 
    FindEntries(self->buckets[bucket], key, hash, &parent, &entry); 

    if (entry) 
    {
        self->size--; 

        if (parent) 
        {
            parent->next = entry->next; 
        }
        else 
        {
            self->buckets[bucket] = entry->next; 
        }

        entry->next = NULL; 
        FreeEntries(entry); 

        return QZ_TRUE; 
    }
    else return QZ_FALSE; 
}

void QzTableClear(QzTable *self) 
{
    QZ_ASSERT(self); 

    self->size = 0; 

    qzuint i; 
    for (i = 0; i < self->capacity; i++) 
    {
        FreeEntries(self->buckets[i]); 
        self->buckets[i] = NULL; 
    }
}
