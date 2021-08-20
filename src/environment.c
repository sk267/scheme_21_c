#include "scheme.h"

// #define ENV_DEBUG

#ifdef ENV_DEBUG
#define ENV_DEBUG_CODE(code) code
#else
#define ENV_DEBUG_CODE(code)
#endif

scmObject allocateEnvironment(int inCapacitiy, scmObject parEnv)
{
    scmObject env = (scmObject)malloc(sizeof(struct scmObjectStruct));
    env->tag = TAG_ENV;
    env->value.scmEnv.nVariables = 0;
    env->value.scmEnv.capacity = inCapacitiy;
    env->value.scmEnv.parentEnv = parEnv;
    // Hier auf (scmObject*) casten???
    env->value.scmEnv.keyValuePairs = (scmObject *)calloc(inCapacitiy, sizeof(scmObject));
    ENV_DEBUG_CODE(
        {
            printf("++++++++++++++++++++++++++++++++++++\n");
            printf("++++++++++++++++++++++++++++++++++++\n");
            printf("Betrete allocate allocateEnvironment");
            printf("inCapaicity: %d\n", inCapacitiy);
            printf("inCapaicity parent: %d\n", parEnv->value.scmEnv.capacity);
            printf("-------------------------------------\n");
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
    int index;
    int capacity;

    if (env->value.scmEnv.capacity == env->value.scmEnv.nVariables)
    {
        growEnvironment(env);
    }

    initialIndex = hashForEnv(key, env);
    capacity = env->value.scmEnv.capacity;
    initialIndex = initialIndex % capacity;
    index = initialIndex;

    // Prüfen, ob an dieser Stelle überhaupt platz ist.
    while (true)
    {
        if (env->value.scmEnv.keyValuePairs[index % capacity] != NULL)
        {
            // Slot ist nicht frei!
            if (env->value.scmEnv.keyValuePairs[index % capacity]->value.scmCons.car == key)
            {
                // Diese Symbol existiert bereits, wir müssen nur den cdr hier ändern
                // und die Methode verlassen
                env->value.scmEnv.keyValuePairs[index % capacity]->value.scmCons.cdr = value;
                return;
            }
            else
            {
                // Symbol noch nicht gefunden, wir müssen weitersuchen
                index++;
            }
        }
        else if (env->value.scmEnv.keyValuePairs[index % capacity] == NULL)
        {
            // Slot ist frei, evt. hier eintragen

            if (!scmDefine)
            {
                // Eintragen ist nur erlaubt, wenn define aufgerufen wurde!
                // Bei set! können nur vorhandene variablen überschrieben werden
                scmError("set! is only allowed to override existing variables! Use define instead!");
            }

            index = index % capacity;
            ENV_DEBUG_CODE({
                printf("free slot found at: %d\n", index);
            })
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

scmObject getEnvironmentValue(scmObject key, scmObject inEnv)
{

    scmObject env = inEnv;

    // printf("Betrete getEnvironmentValue\n");
    int initialIndex = hashForEnv(key, env);
    int capacity = env->value.scmEnv.capacity;

    initialIndex = initialIndex % capacity;
    int index = initialIndex;

    // printf("getEnvironmentValue: initialIndex: %d\n", initialIndex);
    // printf("getEnvironmentValue: initialIndex mod capaxity: %d\n", (initialIndex % capacity));

    // Suche im inEnv sowie allen parent Environments:
    while (env != SCM_NULL)
    {
        // Suche mithilfe des Hashs nach dem Value
        ENV_DEBUG_CODE(
            {
                printf("//////////////////////////////////////////beginne Suche in diesem Env: %p\n", env);
                printf("gesuchter Key ist: ");
                scm_print(key);
                printf("\n");
            })
        scmObject tmpParent = env;

        ENV_DEBUG_CODE(
            {
                while (true)
                {
                    if ((tmpParent = tmpParent->value.scmEnv.parentEnv) != SCM_NULL)
                    {
                        printf("\nund sein Parent Env ist %p\n", tmpParent);
                    }
                    else
                    {
                        break;
                    }
                }
            })

        // printf("\n das env beinhaltet: \n");
        // for (int i = 0; i < env->value.scmEnv.capacity; i++)
        // {
        //     if (env->value.scmEnv.keyValuePairs[i] == NULL)
        //     {
        //         printf("NULL\n");
        //     }
        //     else
        //     {
        //         printf("i: %d  -  ", i);
        //         scm_print(getCar(env->value.scmEnv.keyValuePairs[i]));
        //         printf("\n");
        //     }
        // }
        while (true)
        {

            if (env->value.scmEnv.keyValuePairs[(index % capacity)] == NULL)
            {
                // Leerer Platz -> weitersuchen
                index++;
                index = index % capacity;
            }
            else if (getCar(env->value.scmEnv.keyValuePairs[(index % capacity)]) != key)
            {
                // Nicht der richtige Key -> weitersuchen
                ENV_DEBUG_CODE(
                    {
                        printf("index: %d\n", index);
                        printf("key ist: ");
                        scm_print(key);
                        printf("\n");
                        printf("wohl nicht der richtige Key: ");
                        scm_print(getCar(env->value.scmEnv.keyValuePairs[(index % capacity)]));
                        printf(" ...\n");
                    })
                index++;
                index = index % capacity;
            }
            else if (env->value.scmEnv.keyValuePairs[(index % capacity)]->value.scmCons.car == key)
            {
                // Key gefunden -> wert zurückgeben
                index = (index % capacity);
                ENV_DEBUG_CODE(
                    {
                        printf("gesuchter key war: ");
                        scm_print(key);
                        printf("\n");
                        printf("key gefunden at: %d\n", index);
                    })
                return env->value.scmEnv.keyValuePairs[index]->value.scmCons.cdr;
            }
            if (initialIndex == (index % capacity))
            {
                // Wir sind wieder beim ersten Index -> Den Key scheint es hier nicht zu geben!
                // Wir schauen im nächsten Parent_ENV!
                env = env->value.scmEnv.parentEnv;

                ENV_DEBUG_CODE(
                    {
                        printf("getEnvironmentValue: im bisherigen ENV wurde folgender Key nicht gefunden: ");
                        scm_print(key);
                        printf("\n bisheriges ENV capazität: %d\n", capacity);
                        printf("\n");
                    })
                if (env == SCM_NULL)
                {
                    printf("\n\n!!!!FATAL: getEnvironmentValue:  in keinem Env wurde das Symbol gefunden!! \n\n");
                    printf("gesuchter Key war: ");
                    scm_print(key);
                    printf("\n");
                    scmError("Gehe zurück zum REPL");
                }
                capacity = env->value.scmEnv.capacity;
                ENV_DEBUG_CODE({
                    printf("das neue Env hat Capazität: %d\n", capacity);
                })
                break;
            }
        }
    }

    return SCM_NULL;
}