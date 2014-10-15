#ifndef PARSE_H
#define PARSE_H

#include <glob.h>

#define MAX_CMD_LEN 255
#define CMD_LIST_LEN 10

struct parsetree {
        char *rawline;
        char **token_array;
        char **list[CMD_LIST_LEN];
        glob_t *tokens_pglob;
        int count;
};

int buildtree(char **tokens, char **list[], int *count);
int tokenize(char cmdline[], char *token_store[]);
int parser(const char cmdline[], struct parsetree *cmd_info);
int cmdtok(char ***tokens_ptr, char ***target);
int is_seperator(char *token);
void free_parsetree_content(struct parsetree *cmd_info);

#endif
