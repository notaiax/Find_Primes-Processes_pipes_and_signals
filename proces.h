/* -----------------------------------------------------------------------
PRA1:  Processos, pipes i senyals: Primers - Pralab 1
Codi font: proces.h
Àiax Faura Vilalta
---------------------------------------------------------------------- */

#ifndef __PROCES_H__
# define __PROCES_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct
{
	int pid;
	int nombre;
	char primer;
} t_infoNombre;

void Error(char *missatge){
    write(2, "Error: ", 7);
    write(2, missatge, strlen(missatge));
		write(2, "\n", 2);
    exit(-1);
}

#endif
