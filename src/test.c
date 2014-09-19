#include <stdio.h>
#include "parser.h"

void main(void)
{
        if (is_seperator("1"))
                printf("yes\n");
        if (is_seperator("|"))
                printf("yes\n");
        char *line[10] = {"ab", "cd", "|", "eee", "|", "a"};
        char **lineptr = line;
        char **list[5];

        int i=0;
        while (1){
                list[i] = cmdtok(&lineptr);
                if (!list[i]) break;
                i++;
        }

        for (int j=0;; j++){
                if (!list[j]) break;
                char **cmd = list[j];
                for(int i=0;; i++){
                        if (!*cmd) break;
                        printf("%s\n", *cmd);
                        cmd++;
                }
                printf("\n");
        }
}

