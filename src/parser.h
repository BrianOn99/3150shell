#ifndef PARSE_H
#define PARSE_H

#define MAX_CMD_LEN 255
#define CMD_LIST_LEN 10

struct parsetree {
        char **list[CMD_LIST_LEN];
        int count;
};

int buildtree(char **tokens, char **list[], int *count);
int tokenize(char cmdline[], char *token_store[]);
int parser(char cmdline[], struct parsetree *cmd_info);
int cmdtok(char ***tokens_ptr, char ***target);
int is_seperator(char *token);

#endif
