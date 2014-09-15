#include <string.h>
#include <stdio.h>
#include "parser.h"
#define TOKEN_ARRAY_SIZE (MAX_CMD_LEN /2)

static char *token_array[TOKEN_ARRAY_SIZE];

int tokenize(char cmdline[], char *token_store[])
{
        token_store[0] = strtok(cmdline, " ");

        for (int i = 1; i < TOKEN_ARRAY_SIZE; i++){
                token_store[i] = strtok(NULL, " ");
                if (token_store[i] == NULL)
                        return i - 1;
        }

        // we should never get here
        fprintf(stderr, "toomany tokens\n");
}

void parser(char cmdline[], struct parsetree *cmd_info)
{
        // it should be more compllex in phase 2
        cmd_info->cmd_len = tokenize(cmdline, token_array);
        cmd_info->cmd = token_array;
}

