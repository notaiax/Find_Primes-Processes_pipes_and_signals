/* -----------------------------------------------------------------------
PRA1: [TODO]
Codi font: generador.c
Nom complet Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "process.h"

void sig_handler(int signal){
    exit(0);
}

int main(int argc, char **argv){
    int seq, i;

    seq = atoi(argv[1]);
    for(i = 2; i <= seq; i++){
        if(write(10, &i, sizeof(int)) <= 0)
            Error("[generador.c:main] Write pipe numbers\n"); 
    }
    close(10);
    signal(SIGTERM, sig_handler);
    pause();
}

