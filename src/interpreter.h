#include "parser.h"

struct cmdmapping {
        char *cmdname;
        int (*cmdfunction)(char**);
};

void interpreter(struct parsetree cmd_info);
