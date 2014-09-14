#include <string.h>
#include <stdio.h>
#include "parser.h"
#define TOKEN_ARRAY_SIZE (MAX_CMD_LEN /2)

static char *token_array[TOKEN_ARRAY_SIZE];

char **tokenize(char cmdline[])
{
        token_array[0] = strtok(cmdline, " ");

        for (int i = 1; i < TOKEN_ARRAY_SIZE; i++){
                token_array[i] = strtok(NULL, " ");
                if (token_array[i] == NULL)
                        return token_array;
        }

        // we should never get here
        fprintf(stderr, "toomany tokens\n");
}

void parser(char cmdline[], struct parsetree *cmd_info)
{
        // it should be more compllex in phase 2
        cmd_info->cmd = tokenize(cmdline);
}

