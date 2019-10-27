#ifndef __PROCESS_H__
# define __PROCESS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

typedef struct
{
    int     pid;
    int     name;
    char    prime;
} t_nameInfo;

void Error(char *message){
    write(2, "Error: ", 7);
    write(2, message, strlen(message));
    
    exit(-1);
}
#endif
