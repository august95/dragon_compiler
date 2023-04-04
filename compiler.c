#include "compiler.h"

struct lex_process_functions compiler_lex_functions = {
    .next_char=compile_process_next_char,
    .peak_char=compile_process_peak_char,
    .push_char=compile_process_push_char
};

int compile_file(const char* filename, const char* out_file, int flags)
{
    struct compile_process* process = compile_process_create(filename, out_file, flags);
    if(!process)
        return COMPILER_FAILED_WITH_ERRORS;

    //perform lexical analysis
    struct lex_process* lex_process = lex_process_create(process, &compiler_lex_functions, NULL);
    if(!lex_process)
    {
        return COMPILER_FAILED_WITH_ERRORS;
    }
    if(lex(lex_process) != LEXICAL_ANALYSIS_ALL_OK)
    {
        return COMPILER_FAILED_WITH_ERRORS;
    }

    //perfrom parsings

    //perfrom code generation..
    return COMILER_FILE_COMPILED_OK;
}