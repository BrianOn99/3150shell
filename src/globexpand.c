#include <glob.h>
#include <stdlib.h>
#include <stdio.h>
#include "globexpand.h"

int glob_expand(char * const *words, glob_t *pglob_res)
{
        char *word;
        int flags = GLOB_MARK | GLOB_NOMAGIC | GLOB_NOCHECK;

        while (word = *words++) {
                int ret = glob(word, flags, NULL, pglob_res);

                if (ret == GLOB_NOSPACE) {
                        fprintf(stderr, "No memory for globbing\n");
                        return -1;
                }

                /* want this flag starting from 2nd time */
                flags |= GLOB_APPEND;  
        }

        return 0;
}
