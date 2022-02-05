#Programa makefile para compilar el proyecto
Diccionario : Diccionario.c
	gcc Diccionario.c -o Diccionario
#Limpiar	
clean:
	rm -f *.o Diccionario
