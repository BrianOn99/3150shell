#include <stdio.h>
#include "interpreter.h"

void interpreter(struct parsetree cmd_info)
{
        char **token = cmd_info.cmd;

        static cmdmapping bn_cmdmap[] =
        {
                {"cd", NULL},
                {"exit", NULL},
                {"fg", NULL},
                {"jobs", NULL},
                {NULL, NULL}
        };

        for (int i = 0; token[i] != NULL; i++)
                printf("Token %d: %s\n", i + 1, token[i]);
}
