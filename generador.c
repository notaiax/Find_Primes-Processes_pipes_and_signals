/* -----------------------------------------------------------------------
PRA1:  Processos, pipes i senyals: Primers - Pralab 1
Codi font: generador.c
Àiax Faura Vilalta
---------------------------------------------------------------------- */
#include "proces.h"

int main (int argc, char *argv[]) {

	int N = atoi(argv[1]);

	// Generador escriu la seqüència de N enters al pipe nombres
	for(int i=2; i <= N; i++) {
		if ((write(NOMBRES_IN, &i, sizeof(int))) < 0) {
			perror("Error al escriure al pipe nombres");
      	exit(-1);
		}
	}

	//close(nombres[0]);
	close(NOMBRES_IN);

	// Esperem una senyal per acabar
	signal(SIGTERM, SIG_DFL);
	pause();
}
