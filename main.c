#include <stdio.h>
#include "compiler.h"
#include "helpers/vector.h"
int main()
{
    int res = compile_file("./test.c", "./test", 0);
    if(res == COMILER_FILE_COMPILED_OK)
    {
        printf("everything compiled fine\n");
    }
    else if (res ==  COMPILER_FAILED_WITH_ERRORS);
    {
        printf("Unknown response for compile file\n");
    }

    return 0;
}