#include <sys/queue.h>
#include "parser.h"

typedef int (*cmd_evaluater)(char**);

struct cmdmapping {
        char *cmdname;
        cmd_evaluater cmdfunction;
};

struct job {
        char *rawline;
        pid_t pgid;
        TAILQ_ENTRY(job) entries;
};

void job_queue_init();
int interpreter(struct parsetree *cmd_info);
enum cmd_type classify(char *given_cmdname, cmd_evaluater *backeval);
