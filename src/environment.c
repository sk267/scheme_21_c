#include "scheme.h"

// #define ENV_DEBUG

#ifdef ENV_DEBUG
#define ENV_DEBUG_CODE(code) code
#else
#define ENV_DEBUG_CODE(code)
#endif

scmObject allocateEnvironment(int inCapacitiy)
{
    ENV_DEBUG_CODE({
        printf("Betrete allocate allocateEnvironment");
    })
    scmObject env = (scmObject)malloc(sizeof(struct scmObjectStruct));
    env->tag = TAG_ENV;
    env->value.scmEnv.nVariables = 0;
    env->value.scmEnv.capacity = inCapacitiy;
    // Hier auf (scmObject*) casten???
    env->value.scmEnv.keyValuePairs = (scmObject *)malloc(sizeof(scmObject) * inCapacitiy);
    printf("allocateEnvironment: capacity: %d\n", env->value.scmEnv.capacity);

    return env;
}

int hashForEnv(scmObject symbol, scmObject env)
{
    int myHash;
    myHash = (intptr_t)symbol * 7;
    myHash = myHash % env->value.scmEnv.capacity;
    ENV_DEBUG_CODE({
        printf("hashForEnv: myHash %d\n", myHash);
    })
    return myHash;
}

void initializeTopEnv()
{
    TOP_ENV = allocateEnvironment(100);
}

void growEnvironment(scmObject env)
{
    // Alte Bindings aus der Tabelle herausholen,
    // die Werte in die neue Tabelle rein hashen
}

void setEnvironmentValue(scmObject key, scmObject value, scmObject env)
{
    // Returns 1 if element was added
    // Return 0 if the table size stays the same
    // Idee: einfach über das Env checken, ob wir schon ein Grow machen müssen!?

    // An der entsprechenden Stelle kann man newCons(key, value) aufrufen!
    int index = hashForEnv(key, env);
    env->value.scmEnv.keyValuePairs[index] = newCons(key, value);
}

scmObject getEnvironmentValue(scmObject key, scmObject env)
{
    int index = hashForEnv(key, env);
    return env->value.scmEnv.keyValuePairs[index]->value.scmCons.cdr;
}