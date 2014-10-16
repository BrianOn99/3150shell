#include <sys/queue.h>
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
        pid_t pgid;
        TAILQ_ENTRY(job) entries;
};

void interpreter_init();
int interpreter(struct parsetree *cmd_info);
enum cmd_type classify(char *given_cmdname, cmd_evaluater *backeval);
int update_job_queue();
int wait_job(struct job *j, int insert);
