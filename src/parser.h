#ifndef PARSE_H
#define PARSE_H

#define MAX_CMD_LEN 255

struct parsetree {
        char **cmd;
        int cmd_len;
};

int tokenize(char cmdline[], char *token_store[]);
void parser(char cmdline[], struct parsetree *cmd_info);

#endif
