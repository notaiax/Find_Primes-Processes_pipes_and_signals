/* -----------------------------------------------------------------------
PRA1:  Processos, pipes i senyals: Primers - Pralab 1
Codi font: controlador.c
Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "proces.h"

int main(int argc, char *argv[]) {
	int N;
	char num[15];

	if (argc < 2)
		Error("Insuficient número d'arguments.");
	// Agafem el nombre de processos que ens han passat per paràmetre
	int processos = atoi(argv[1]);

	// Creació de variables que necessitarem per demanar la N

	// Demanem a l'usuari fins a quin nombre (N) hem de calcular els nombres primers
		write(1, "Calcular nombres primers de 2 a? ", strlen("Calcular nombres primers de 2 a? "));
		read(0, num, sizeof(num));
		N = atoi(num);

	if (N < 2)
		Error("N ha de ser 2 o major");

	// Declaració dels pipes
	int nombres[2], respostes[2];

	// Creació pipe Nombres
	if(pipe(nombres) < 0)
		// Cas d'error de creació dels pipes
		Error ("ERROR en la creació del pipe nombres");

	// Creació del procés fill Generador
	int pidGenerador = fork();

	if(pidGenerador < 0)
		// Cas d'error del fork()
		Error("ERROR en el fork del Generador");

 	else if (pidGenerador == 0) {
		// Crear el procés fill Generador
		// Dupliquem el descriptor d'escriptura de nombres al descriptor de fitxers 10 i el tanquem
		dup2(nombres[1], 10);
		close(nombres[0]);
		close(nombres[1]);

		// Transformem aquest procés en un procés generador

		execlp("./generador", "generador", num, NULL);
		Error("ERROR en el recobriment del generador");
	}

	// Creació dels processos fills Calculador que calguin
	int pidCalculadors[processos];

	// Creació pipe Respostes
	if(pipe(respostes) < 0)
		// Cas d'error de creació dels pipes
		Error("ERROR en la creació del pipe respostes");


	for (int i = 0; i < processos; i++) {

		pidCalculadors[i] = fork();

		if (pidCalculadors[i] < 0 )
			// Cas d'error del fork()
			Error("ERROR en el fork del Calculador");
		else if (pidCalculadors[i] == 0) {
			// Creació del procés fill Calculador
			// Dupliquem el descriptor de lectura de nombres al descriptor de fitxers 11 i el tanquem
			dup2(nombres[0], 11);
			close(nombres[0]);
			close(nombres[1]);

			// Fem el mateix amb el descriptor de escriptura de respostes posant-lo al 20
			dup2(respostes[1], 20);
			close(respostes[0]);
			close(respostes[1]);

			//Transformem aquest procés en un procés calculador
			execlp("./calculador", "calculador", NULL);
				// En cas d'error en el recobriment
			Error("ERROR en el recobriment del calculador");
			exit(0);
		}
	}
	// Dupliquem la lectura del pipe respostes al descriptor 21
	dup2(respostes[0], 21);
	close(respostes[1]);
	close(respostes[0]);

	// Tanquem els descriptos no utilitzats pel pare
	close(nombres[1]);
	close(nombres[0]);

	int contadorPrimers = 0;

	t_infoNombre infoNombre;

	while((read(21, &infoNombre, sizeof(t_infoNombre))) > 0) {
		char buffer[100];
		sprintf(buffer, "Controlador: rebut del Calculador PID %d: nombre %d és primer? %c \n", infoNombre.pid, infoNombre.nombre, infoNombre.primer);

		if(infoNombre.primer == 'S') {
			contadorPrimers++;
		}

		// Mostrar els resultats dels calculadors i controlar l'error si n'hi ha
		if(write(1,buffer,strlen(buffer)) < 0)
          Error("Hi ha un error al escriure a la sortida");
	}

	if((read(21, &infoNombre, sizeof(t_infoNombre))) < 0)
		Error("Error al llegir els resultats dels calculadors");


	// Enviem senyal SIGTERM al generador
	if(kill(pidGenerador, SIGTERM) < 0)
		Error("Error en la senyal SIGTERM en el generador");
	// Esperem la finalització del generador
	wait(NULL);

	int primersOut = 0;
	int exitCalculadors = 0;

	// Enviem senyal SIGTERM als calculadors
	for (int i = 0; i < processos ; i++) {
		if(kill(pidCalculadors[i], SIGTERM) < 0)
			Error("Error en la senyal SIGTERM en el calculador");
		// Esperem la finalització del calculador en curs
		wait(&exitCalculadors);
		primersOut += WEXITSTATUS(exitCalculadors);
	}

	// Mostrar resultats
	char resultats[200];
	sprintf(resultats, "Controlador: nombrePrimersPipe = %d nombrePrimersExit = %d \n", contadorPrimers, primersOut);
	if(write (1, resultats, strlen(resultats)) < 0)
		Error("Error en la sortida");
	exit(0);
}
