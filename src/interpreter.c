#include <stdio.h>
#include <string.h>
#include "interpreter.h"

/*
 * Here are functions making up the interprter, which actually run commands
 * the functions starting with "bn_" represent builtin commands
 */

int bn_cd(int cmdargc, char* cmdargv[])
{
        if (cmdargc != 1){
                fprintf(stderr, "cd: wrong number of arguments\n");
                return 1;
        }
        /* TODO: really do a cd */

        return 0;
}

char *classify(char *given_cmdname, cmd_evaluater *backeval)
{
        static struct cmdmapping bn_cmdmap[] =
        {
                {"cd", bn_cd},
                {"exit", NULL},
                {"fg", NULL},
                {"jobs", NULL},
                {NULL, NULL}
        };

        /*
         *TODO: check actual requirement of the string, I am lazy
         */
        struct cmdmapping *map_entry = bn_cmdmap;
        while (map_entry->cmdname){
                if (!strcmp(map_entry->cmdname, given_cmdname)){
                        *backeval = map_entry->cmdfunction;
                        return "Built-in Command";
                }
                map_entry++;
        } 

        return "Command Name";
}

void interpreter(struct parsetree cmd_info)
{
        char **token = cmd_info.cmd;
        
        cmd_evaluater eval = NULL;
        if (token[0] != NULL){
                char *cmd_type = classify(token[0], &eval);
                printf("Token %d: %s (%s)\n", 1, token[0], cmd_type);
        }

        for (int i = 1; token[i] != NULL; i++)
                printf("Token %d: %s (%s)\n", i + 1, token[i], "Argument");

        if (eval != NULL)
                eval(cmd_info.cmd_len, cmd_info.cmd);
}
