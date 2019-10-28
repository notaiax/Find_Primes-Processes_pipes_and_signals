/* -----------------------------------------------------------------------
PRA1: [TODO]
Codi font: controlador.c
Nom complet Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "process.h"

#define MAX_PROCESSES 100


int main(int argc, char **argv){
    int num_proc, gen_pid; //seq->last num in the prime sequence 
    int pipe_responses[2], pipe_numbers[2];
    char char_seq[15]; //seq->last num in the prime sequence (char format)    

    //Read num processes
    if (argc < 2)
        Error("[controlador.c:main] Number of processes to create not specified\n");
    num_proc = atoi(argv[1]);
    //Check max processes
    if (num_proc > MAX_PROCESSES)
        Error("[controlador.c:main] Exceeded the process limit\n");
    //Read sequence size
    if (write(1, "Calcular nombres primers de 2 a?\n", strlen("Calcular nombres primers de 2 a?\n")) < 0)
    if (read(0, &char_seq, sizeof(char_seq)) < 0)
        Error("[controlador.c:main] Reading sequence of primes\n");
    //Signal treatment
    if (signal(SIGTERM, SIG_IGN) == SIG_ERR)
        Error("[controlador.c:main] Signal treatment\n");
    //Pipes creation
    if((pipe(pipe_responses) || pipe(pipe_numbers)) < 0)
        Error("[controlador.c:main] Pipe creation\n");
    //Get sequence from generador.c
    if((gen_pid = fork()) == 0){
        dup2(pipe_numbers[1],10);
        close(pipe_numbers[0]);
        close(pipe_numbers[1]); 
        close(pipe_responses[0]);
        close(pipe_responses[1]);
        execl("./generador", "generador", char_seq, NULL);
        Error("[controlador.c:main] Covering son.\n"); 
    }
    close(pipe_numbers[1]);


}

