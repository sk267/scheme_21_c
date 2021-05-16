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
    env->value.scmEnv.keyValuePairs = (scmObject *)calloc(inCapacitiy, sizeof(scmObject));
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
    TOP_ENV = allocateEnvironment(1);
}

void growEnvironment(scmObject env)
{
    // Alte Bindings aus der Tabelle herausholen,
    // die Werte in die neue Tabelle rein hashen
    // Müsste mit setEnvironmentValue gehen
    // Über alle Werte drüber gehen, wenn es != NULL ist, dann setEnvironmentValue aufrufen
    // Die alten Bindings löschen

    printf("Environment: growEnvirtonment betreten!\n");

    scmObject key;
    scmObject value;
    scmObject *oldBindings = env->value.scmEnv.keyValuePairs;

    int oldCapacity = env->value.scmEnv.capacity;
    int newCapacity = oldCapacity * 2 + 1;
    scmObject *newBindings = (scmObject *)calloc(newCapacity, sizeof(scmObject));

    env->value.scmEnv.keyValuePairs = newBindings;
    printf("Environment: newCapacity: %d\n", newCapacity);
    env->value.scmEnv.capacity = newCapacity;

    for (int i = 0; i < oldCapacity; i++)
    {

        printf("------------------ i begin:%d\n", i);
        if (oldBindings[i] != NULL)
        {
            key = oldBindings[i]->value.scmCons.car;
            value = oldBindings[i]->value.scmCons.cdr;

            printf("Environment: for: key: \n");
            scm_print(key);
            printf("\n");
            printf("Environment: for: value: \n");
            scm_print(value);
            printf("\n");

            setEnvironmentValue(key, value, env);
        }
        printf("------------------ i finished:%d\n\n\n", i);
    }

    free(oldBindings);
}

void setEnvironmentValue(scmObject key, scmObject value, scmObject env)
{
    // Returns 1 if element was added
    // Return 0 if the table size stays the same
    // Idee: einfach über das Env checken, ob wir schon ein Grow machen müssen!?

    if (env->value.scmEnv.capacity == env->value.scmEnv.nVariables)
    {
        growEnvironment(env);
    }

    // An der entsprechenden Stelle kann man newCons(key, value) aufrufen!
    int index = hashForEnv(key, env);

    // Prüfen, ob an dieser Stelle überhaupt platz ist.
    while (env->value.scmEnv.keyValuePairs[index] != NULL)
    {
        printf("setEnvironmentValue: idx: %d already full!! Add one\n", index);
        index++;
    }

    index = index % env->value.scmEnv.capacity;
    env->value.scmEnv.keyValuePairs[index] = newCons(key, value);
    env->value.scmEnv.nVariables += 1;
}

scmObject getEnvironmentValue(scmObject key, scmObject env)
{
    int index = hashForEnv(key, env);

    // Prüfen, habe ich wirklich den richtigen Key?
    while (env->value.scmEnv.keyValuePairs[index]->value.scmCons.car != key)
    {
        index++;
    }
    index = index % env->value.scmEnv.capacity;
    return env->value.scmEnv.keyValuePairs[index]->value.scmCons.cdr;
}