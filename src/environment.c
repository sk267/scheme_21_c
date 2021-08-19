#include "scheme.h"

// #define ENV_DEBUG

#ifdef ENV_DEBUG
#define ENV_DEBUG_CODE(code) code
#else
#define ENV_DEBUG_CODE(code)
#endif

scmObject allocateEnvironment(int inCapacitiy, scmObject parEnv)
{
    ENV_DEBUG_CODE({
        printf("Betrete allocate allocateEnvironment");
    })
    scmObject env = (scmObject)malloc(sizeof(struct scmObjectStruct));
    env->tag = TAG_ENV;
    env->value.scmEnv.nVariables = 0;
    env->value.scmEnv.capacity = inCapacitiy;
    env->value.scmEnv.parentEnv = parEnv;
    // Hier auf (scmObject*) casten???
    env->value.scmEnv.keyValuePairs = (scmObject *)calloc(inCapacitiy, sizeof(scmObject));

    ENV_DEBUG_CODE({
        printf("allocateEnvironment: capacity: %d\n", env->value.scmEnv.capacity);
    })

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
    TOP_ENV = allocateEnvironment(50, SCM_NULL);
}

void growEnvironment(scmObject env)
{
    // Alte Bindings aus der Tabelle herausholen,
    // die Werte in die neue Tabelle rein hashen
    // Müsste mit defineEnvironmentValue gehen
    // Über alle Werte drüber gehen, wenn es != NULL ist, dann defineEnvironmentValue aufrufen
    // Die alten Bindings löschen

    ENV_DEBUG_CODE({
        printf("Environment: growEnvirtonment betreten!\n");
    })

    scmObject key;
    scmObject value;
    scmObject *oldBindings = env->value.scmEnv.keyValuePairs;

    int oldCapacity = env->value.scmEnv.capacity;
    int newCapacity = oldCapacity * 2 + 1;
    scmObject *newBindings = (scmObject *)calloc(newCapacity, sizeof(scmObject));

    env->value.scmEnv.keyValuePairs = newBindings;
    ENV_DEBUG_CODE({
        printf("Environment: newCapacity: %d\n", newCapacity);
    })
    env->value.scmEnv.capacity = newCapacity;

    for (int i = 0; i < oldCapacity; i++)
    {

        // printf("------------------ i begin:%d\n", i);
        if (oldBindings[i] != NULL)
        {
            key = oldBindings[i]->value.scmCons.car;
            value = oldBindings[i]->value.scmCons.cdr;

            // printf("Environment: for: key: \n");
            // scm_print(key);
            // printf("\n");
            // printf("Environment: for: value: \n");
            // scm_print(value);
            // printf("\n");

            defineEnvironmentValue(key, value, env);
        }
        // printf("------------------ i finished:%d\n\n\n", i);
    }

    free(oldBindings);
}

void setOrDefineEnvironmentValue(scmObject key, scmObject value, scmObject env, bool scmDefine)
{
    int initialIndex;
    int capacity;

    ENV_DEBUG_CODE(
        {
            printf("++++++++++++++++++++++++++++++++++++++++\n");
            printf("setOrDefineEnvironmentValue: key: ");
            scm_print(key);
            printf(" value: ");
            scm_print(value);
            printf("\n---------------------------------------\n");
        })

    if (env->value.scmEnv.capacity == env->value.scmEnv.nVariables)
    {
        growEnvironment(env);
    }

    int index = hashForEnv(key, env);
    initialIndex = index;
    capacity = env->value.scmEnv.capacity;

    // Prüfen, ob an dieser Stelle überhaupt platz ist.
    while (true)
    {
        if (env->value.scmEnv.keyValuePairs[index] != NULL)
        {
            // Slot ist nicht frei!
            if (env->value.scmEnv.keyValuePairs[index]->value.scmCons.car == key)
            {
                // Diese Symbol existiert bereits, wir müssen nur den cdr hier ändern
                // und die Methode verlassen
                env->value.scmEnv.keyValuePairs[index]->value.scmCons.cdr = value;
                return;
            }
            else
            {
                // Symbol noch nicht gefunden, wir müssen weitersuchen
                index++;
            }
        }
        else if (env->value.scmEnv.keyValuePairs[index] == NULL)
        {
            // Slot ist frei, evt. hier eintragen

            if (!scmDefine)
            {
                // Eintragen ist nur erlaubt, wenn define aufgerufen wurde!
                // Bei set! können nur vorhandene variablen überschrieben werden
                scmError("set! is only allowed to override existing variables! Use define instead!");
            }

            // printf("free slot found\n");
            index = index % capacity;
            env->value.scmEnv.keyValuePairs[index] = newCons(key, value);
            env->value.scmEnv.nVariables += 1;
            return;
        }

        if (initialIndex == (index % capacity))
        {
            // Es gab überhaupt keine freien Slot mehr
            scmError("defineEnvironmentValue: There was no free slot, FATAL");
        }
    }
}

void defineEnvironmentValue(scmObject key, scmObject value, scmObject env)
{
    setOrDefineEnvironmentValue(key, value, env, true);
}

void setEnvironmentValue(scmObject key, scmObject value, scmObject env)
{
    setOrDefineEnvironmentValue(key, value, env, false);
}

scmObject getEnvironmentValue(scmObject key, scmObject env)
{

    // printf("Betrete getEnvironmentValue\n");
    int index = hashForEnv(key, env);
    int initialIndex = index;
    int capacity = env->value.scmEnv.capacity;

    // printf("getEnvironmentValue: initialIndex: %d\n", initialIndex);
    // printf("getEnvironmentValue: initialIndex mod capaxity: %d\n", (initialIndex % capacity));

    // Prüfen, habe ich wirklich den richtigen Key?
    while (true)
    {

        if (env->value.scmEnv.keyValuePairs[(index % capacity)] == NULL)
        {
            // Leerer Platz -> weitersuchen
            index++;
        }
        else if (env->value.scmEnv.keyValuePairs[(index % capacity)]->value.scmCons.car != key)
        {
            // Nicht der richtige Key -> weitersuchen
            index++;
        }
        else if (env->value.scmEnv.keyValuePairs[(index % capacity)]->value.scmCons.car == key)
        {
            // Key gefunden -> wert zurückgeben
            index = index % capacity;
            return env->value.scmEnv.keyValuePairs[index]->value.scmCons.cdr;
        }
        if (initialIndex == (index % capacity))
        {
            // Wir sind wieder beim ersten Index -> Den Key scheint es noch nicht zu geben!
            ENV_DEBUG_CODE({
                printf("\n\n!!!!FATAL:getEnvironmentValue:  wieder beim ersten Index herausgekommen!! \n\n");
            })
            return SCM_NULL;
        }
    }
    return SCM_NULL;
}