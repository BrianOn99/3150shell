#include "parser.h"

typedef int (*cmd_evaluater)(char**);

struct cmdmapping {
        char *cmdname;
        cmd_evaluater cmdfunction;
};

void interpreter(struct parsetree *cmd_info);
