#include <stdio.h>
#include "interpreter.h"

void interpreter(struct parsetree cmd_info)
{
        char **token = cmd_info.cmd;
        for (int i = 0; token[i] != NULL; i++)
                printf("Token %d: %s\n", i + 1, token[i]);
}
