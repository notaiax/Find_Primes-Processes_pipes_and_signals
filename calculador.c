/* -----------------------------------------------------------------------
PRA1:  Processos, pipes i senyals: Primers - Pralab 1
Codi font: calculador.c
Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "proces.h"

int totalPrimers;

char numPrimer (int num);

void sig_handler();

int main(int argc, char const *argv[]) {

	int nombre;
	t_infoNombre resultat;

	while ((read(NOMBRES_OUT, &nombre, sizeof(int))) > 0) {

		resultat.pid = getpid();
		resultat.nombre = nombre;
		resultat.primer = numPrimer(nombre);

		if(resultat.primer == 'S') {
			totalPrimers++;
		}

		if(write(RESPOSTES_IN, &resultat, sizeof(t_infoNombre)) < 0) {
			perror("Error en la escriptura en el pipe respostes");
			exit(-1);
		}
	}

	if((read(NOMBRES_OUT, &nombre, sizeof(int))) < 0) {
		perror("Error en la lectura del pipe nombres");
		exit(-1);
	}

	close(NOMBRES_OUT);
	close(RESPOSTES_IN);

	signal(SIGTERM, sig_handler);
	pause();		// esperar fins a rebre una senyal
}

char numPrimer(int num) {
	int cont = 2;

		while (cont < num) {
			if (num % cont == 0) {
				return 'N';
			}
			cont++;
		}
		return 'S';
}

void sig_handler() {
	exit(totalPrimers);
}
