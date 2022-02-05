/* Universidad Catolica Andres Bello

Integrantes:

	Vielma Jose
	Vasquez Luis
	Youssef Paolo

Profesores:
	
	Jesus Larez
	Jannelly Bello

*/

#include "Funciones.h" //Libreria con todas las funciones y librerias utilizadas en el programa


//Modo Linea de Comando

void main(int argc, char *argv[]){

	Trie *Raiz = NULL;

	if (argc == 3) {
	
		//Coloca en minusculas los argumentos dados por el usuario
	
		strcpy(argv[1], lowcase(argv[1]));
		strcpy(argv[2], lowcase(argv[2]));
		
		int LineCounter=0;
		extraer("Diccionario.dic", &LineCounter); //Carga del archivo en el diccionario
		Raiz=Inicializar(); 
		Raiz=cargarArchivo(Raiz, LineCounter); 
		
		
		if (strcmp(argv[1], "s") == 0 || (strcmp(argv[1], "a") == 0)){
					
				Busqueda(Raiz, argv[2], argv[1][0]);			
													
				} else {
							
						ErrorCommand(); //Muestra si hay un error en los datos introducidos por el usuario
				}	
								 
	
	} else if (argc > 3 || argc == 2){
				
				ErrorCommand();
	
	
			} else {	//Modo Iterativo
		
						bool verify = false;
		
						char input[60];
						char command[10];
						char word[30];
						int LineCounter=0;
						
						
						printf("\nBienvenido al Diccionario de Sinonimos y Antonimos\n");
						printf("Por favor, introduzca un comando para comenzar.\n");
						
						
						
						while(strcmp(command, "salir")!=0){ 
							
							//Pedido de entradas
							
							fgets(input, 60, stdin); 
							sscanf(input, "%s %s", command, word);
							
							
							strcpy(command, lowcase(command));
							
							if (strcmp(command, "cargar") == 0){
									
								//Carga el archivo
									
								extraer(word, &LineCounter); 
								Raiz=Inicializar(); 
								Raiz=cargarArchivo(Raiz, LineCounter);
								verify = true;
								
							} else if ((strcmp(command, "s") == 0 || strcmp(command, "a") == 0) && (verify ==  true)){
							
									//Busqueda de sinonimos y antonimos
									
									strcpy(word, lowcase(word));
									Busqueda(Raiz, word, command[0]); 
							
										}
									else if(strcmp(command, "e") == 0){	
											
										 //Busqueda de sinonimos y antonimos con RegEx	
											
										 strcpy(word, lowcase(word));
									         filtroRegEx(palabras, word, LineCounter, Raiz); 
										
										
										 }else if (strcmp(command, "ayuda") == 0){
												
												//Muestra los comandos disponibles en el programa
												
												Help();
												
												} else if (verify == false) {
															printf("\nNo se ha cargado ningun archivo\n");
														} else if (strcmp(command, "salir") != 0) {
																	CommandNotFound();
																}

						}
						
				}
	
	printf("\nEl Programa ha finalizado exitosamente.\n"); //Finalizacion del programa
	printf("\n");
	

}

