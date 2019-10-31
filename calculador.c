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
	//Llegim del pipe nombres
	while ((read(11, &nombre, sizeof(int))) > 0) {

		resultat.pid = getpid();
		resultat.nombre = nombre;
		resultat.primer = numPrimer(nombre);

		if(resultat.primer == 'S') {
			totalPrimers++;
		}

		if(write(20, &resultat, sizeof(t_infoNombre)) < 0)
			Error("Error en la escriptura en el pipe respostes");

	}
	if((read(11, &nombre, sizeof(int))) < 0)
		Error("Error en la lectura del pipe nombres");

	close(11);
	close(20);

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
