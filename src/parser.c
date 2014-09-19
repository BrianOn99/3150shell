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

int parser(char cmdline[], struct parsetree *cmd_info)
{
        // it should be more compllex in phase 2
        int cmd_len = tokenize(cmdline, token_array);
        if (buildtree(token_array, cmd_info->list, &(cmd_info->count)))
                return -1;
}

int buildtree(char **tokens, char **list[], int *count)
{
        for (int i = 0; i < CMD_LIST_LEN; i++){
                list[i] = cmdtok(&tokens);
                if (is_seperator(*(tokens+1)))
                        return -1;
                if (list[i] == NULL){
                        *count = i;
                        return 0;
                }
        }

        fprintf(stderr, "toomany commands\n");
        return -1;
}

int is_seperator(char *token)
{
        return (!token) || (!strcmp(token, "|"));
}

char** cmdtok(char ***tokens_ptr)
{
        char **tokens = *tokens_ptr;
        char **orig = tokens;

        if (!*tokens)
                return NULL;

        do{
                tokens++;
        } while (!is_seperator(*tokens));

	*tokens = NULL;
        *tokens_ptr = tokens + 1;

        return orig;
        
}
