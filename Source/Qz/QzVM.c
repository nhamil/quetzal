#include "Qz/QzVM.h" 

#include "Qz/QzMemory.h" 
#include "Qz/QzObject.h" 
#include "Qz/QzScanner.h" 
#include "Qz/QzValue.h" 

#include <math.h> 
#include <stdio.h> 
#include <string.h> 

#define QZ_PUSH(x) self->stack[self->sp++] = x 
#define QZ_POP() self->stack[--self->sp] 
#define QZ_PEEK() self->stack[self->sp - 1] 

static inline void DoubleToString(char *str, qzuint size, qzdouble num) 
{
    snprintf(str, size, "%g", num); 
}

static const char *ConcatStrings(const char *a, const char *b) 
{
    qzuint len1 = strlen(a); 
    qzuint len2 = strlen(b); 
    char *str = QzMalloc(len1 + len2 + 1, 1); 
    memcpy(str, a, len1); 
    memcpy(&str[len1], b, len2); 
    str[len1 + len2] = 0; 
    return str; 
}

static QzFrame MakeFrame(const QzBlock *block) 
{
    QzFrame frame; 
    frame.block = block; 
    frame.pc = 0; 
    return frame; 
}

static QzValue AddValues(QzVM *self, QzValue a, QzValue b) 
{
    if (a.type == QZ_VALUE_INVALID || b.type == QZ_VALUE_INVALID) return QzMakeValueInvalid(); 

    switch (a.type) 
    {
    case QZ_VALUE_NUMBER: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber(a.number + b.number); 
        case QZ_VALUE_STRING: 
            {
                char str[18]; 
                DoubleToString(str, 18, a.number); 
                return QzMakeValueString(ConcatStrings(str, b.string)); 
            }
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber(a.number + b.boolean ? 1 : 0); 
        }
        break; 
    case QZ_VALUE_STRING: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            {
                char str[18]; 
                DoubleToString(str, 18, b.number); 
                return QzMakeValueString(ConcatStrings(a.string, str)); 
            }
        case QZ_VALUE_STRING: 
            return QzMakeValueString(ConcatStrings(a.string, b.string)); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueString(ConcatStrings(a.string, b.boolean ? "true" : "false")); 
        }
        break; 
    case QZ_VALUE_BOOL: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) + b.number); 
        case QZ_VALUE_STRING: 
            return QzMakeValueString(ConcatStrings(a.boolean ? "true" : "false", b.string));
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) + (b.boolean ? 1 : 0)); 
        }
        break; 
    }

    return QzMakeValueInvalid(); 
}

static QzValue SubValues(QzVM *self, QzValue a, QzValue b) 
{
    if (a.type == QZ_VALUE_INVALID || b.type == QZ_VALUE_INVALID) return QzMakeValueInvalid(); 

    switch (a.type) 
    {
    case QZ_VALUE_NUMBER: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber(a.number - b.number); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber(a.number - b.boolean ? 1 : 0); 
        }
        break; 
    case QZ_VALUE_BOOL: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) - b.number); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) - (b.boolean ? 1 : 0)); 
        }
        break; 
    }

    return QzMakeValueInvalid(); 
}

static QzValue MulValues(QzVM *self, QzValue a, QzValue b) 
{
    if (a.type == QZ_VALUE_INVALID || b.type == QZ_VALUE_INVALID) return QzMakeValueInvalid(); 

    switch (a.type) 
    {
    case QZ_VALUE_NUMBER: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber(a.number * b.number); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber(a.number * b.boolean ? 1 : 0); 
        }
        break; 
    case QZ_VALUE_BOOL: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) * b.number); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) * (b.boolean ? 1 : 0)); 
        }
        break; 
    }

    return QzMakeValueInvalid(); 
}

static QzValue DivValues(QzVM *self, QzValue a, QzValue b) 
{
    if (a.type == QZ_VALUE_INVALID || b.type == QZ_VALUE_INVALID) return QzMakeValueInvalid(); 

    switch (a.type) 
    {
    case QZ_VALUE_NUMBER: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber(a.number / b.number); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber(a.number / b.boolean ? 1 : 0); 
        }
        break; 
    case QZ_VALUE_BOOL: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) / b.number); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber((a.boolean ? 1 : 0) / (b.boolean ? 1 : 0)); 
        }
        break; 
    }

    return QzMakeValueInvalid(); 
}

static QzValue PowValues(QzVM *self, QzValue a, QzValue b) 
{
    if (a.type == QZ_VALUE_INVALID || b.type == QZ_VALUE_INVALID) return QzMakeValueInvalid(); 

    switch (a.type) 
    {
    case QZ_VALUE_NUMBER: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber(pow(a.number, b.number)); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber(pow(a.number, b.boolean ? 1 : 0)); 
        }
        break; 
    case QZ_VALUE_BOOL: 
        switch (b.type) 
        {
        case QZ_VALUE_NUMBER: 
            return QzMakeValueNumber(pow((a.boolean ? 1 : 0), b.number)); 
        case QZ_VALUE_BOOL: 
            return QzMakeValueNumber(pow((a.boolean ? 1 : 0), (b.boolean ? 1 : 0))); 
        }
        break; 
    }

    return QzMakeValueInvalid(); 
}

