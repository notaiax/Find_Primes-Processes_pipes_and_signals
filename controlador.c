/* -----------------------------------------------------------------------
PRA1:  Processos, pipes i senyals: Primers - Pralab 1
Codi font: controlador.c
Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "proces.h"

int main(int argc, char *argv[]) {

	if (argc < 2)
		Error("Insuficient número d'arguments.");
	// Agafem el nombre de processos que ens han passat per paràmetre
	int processos = atoi(argv[1]);

	// Creació de variables que necessitarem per demanar la N
	int N;
	char num[15];

	// Demanem a l'usuari fins a quin nombre (N) hem de calcular els nombres primers
		write(1, "Calcular nombres primers de 2 a? ", strlen("Calcular nombres primers de 2 a? "));
		read(0, num, sizeof(num));
		N = atoi(num);

	if (N < 2)
		Error("N ha de ser 2 o major");

	// Declaració dels pipes
	int nombres[2], respostes[2];

	// Creació pipe Nombres
	if(pipe(nombres) < 0) {
		// Cas d'error de creació dels pipes
		perror ("ERROR en la creació del pipe nombres");
		exit (-1);
	}

	// Creació del procés fill Generador
	int pidGenerador = fork();

	if(pidGenerador < 0) {
		// Cas d'error del fork()
		perror("ERROR en el fork del Generador");
		exit(-1);

	} else if (pidGenerador == 0) {
		// Crear el procés fill Generador
		// Dupliquem el descriptor d'escriptura de nombres al descriptor de fitxers 10 i el tanquem
		dup2(nombres[1], NOMBRES_IN);
		close(nombres[0]);
		close(nombres[1]);

		// Transformem aquest procés en un procés generador

		if(execlp("./generador", "generador", num, NULL) < 0) {
			// En cas d'error en el recobriment
			perror("ERROR en el recobriment del generador");
			exit(-1);
		}
	}

	// Creació dels processos fills Calculador que calguin
	int pidCalculadors[processos];

	// Creació pipe Respostes
	if(pipe(respostes) < 0) {
		// Cas d'error de creació dels pipes
		perror ("ERROR en la creació del pipe respostes");
		exit (-1);
	}

	for (int i = 0; i < processos; i++) {

		pidCalculadors[i] = fork();

		if (pidCalculadors[i] < 0 ) {
			// Cas d'error del fork()
			perror("ERROR en el fork del Calculador");
			exit(-1);

		} else if (pidCalculadors[i] == 0) {
			// Creació del procés fill Calculador
			// Dupliquem el descriptor de lectura de nombres al descriptor de fitxers 11 i el tanquem
			dup2(nombres[0], NOMBRES_OUT);
			close(nombres[0]);
			close(nombres[1]);

			// Fem el mateix amb el descriptor de escriptura de respostes posant-lo al 20
			dup2(respostes[1], RESPOSTES_IN);
			close(respostes[0]);
			close(respostes[1]);

			//Transformem aquest procés en un procés calculador
			if(execlp("./calculador", "calculador", NULL) < 0) {
				// En cas d'error en el recobriment
				perror("ERROR en el recobriment del calculador");
				exit(-1);
			}
			exit(0);
		}
	}
	// Dupliquem la lectura del pipe respostes al descriptor 21
	dup2(respostes[0],RESPOSTES_OUT);
	close(respostes[1]);
	close(respostes[0]);

	// Tanquem els descriptos no utilitzats pel pare
	close(nombres[1]);
	close(nombres[0]);

	int contadorPrimers = 0;

	t_infoNombre infoNombre;

	while((read(RESPOSTES_OUT, &infoNombre, sizeof(t_infoNombre))) > 0) {
		char buffer[100];
		sprintf(buffer, "Controlador: rebut del Calculador PID %d: nombre %d és primer? %c \n", infoNombre.pid, infoNombre.nombre, infoNombre.primer);

		if(infoNombre.primer == 'S') {
			contadorPrimers++;
		}

		// Mostrar els resultats dels calculadors i controlar l'error si n'hi ha
		if(write(1,buffer,strlen(buffer)) < 0){
          perror("Hi ha un error al escriure a la sortida: ");
          exit(-1);
      }
	}

	if((read(RESPOSTES_OUT, &infoNombre, sizeof(t_infoNombre))) < 0) {
		perror("Error al llegir els resultats dels calculadors");
      exit(-1);
	}

	// Enviem senyal SIGTERM al generador
	if(kill(pidGenerador, SIGTERM) < 0) {
		perror("Error en la senyal SIGTERM en el generador");
		exit(-1);
	}
	// Esperem la finalització del generador
	wait(NULL);

	int primersOut = 0;
	int exitCalculadors = 0;

	// Enviem senyal SIGTERM als calculadors
	for (int i = 0; i < processos ; i++) {
		if(kill(pidCalculadors[i], SIGTERM) < 0) {
			perror("Error en la senyal SIGTERM en el calculador");
			exit(-1);
		}
		// Esperem la finalització del calculador en curs
		wait(&exitCalculadors);
		primersOut += WEXITSTATUS(exitCalculadors);
	}

	// Mostrar resultats
	char resultats[300];
	sprintf(resultats, "Controlador: nombrePrimersPipe = %d nombrePrimersExit = %d \n", contadorPrimers, primersOut);
	if(write (1, resultats, strlen(resultats)) < 0) {
		perror("Error en la sortida");
      exit(-1);
	}
	exit(0);
}
