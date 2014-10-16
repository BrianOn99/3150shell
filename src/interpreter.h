#include <sys/queue.h>
#include <sys/types.h>
#include "parser.h"

typedef int (*cmd_evaluater)(char**);

struct cmdmapping {
        char *cmdname;
        cmd_evaluater cmdfunction;
};

struct job {
        char *rawline;
        int remain;
        int awake;
        int jobid;
        pid_t *pid;
        pid_t pgid;
        int count;
        TAILQ_ENTRY(job) entries;
};

void interpreter_init();
int interpreter(struct parsetree *cmd_info);
enum cmd_type classify(char *given_cmdname, cmd_evaluater *backeval);
int update_job_queue();
int wait_job(struct job *jp);

