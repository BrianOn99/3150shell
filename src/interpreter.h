#include "parser.h"

typedef int (*cmd_evaluater)(char**);

struct cmdmapping {
        char *cmdname;
        cmd_evaluater cmdfunction;
};

int interpreter(struct parsetree *cmd_info);
enum cmd_type classify(char *given_cmdname, cmd_evaluater *backeval);
