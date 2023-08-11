#include "compiler.h"
#include "helpers/vector.h"
#include <assert.h>

#define STACK_PUSH_SIZE 4

void stackframe_pop(struct node* func_node)
{
    struct stack_frame* frame = &func_node->func.frame;
    vector_pop(frame->elements);
}

struct stack_frame_element* stackframe_back(struct node* funct_node)
{
    return vector_back_or_null(funct_node->func.frame.elements);
}

void stackframe_pop_expecting(struct node* func_node, int expecting_type, const char* expecting_name)
{
    struct stack_frame* frame = &func_node->func.frame;
    struct stack_frame_element* last_element = stackframe_back(func_node);
    assert(last_element);
    assert(last_element->type == expecting_type && S_EQ(last_element->name, expecting_name));
    stackframe_pop(func_node);
}

void stackframe_push( struct node* func_node, struct stack_frame_element* element)
{
    struct stack_frame* frame = &func_node->func.frame;
    //stack grows downwards
    element->offset_from_bp = -(vector_count(frame->elements) * STACK_PUSH_SIZE);
    vector_push(frame->elements, element);
}

void stackframe_sub(struct node* func_node, int type, const char* name, size_t amount)
{
    assert((amount % STACK_PUSH_SIZE) == 0);
    size_t total_pushes = amount / STACK_PUSH_SIZE;
    for(size_t i = 0; i < total_pushes; i++)
    {
        stackframe_push(func_node, &(struct stack_frame_element){.type=type, .name=name});
    }
}

void stackframe_add(struct node* func_node, int type, const char* name, size_t amount)
{
    assert((amount % STACK_PUSH_SIZE) == 0);
    size_t total_pushes = amount / STACK_PUSH_SIZE;
    for(size_t i = 0; i < total_pushes; i++)
    {
        stackframe_pop(func_node);
    }
}