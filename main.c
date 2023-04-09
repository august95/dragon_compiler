#include <stdio.h>
#include "compiler.h"
#include "helpers/vector.h"
int main()
{
    printf("---starting compiler---\n");
    int res = 0;
    res = compile_file("./test.c", "./test", 0);
    if(res == COMILER_FILE_COMPILED_OK)
    {
        printf("everything compiled fine[%i]\n",res);
    }
    else 
    {
        printf("Unknown response for compile file[%i]\n",res);
    }

    return 0;
}