#include <signal.h>
#include "setsig.h"

int siglist[] = {SIGINT, SIGTERM, SIGQUIT, SIGTSTP, SIGTTOU};

void setsig(){
        for (int i=0; i < sizeof(siglist)/sizeof(int); i++)
                signal(siglist[i], SIG_IGN);
}

void unsetsig(){
        for (int i=0; i < sizeof(siglist)/sizeof(int); i++)
                signal(siglist[i], SIG_DFL);
}
