/* -----------------------------------------------------------------------
PRA1: [TODO]
Codi font: calculador.c
Nom complet Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "process.h"

void sig_handler(int signal){
    exit(0);
}
int main(int argc, char **argv){
    int num;
    t_nameInfo info;
    
    info.pid = getpid(); 
    while(read(11, &num, sizeof(num)) != 0){
        info.prime = 'S';
        info.name = num;
        for(int i = 0; i < num; i++){
            if(num % i == 0)
                info.prime = 'N';
        }
        write(20, &info, sizeof(t_nameInfo));
    }
    close(20);
    signal(SIGTERM, sig_handler);
    pause();
}

