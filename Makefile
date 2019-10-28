all:
	gcc controlador.c -o controlador
	gcc generador.c -o generador
	gcc calculador.c -o calculador
	./controlador.c 3 7   
