
#include "process.h"

#define MAX_PROCESSES 100

int main(int argc, char **argv){
    int num_proc, seq;
    char char_seq[15];

    if (argc < 2)
        Error("[controlador.c:main] Number of processes to create not specified\n");
    num_proc = atoi(argv[1]);
    if (num_proc > MAX_PROCESSES)
        Error("[controlador.c:main] Exceeded the process limit\n");
    if (write(1, "Calcular nombres primers de 2 a?\n", strlen("Calcular nombres primers de 2 a?")) < 0)
        Error("[controlador.c:main] Exceeded the process limit\n");
    if (read(0, &char_seq, 15) < 0)
        Error("[controlador.c:main] Reading sequence of primes\n");
    seq = atoi(char_seq);

}