static void PrintValue(QzVM *self, QzValue a) 
{
    printf("PRINT: "); 

    switch (a.type) 
    {
    case QZ_VALUE_NONE: 
        printf("[none]\n"); 
        break; 
    case QZ_VALUE_NUMBER: 
        printf("%g\n", a.number); 
        break; 
    case QZ_VALUE_STRING: 
        printf("%s\n", a.string); 
        break; 
    case QZ_VALUE_BOOL: 
        printf("%s\n", a.boolean ? "true" : "false"); 
        break; 
    case QZ_VALUE_OBJECT: 
        printf("[Object]\n"); 
        break; 
    default: 
    case QZ_VALUE_INVALID: 
        printf("[invalid]\n"); 
        break; 
    }
}

QzVM *QzNewVM(void) 
{
    QzVM *self = QzMalloc(1, sizeof (QzVM));

    return self;  
}

void QzFreeVM(QzVM *self) 
{
    QZ_ASSERT(self); 

    QzFree(self); 
}

qzbool QzVMRunFile(QzVM *self, const char *filename) 
{
    QZ_ASSERT_VAL(self && filename, QZ_FALSE); 

    FILE *file = fopen(filename, "r"); 
    if (file) 
    {
        fseek(file, 0, SEEK_END); 
        qzulong length = ftell(file); 
        rewind(file); 
        char *buffer = QzMalloc(length + 1, 1); 
        fread(buffer, 1, length, file); 
        buffer[length] = 0; 

        printf("SOURCE: \n%s\nEND\n", buffer); 

        QzVMRunCode(self, buffer); 
        QzFree(buffer); 

        return QZ_TRUE; 
    }
    else 
    {
        return QZ_FALSE; 
    }
}

void QzVMRunCode(QzVM *self, const char *code) 
{
    QZ_ASSERT(self && code); 

    printf("START SCAN\n"); 

    qzuint count = 0; 

    QzScanner scanner; 
    QzCreateScanner(&scanner, strlen(code), code); 

    QzToken token; 
    while (1) 
    {
        token = QzScannerNext(&scanner); 

        if (token.type == QZ_TOKEN_INVALID) 
        {
            printf("Unexpected token on line %d: '%.*s'\n", token.line, token.length, token.text); 
            break; 
        }
        else if (token.type == QZ_TOKEN_EOF) 
        {
            printf("Done\n"); 
            break; 
        }

        count++; 

        // printf("Type: %d, ", token.type); 
        if (token.type == QZ_TOKEN_STRING) 
        {
            printf("String     : \"%.*s\"\n", token.length, token.text); 
        }
        else if (token.type == QZ_TOKEN_IDENTIFIER) 
        {
            printf("Identifier : '%.*s'\n", token.length, token.text); 
        }
        else if (token.type == QZ_TOKEN_NUMBER) 
        {
            printf("Number     : '%.*s'\n", token.length, token.text); 
        }
        else 
        {
            printf("Token      : '%.*s'\n", token.length, token.text); 
        }
    }

    printf("END SCAN: %d\n", count); 
}

