#include <stdio.h> 
#include <string.h> 

#include <Qz/QzBlock.h> 
#include <Qz/QzMemory.h> 
#include <Qz/QzObject.h> 
#include <Qz/QzTable.h> 
#include <Qz/QzValue.h> 
#include <Qz/QzVM.h> 

qzubyte code[] = 
{
    QZ_OP_LOAD_CONST, 0, // 3.14
    QZ_OP_LOAD_CONST, 1, // 3.14, 2
    QZ_OP_ADD, // 5.14
    QZ_OP_LOAD_CONST, 1, // 5.14, 2
    QZ_OP_POW, // 26.4196
    QZ_OP_LOAD_CONST, 2, // 26.4196, 'hello' 
    QZ_OP_ADD,  // '26.4196hello'
    QZ_OP_STORE_LOCAL, 0, // []
    QZ_OP_PUSH_ENV, 
    QZ_OP_LOAD_CONST, 0, // 3.14 
    QZ_OP_STORE_LOCAL, 0, 
    QZ_OP_LOAD_NAME, 0, // '26.4196hello'
    QZ_OP_POP_ENV, 
    QZ_OP_PRINT, // [] 

    QZ_OP_MAKE_FUNCTION, 0, 5, // 0 arg, 4 bytes
        QZ_OP_LOAD_CONST, 3, // 1.414 
        QZ_OP_PRINT,
        QZ_OP_PUSH_NONE,  
        QZ_OP_RETURN, 

    QZ_OP_STORE_LOCAL, 1, // function into 'name1
    QZ_OP_LOAD_NAME, 1, // function 
    QZ_OP_CALL, 
    QZ_OP_POP, 
    0
};

QzValue consts[128]; 

const char *names[128]; 

static void PrintValue(QzValue a) 
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

int main(int argc, char **argv) 
{
    int i; 

    consts[0] = QzMakeValueNumber(3.14); 
    consts[1] = QzMakeValueNumber(2); 
    consts[2] = QzMakeValueString("hello"); 
    consts[3] = QzMakeValueNumber(1.414); 

    names[0] = "test"; 
    names[1] = "name1"; 

    QzVM *vm = QzNewVM(); 
    QzBlock block; 
    QzCreateBlock(&block); 

    for (i = 0; i < sizeof (code); i++) QzBlockWriteOp(&block, code[i], 0); 

    for (i = 0; i < sizeof (consts) / sizeof (QzValue); i++) QzBlockAddConstant(&block, consts[i]); 

    for (i = 0; names[i]; i++) QzBlockAddName(&block, names[i]); 

    QzVMRunBlock(vm, &block); 
    QzDestroyBlock(&block); 
    QzFreeVM(vm); 

    // printf("Env Test\n"); 

    // QzEnv *env = QzNewEnv(NULL); 
    // QzEnv *env2 = QzNewEnv(env); 
    // QzEnv *env3 = QzNewEnv(env2); 

    // QzEnvStoreName(env, "asdf", QzMakeValueNumber(2)); 

    // PrintValue(QzEnvLoadName(env, "asd")); // invalid
    // PrintValue(QzEnvLoadName(env, "asdf")); // 2

    // PrintValue(QzEnvLoadName(env3, "asd")); // invalid 
    // PrintValue(QzEnvLoadName(env3, "asdf")); // 2

    // QzEnvStoreName(env2, "asdf", QzMakeValueNumber(3)); 
    // printf("----------------\n"); 

    // PrintValue(QzEnvLoadName(env, "asdf")); // 3
    // PrintValue(QzEnvLoadName(env2, "asdf")); // 3
    // PrintValue(QzEnvLoadName(env3, "asdf")); // 3

    // QzEnvStoreLocal(env2, "asdf", QzMakeValueNumber(4)); 
    // printf("----------------\n"); 

    // PrintValue(QzEnvLoadName(env, "asdf")); // 3
    // PrintValue(QzEnvLoadName(env2, "asdf")); // 4
    // PrintValue(QzEnvLoadName(env3, "asdf")); // 3

    // QzEnvStoreName(env3, "asdf", QzMakeValueNumber(5)); 
    // printf("----------------\n"); 

    // PrintValue(QzEnvLoadName(env, "asdf")); // 5
    // PrintValue(QzEnvLoadName(env2, "asdf")); // 4
    // PrintValue(QzEnvLoadName(env3, "asdf")); // 5

    // QzEnvStoreName(env2, "asd", QzMakeValueString("hello")); 
    // QzEnvStoreName(env, "asd", QzMakeValueString("hello1")); 
    // QzEnvStoreName(env3, "asd", QzMakeValueString("hello2")); 
    // QzEnvStoreLocal(env3, "asd", QzMakeValueString("hello3")); 
    // printf("----------------\n"); 

    // PrintValue(QzEnvLoadName(env, "asdf")); // 5
    // PrintValue(QzEnvLoadName(env2, "asdf")); // 4
    // PrintValue(QzEnvLoadName(env3, "asdf")); // 5
    // PrintValue(QzEnvLoadName(env, "asd")); // hello1
    // PrintValue(QzEnvLoadName(env2, "asd")); // hello2
    // PrintValue(QzEnvLoadName(env3, "asd")); // hello3

    // QzFreeEnv(env3); 
    // QzFreeEnv(env2); 
    // QzFreeEnv(env); 

    // printf("Done\n"); 

    return 0; 
}
