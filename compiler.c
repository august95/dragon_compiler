#include "compiler.h"

int compile_file(const char* filename, const char* out_file, int flags)
{
    struct compile_process* process = compile_process_create(filename, out_file, flags);
    if(!process)
        return COMPILER_FAILED_WITH_ERRORS;

    //perform lexical analysis

    //perfrom parsings

    //perfrom code generation..
    return COMILER_FILE_COMPILED_OK;
}