void QzVMRunBlock(QzVM *self, const QzBlock *block) 
{
    QZ_ASSERT(self && block); 

    self->fp = 0; 
    self->sp = 0; 
    self->pc = 0; 
    self->ep = 0; 
    self->block = block; 
    self->env = QzNewEnv(NULL); 

    QzOp op; 
    while (op = QzBlockGetOp(self->block, self->pc)) 
    {
        printf("%-3d %s ", self->pc++, QzOpName(op)); 

        switch (op) 
        {
        case QZ_OP_PUSH_NONE: 
            printf("\n"); 
            QZ_PUSH(QzMakeValueNone()); 
            break; 
        case QZ_OP_LOAD_CONST: 
            {
                qzuint index = QzBlockGetOp(self->block, self->pc++); 
                printf("%d\n", index); 
                QzValue value = QzBlockGetConstant(self->block, index); 
                QZ_PUSH(value); 
            }
            break; 
        case QZ_OP_ADD: 
            {
                printf("\n"); 
                QzValue b = QZ_POP(); 
                QzValue a = QZ_POP(); 
                QZ_PUSH(AddValues(self, a, b)); 
            }
            break; 
        case QZ_OP_SUB: 
            {
                printf("\n"); 
                QzValue b = QZ_POP(); 
                QzValue a = QZ_POP(); 
                QZ_PUSH(SubValues(self, a, b)); 
            }
            break; 
        case QZ_OP_MUL: 
            {
                printf("\n"); 
                QzValue b = QZ_POP(); 
                QzValue a = QZ_POP(); 
                QZ_PUSH(MulValues(self, a, b)); 
            }
            break; 
        case QZ_OP_DIV: 
            {
                printf("\n"); 
                QzValue b = QZ_POP(); 
                QzValue a = QZ_POP(); 
                QZ_PUSH(DivValues(self, a, b)); 
            }
            break; 
        case QZ_OP_POW: 
            {
                printf("\n"); 
                QzValue b = QZ_POP(); 
                QzValue a = QZ_POP(); 
                QZ_PUSH(PowValues(self, a, b)); 
            }
            break; 
        case QZ_OP_DUP: 
            printf("\n"); 
            QZ_PUSH(QZ_PEEK()); 
            break; 
        case QZ_OP_PRINT: 
            printf("\n"); 
            PrintValue(self, QZ_POP()); 
            break; 
        case QZ_OP_STORE_LOCAL: 
            {
                qzuint index = QzBlockGetOp(self->block, self->pc++); 
                const char *name = QzBlockGetName(self->block, index); 
                printf("%d // '%s'\n", index, name); 
                QzValue value = QZ_POP(); 
                QzEnvStoreLocal(self->env, name, value); 
            }
            break; 
        case QZ_OP_STORE_NAME: 
            {
                qzuint index = QzBlockGetOp(self->block, self->pc++); 
                const char *name = QzBlockGetName(self->block, index); 
                printf("%d // '%s'\n", index, name); 
                QzValue value = QZ_POP(); 
                QzEnvStoreName(self->env, name, value); 
            }
            break; 
        case QZ_OP_LOAD_NAME: 
            {
                qzuint index = QzBlockGetOp(self->block, self->pc++); 
                const char *name = QzBlockGetName(self->block, index); 
                printf("%d // '%s'\n", index, name); 
                QzValue value; 
                QZ_PUSH(QzEnvLoadName(self->env, name)); 
            }
            break; 
        case QZ_OP_PUSH_ENV: 
            {
                printf("\n"); 
                QzEnv *newEnv = QzNewEnv(self->env); 
                self->env = newEnv; 
            }
            break; 
        case QZ_OP_POP_ENV: 
            {
                printf("\n"); 
                QzEnv *lastEnv = self->env->parent; 
                QzEnvReleaseRef(self->env); 
                self->env = lastEnv; 
            }
            break; 
        case QZ_OP_MAKE_FUNCTION: 
            {
                qzuint args = QzBlockGetOp(self->block, self->pc++); 
                qzuint size = QzBlockGetOp(self->block, self->pc++); 
                printf("%d %d\n", args, size); 
                QzFunction *func = QzNewFunction(); 
                QzFunction f2; 
                qzuint i; 
                for (i = 0; i < size; i++) 
                {
                    QzBlockWriteOp(
                        &func->block, 
                        QzBlockGetOp(self->block, self->pc), 
                        QzBlockGetLine(self->block, self->pc++)
                    ); 
                }
                for (i = 0; i < self->block->constants.size; i++) 
                {
                    QzBlockAddConstant(
                        &func->block, 
                        QzBlockGetConstant(self->block, i) 
                    );
                }
                for (i = 0; i < self->block->names.size; i++) 
                {
                    QzBlockAddName(
                        &func->block, 
                        QzBlockGetName(self->block, i) 
                    );
                }
                QZ_PUSH(QzMakeValueObject(func)); 
            }
            break; 
        case QZ_OP_CALL: 
            {
                printf("\n"); 
                QzValue value = QZ_POP(); 
                switch (value.type) 
                {
                case QZ_VALUE_OBJECT: 
                    {
                        QzObject *obj = value.object; 

                        if (obj->type == QZ_OBJECT_FUNCTION) 
                        {
                            QzFunction *func = (QzFunction *) obj; 

                            QzFrame frame; 
                            frame.env = self->env; 
                            frame.pc = self->pc; 
                            frame.block = self->block; 
                            self->frames[self->fp++] = frame; 

                            self->env = QzNewEnv(self->env); 
                            self->pc = 0; 
                            self->block = &func->block; 

                            break; 
                        }
                    }
                default: 
                    printf("FATAL ERROR: CANNOT CALL OBJECT\n"); 
                    return; 
                }
            }
            break; 
        case QZ_OP_RETURN: 
            {
                printf("\n"); 
                QzFrame frame = self->frames[--self->fp]; 
                QzEnvReleaseRef(self->env); 
                self->env = frame.env; 
                self->pc = frame.pc; 
                self->block = frame.block; 
            }
            break; 
        case QZ_OP_POP: 
            printf("\n"); 
            QZ_POP(); 
            break; 
        default: 
            printf("\nFATAL ERROR: INVALID BYTECODE\n"); 
            return; 
        }
    }
}
