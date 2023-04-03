#ifndef CCOMPIELR_H
#define  CCOMPIELR_H

#include <stdio.h>

enum
{
    COMILER_FILE_COMPILED_OK,
    COMPILER_FAILED_WITH_ERRORS 
};

struct compile_process
{
    // how to compile
    int flags;

    struct compile_process_input_file
    {
        FILE* fp;
        const char* abs_path;
    } cfile;

    FILE* ofile;

};
int compile_file(const char* filename, const char* out_file, int flags);
struct compile_process* compile_process_create(const char* filename, const char* filename_out, int flags);



#endif /* CCOMPIELR_H*/