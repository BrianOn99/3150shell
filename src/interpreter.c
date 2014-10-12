#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <errno.h>
#include <sys/queue.h>
#include "setsig.h"
#include "interpreter.h"

#define PGID 0
#define JOBID 1

static int jobno = 0;
static TAILQ_HEAD(tailhead, job) head;
 
void job_queue_init()
{
        TAILQ_INIT(&head);
}

enum cmd_type { builtin, external };

struct iofd {
        int in;
        int out;
};

int cmdlen(char** cmdargv)
{
	char **arg_ptr = cmdargv;
	while (*(++arg_ptr));
	return arg_ptr - cmdargv;
}

int check_cmdlen(char* cmdargv[], int len, const char *cmdname)
{
        int cmdargc = cmdlen(cmdargv);
        if (cmdargc != len) {
                /*
                 * Error SHOULD be printed to stderr, but P.21 require stdout
                 * I am Very reluctant to do so, it is ugly
                 */
                fprintf(stdout, "%s: wrong number of arguments\n", cmdname);
                return 0;
        }
}

/*
 * Here are functions making up the interprter, which actually run commands
 * the functions starting with "bn_" represent builtin commands
*/
int bn_cd(char* cmdargv[])
{
        if (!check_cmdlen(cmdargv, 2, "cd"))
                return 0;

        chdir(cmdargv[1]);
        return 1;
}

int bn_fg(char* cmdargv[])
{
        if (!check_cmdlen(cmdargv, 2, "fg"))
                return 0;
        return 1;
}

int bn_jobs(char* cmdargv[])
{
        if (!check_cmdlen(cmdargv, 1, "jobs"))
                return 0;

        update_job_queue();

        struct job *jp;
        for (jp = head.tqh_first; jp != NULL; jp = jp->entries.tqe_next) {
                printf("job: %s\n", jp->rawline);
        }
        return 1;
}
 
int bn_exit(char* cmdargv[])
{
        if (!check_cmdlen(cmdargv, 1, "exit"))
                return 0;

        printf("[ Shell Terminated ]\n");
        exit(0);
}

struct job *getjob(int id, int key)
{
        struct job *jp;
        for (jp = head.tqh_first; jp != NULL; jp = jp->entries.tqe_next) {
                int res;
                if (key == PGID)
                        res = (id == jp->pgid);
                if (key == JOBID)
                        res = (id == jp->jobid);

                if (res)
                        return jp;
        }

        fprintf(stderr, "no indicated id %s\n", id);
        return NULL;
}

void rmjob(struct job *j)
{
        TAILQ_REMOVE(&head, j, entries);
}

void update_job_queue()
{
        int status;
        struct job *jp;

        for (jp = head.tqh_first; jp != NULL; jp = jp->entries.tqe_next) {
                while (waitpid(-(jp->pgid), &status, WUNTRACED|WNOHANG)) {
                        if WIFEXITED(status)
                                jp->remain--;
                        if (jp->remain == 0) {
                                rmjob(jp);
                                break;
                        }
                }
        }
}

int run_builtin(char **cmd)
{
        cmd_evaluater eval = NULL;
        classify(cmd[0], &eval);
        return eval(cmd);
}

int run_external(char* cmdargv[], struct iofd inoutfd, pid_t pgid)
{
        int pid = fork();
        if (pid == -1) {
                perror("fork");
                return -1;
        } else if (pid == 0) {
                pid = getpid ();
                if (pgid == 0)
                        pgid = pid;
                setpgid (pid, pgid);
                tcsetpgrp(0, pgid);

                if (inoutfd.in != 0) {
                        dup2(inoutfd.in, 0);
                        close(inoutfd.in);
                }
                if (inoutfd.out != 1) {
                        dup2(inoutfd.out, 1);
                        close(inoutfd.out);
                }

                unsetsig();
                execvp(cmdargv[0], cmdargv);
                perror("exec");
                exit(-1);

        } else {
                return pid;
        }
}

static struct cmdmapping bn_cmdmap[] =
{
        {"cd", bn_cd},
        {"exit", bn_exit},
        {"fg", bn_fg},
        {"jobs", bn_jobs},
        {NULL, NULL}
};

/* 
 * classify the command: builtin or external program
 * A function pointer will be stored in backeval, which implement the action
 */
enum cmd_type classify(char *given_cmdname, cmd_evaluater *backeval)
{

        struct cmdmapping *map_entry = bn_cmdmap;
        while (map_entry->cmdname){
                if (!strcmp(map_entry->cmdname, given_cmdname)){
                        if (backeval)
                                *backeval = map_entry->cmdfunction;
                        return builtin;
                }
                map_entry++;
        } 

        return external;
}

/* master function of this library */
int interpreter(struct parsetree *cmd_info)
{
        struct job *jobnow = malloc(sizeof(struct job));
	char ***list = cmd_info->list;
        struct iofd *inoutfds = malloc(sizeof(struct iofd) * cmd_info->count);

        jobnow->pgid = 0;
        jobnow->rawline = strdup(cmd_info->rawline);
        jobnow->remain = cmd_info->count;
        jobnow->jobid = jobno++;
        inoutfds[0].in = 0;
        inoutfds[cmd_info->count-1].out = 1;

        for (int i=0; i+1 < cmd_info->count; i++) {
                int fildes[2];

                if (pipe(fildes) == -1){
                        perror("Creating pipe");
                        return -1;
                }

                inoutfds[i].out = fildes[1];
                inoutfds[i+1].in = fildes[0];
        }

        if (cmd_info->count > 1) {
                for (int i=0; i < cmd_info->count; i++) {
                        if (classify(list[i][0], NULL) == builtin) {
                                printf("Error: invalid input command line\n");
                                return -1;
                        }
                }
        } else {
                cmd_evaluater eval = NULL;
                if (classify(list[0][0], &eval) == builtin)
                        return eval(list[0]);
        }

        for (int i=0; i < cmd_info->count; i++) {
                pid_t chldpid = run_external(list[i], inoutfds[i], jobnow->pgid);
                if (!jobnow->pgid)
                        jobnow->pgid = chldpid;
                setpgid(chldpid, jobnow->pgid);
                tcsetpgrp(0, jobnow->pgid);
        }

        for (int i=0; i < cmd_info->count; i++) {
                int status;
                if (waitpid(- jobnow->pgid, &status, WUNTRACED) == -1) {
                        if (errno == ECHILD)
                                break;
                        else
                                return -1;
                } else if (WIFEXITED(status)) {
                        //return WEXITSTATUS(status);
                } else if (WIFSTOPPED(status)) {
                        printf("DEBUG ONLY: child stopped\n");
                        TAILQ_INSERT_TAIL(&head, jobnow, entries);
                        break;
                }
        }

        if (tcsetpgrp(0, getpgid(0)) == -1)
                perror("tcsetpgrp");

        free_parsetree_content(cmd_info);
        /* TODO: close fd */
}
