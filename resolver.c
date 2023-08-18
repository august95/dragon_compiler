#include "compiler.h"
#include <stdlib.h>
#include "helpers/vector.h"

bool resolver_result_failed(struct resolver_result* result)
{
    return result->flags & RESOLVER_RESULT_FAILED;
}

bool resolver_result_ok(struct resolver_result* result)
{
    return !resolver_result_failed(result);
}

bool resolver_result_finished(struct resolver_result* result)
{
    return result->flags & RESOLVER_RESULT_FLAG_RUNTIME_NEEDED_TO_FINISH_PATH;
}

struct resolver_entity* resolver_result_entity_root(struct resolver_result* result)
{
    return result->entity;
}

struct resolver_entity* resolver_result_entity_next(struct resolver_entity* entity)
{
    return entity->next;
}

struct resolver_entity* resolver_entity_clone(struct resolver_entity* entity)
{
    if(!entity)
    {
        return NULL;
    }

    struct resolver_entity* new_entity = calloc(1, sizeof(struct resolver_entity));
    memcpy(new_entity, entity, sizeof(struct resolver_entity));
    return new_entity;
}

struct resolver_entity* resolver_result_entity(struct resolver_result* result)
{
    if(resolver_result_failed(result))
    {
        return NULL;
    }
    return result->entity;
}

struct resolver_result* resolver_new_result(struct resolver_process* process)
{
    struct resolver_result* result = calloc(1, sizeof(struct resolver_result));
    result->array_data.array_entities = vector_create(sizeof(struct resolver_entity*));
    return result;
}

void resolver_result_free(struct resolver_result* result)
{
    vector_free(result->array_data.array_entities);
    free(result);
}

struct resolver_scope* resolver_process_scope_current(struct resolver_process* process)
{
    return process->scope.curret;
}

void resolver_runtime_needed(struct resolver_result* result, struct resolver_entity * last_entity)
{
    result->entity = last_entity;
    result->flags &= ~RESOLVER_RESULT_FLAG_RUNTIME_NEEDED_TO_FINISH_PATH;
}

void resolver_result_entity_push(struct resolver_result* result, struct resolver_entity * entity)
{
    if(!result->first_entity_const)
    {
        result->first_entity_const = entity;
    }

    if(!result->last_entity)
    {
        result->entity = entity;
        result->last_entity= entity;
        result->count++;
        return;
    }

    result->last_entity->next = entity;
    entity->prev = result->last_entity;
    result->last_entity = entity;
    return result->count++;
}

struct resolver_entity* resolver_result_peek(struct resolver_result* result)
{
    return result->last_entity;
}