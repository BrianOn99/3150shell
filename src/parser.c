#include <string.h>
#include <stdio.h>
#include "parser.h"
#define TOKEN_ARRAY_SIZE (MAX_CMD_LEN /2)

#define is_argchar(c) !invalid_argchar[c]


char invalid_argchar[256] = {['<'] = 1, ['>'] = 1, ['|'] = 1,
                             ['!'] = 1, ['\''] = 1, ['`'] = 1, ['"'] = 1 };

static char *token_array[TOKEN_ARRAY_SIZE];

void invalid_input()
{
        printf("Error: invalid input command line\n");
}

int valid_arg(char *token)
{
        for (char *c = token; *c; c++){
                if (!is_argchar(*c)){
                        return 0;
                }
        }
        return 1;
}

int valid_cmdname(char *token)
{
        for (char *c = token; *c; c++){
                if (!is_argchar(*c) || *c == '*'){
                        return 0;
                }
        }
        return 1;
}

int valid_cmdline(char **tokens)
{
        if (!valid_cmdname(*tokens++))
                return 0;
        while (*tokens){
                if (!valid_arg(*tokens))
                        return 0;
                tokens++;
        }

        return 1;
}

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
        if (buildtree(token_array, cmd_info->list, &(cmd_info->count)) != 0){
	        invalid_input();
                return -1;
	}
	return 0;
}

/* build the syyntax tree, although it is not a tree in this case */
int buildtree(char **tokens, char **list[], int *count)
{
        for (int i = 0; i < CMD_LIST_LEN - 1; i++){
                int ret = cmdtok(&tokens, &list[i]);

                if (!valid_cmdline(list[i]))
                        return -1;

                if (ret == -1){
			/* get the end */
			list[i+1] = NULL;
                        *count = i+1;
                        return 0;
		}

                /* a "|" should not be followed by a "|" or at the start */
                if ((*tokens && is_seperator(*tokens)) || !*tokens){
                        printf("error 104\n");
                        return -1;
                } 
        }

        fprintf(stderr, "toomany commands\n");
        return -1;
}

int is_seperator(char *token)
{
        return !strcmp(token, "|");
}

/*
 * this function is designed to retokenize the tokenized commandline.
 * That is, read the tokens, treat individual commands, separated by "|", as
 * tokens.
 * However, for convenience, there are some differences from strtok, the major
 * one is: don't pass NULL, instead, repetedly pass the same (modifiable) string.
 */
int cmdtok(char ***tokens_ptr, char ***target)
{
        *target = *tokens_ptr;
        char **tokens = *tokens_ptr;
        if (!*tokens){
                return -1;
        }

        do{
                tokens++;
        } while (*tokens && !is_seperator(*tokens));

        if (*tokens){
		*tokens = NULL;
		*tokens_ptr = tokens + 1;
		return 0;
	} else{
	        return -1;
	}

}
