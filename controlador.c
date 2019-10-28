/* -----------------------------------------------------------------------
PRA1: [TODO]
Codi font: controlador.c
Nom complet Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "process.h"

#define MAX_PROCESSES 100


int main(int argc, char **argv){
    int num_proc, gen_pid, n_primes; //seq->last num in the prime sequence 
    int pipe_responses[2], pipe_numbers[2];
    char char_seq[15], result[100]; //seq->last num in the prime sequence (char format)    
    t_nameInfo info;

    n_primes = 0;
    //Read num processes
    if (argc < 2)
        Error("[controlador.c:main] Number of processes to create not specified\n");
    num_proc = atoi(argv[1]);
    //Check max processes
    if (num_proc > MAX_PROCESSES)
        Error("[controlador.c:main] Exceeded the process limit\n");
    //Read sequence size
    if (write(1, "Calcular nombres primers de 2 a?\n", strlen("Calcular nombres primers de 2 a?\n")) < 0) 
        Error("[controlador.c:main] Writing\n");
    if (read(0, char_seq, sizeof(char_seq)) < 0)
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
    // Close pipe_numbers 1 because generador is the only allowed to write in this pipe
    close(pipe_numbers[1]);
    for (int i = 0; i < num_proc; i++){
        if (fork == 0){
            dup2(pipe_numbers[0], 11);
            dup2(pipe_responses[1], 20);
            close(pipe_numbers[0]);
            close(pipe_responses[0]);
            close(pipe_responses[1]);
            execl("./calculador", "calculador", NULL);     
            Error("[controlador.c:main] Covering son.\n"); 
        }
    }
    dup2(pipe_responses[0], 21);
    close(pipe_numbers[0]);
    close(pipe_responses[0]);
    close(pipe_responses[1]);
    
    //Read from the responses pipe

    while (read(21, &info, sizeof(info)) != 0){
        if (info.prime == 'S')
            n_primes++;
        sprintf(result, "Controlador: rebut del Calculador PID %i: nombre %i es primer? %c\n", info.pid, info.name, info.prime);
        write(1, result, strlen(result));
    }
    kill(0, SIGTERM);
    exit(0);
}

