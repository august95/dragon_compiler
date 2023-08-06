#include "compiler.h"
#include <stdarg.h>
#include <stdio.h>
#include "helpers/vector.h"

static struct compile_process* current_process = NULL; 

void codegen_new_scope(int flags)
{
    #warning "the resolver needs to exist"
}

void codegen_finish_scop()
{
    #warning "a resolver is needed"
}

struct node* codegen_node_next()
{
    return vector_peek_ptr(current_process->node_tree_vec);
}

void asm_push_args(const char* ins, va_list args)
{
    va_list args2;
    va_copy(args2, args);
    vfprintf(stdout, ins, args);
    fprintf(stdout, "\n");
    if(current_process->ofile)
    {
        vfprintf(current_process->ofile, ins, args2);
        fprintf(current_process->ofile, "\n");
    }
}

void asm_push(const char* ins, ...)
{
    va_list args;
    va_start(args,ins);
    asm_push_args(ins, args);
    va_end(args);
}

void codegen_generate_data_section_part(struct node* node)
{
    // create a switch for processing global data that is always a root node
}

void codgen_generate_data_section()
{
    asm_push("seciont .data");
    struct node* node = codegen_node_next();
    while(node)
    {
        codegen_generate_data_section_part(node);
        node = codegen_node_next();
    }
}

void codegen_generate_root_node(struct node* node)
{
    //process any functions
    // functions migth push datat to the stack
}

void codegen_generate_root()
{
    asm_push("section .text");
    struct node* node = NULL;
    while((node = codegen_node_next()) != NULL)
    {
        codegen_generate_root_node(node);
    }
}

void codegen_write_strings()
{
    #warning "loop through the string table and write all the strings"
}

void codegen_generate_rod()
{
    asm_push("section .rdata");
    codegen_write_strings();
}

int codegen(struct compile_process* process)
{
    current_process = process;
    scope_create_root(process);
    vector_set_peek_pointer(process->node_tree_vec, 0);
    codegen_new_scope(0);
    codgen_generate_data_section();
    vector_set_peek_pointer(process->node_tree_vec, 0);
    codegen_generate_root();
    codegen_finish_scop();

    //generate read only data

    codegen_generate_rod();
    return 0;
}



