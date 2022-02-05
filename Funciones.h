/*Librerias utilizadas*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <stdbool.h>
#include<regex.h>

typedef struct diccionario{  //Declaración de la estructura para recoger las palabras del diccionario 

    char palabra1[50]; 
    char palabra2[50]; 
    char SA; 

}Diccionario; 

typedef struct trie{ //Declaración de la estructura del arbol trie 

    char letra;  
    int palabra; 
    struct trie *hijos[27]; 
    char *sinonimos[5]; 
    char *antonimos[5]; 
    int cantidadS; 
    int cantidadA; 

}Trie;

Diccionario *palabras = NULL;  //Declaración del arreglo dinámico de palabras 

char *lowcase(char  *input)  //Convierte la cadena en minúsculas 
{
	for (int index = 0; input[index] != '\0'; ++index){
		input[index] = tolower(input[index]);
	}	
	return input;
}


/*FUNCIONES PARA MANEJAR EL ÁRBOL TRIE*/

Trie* newNode(char letra){  //Crear nuevo nodo del arbol trie 

    Trie *newp; 
    
    if((newp=(Trie *)malloc(sizeof(Trie)))==NULL){  //Reservar espacio en la memoria para el nodo
    
        printf("Error al asginar memoria al nuevo nodo\n");
        exit(1); 
    
    }
    /*Asignar al nodo creado la letra solicitada e incializar los demás elementos*/
    
    newp->letra=letra; 
    newp->palabra=0; 
    
    for(int i=0; i<27; i++){
    
        newp->hijos[i]=NULL; 
    
    }; 
    newp->cantidadS=0; 
    newp->cantidadA=0;  

    return newp;     //Retonar el nuevo nodo

}

Trie *Inicializar(void){  //Inicializar la raiz del arbol trie 

    Trie *Raiz; 
    
    if((Raiz=(Trie *)malloc(sizeof(Trie)))==NULL){  //Reservar espacio para la matriz
    
        printf("Error al asginar memoria al nuevo nodo\n");
        exit(1); 
    
    }  
    
    for(int i=0; i<27; i++){  //Inicializar todos los hijos de la raiz
    
        Raiz->hijos[i]=NULL; 
    
    }  

}

int VerificarSA(char *palabraSA[], int contSA, char *posibleSA){  //Verificar si el sinónimo o antónimo
                                                                  //a insertar ya existe dentro del nodo
    for(int i=0; i<contSA; i++){
    
        if(strcmp(palabraSA[i], posibleSA)==0)  //Retorna 1 si ya existe 
            return 1; 
    
    }
    
    return 0; //Retorna 0 si no existe 

}

void Insertar(Trie **Raiz, char palabra1[], char palabra2[], char SA){  //Insertar una palabra en el trie 
                                                                        //junto con su sinónimo o antónimo 
    Trie *Nodo = *Raiz; 
    int control=0; 
    
    for(int i=0; palabra1[i]; i++){  //Recorre la palabra letra por letra 
    
        control=palabra1[i]-'a'; 
        
        if(control==67){            //En caso de que la letra sea ñ esta se asigna al hijo numero 27
        
            control=27; 
        
        }
        
        if(Nodo->hijos[control]==NULL){  //En caso de que la letra no se encuentre en el arbol, la agrega 
        
            Nodo->hijos[control]=newNode(palabra1[i]);
        
        }
        
        Nodo=Nodo->hijos[control];  //Se mueve al siguiente nodo
    
    }
    
    Nodo->palabra=1;  //Al llegar a la última letra guarda en ella un 1 que confirme que ahi termina una palabra 
    
    if(SA == 's'){  //Verifica si la segunda palabra insertada es un sinónimo 
    
        if(VerificarSA(Nodo->sinonimos, Nodo->cantidadS, palabra2)==0){
    /*Luego de verifica que el sinónimo aun no ha sido agregado, lo agrega en el nodo de la ultima letra de la palabra*/
        
            Nodo->sinonimos[Nodo->cantidadS]=(char *)malloc(sizeof(char)*(strlen(palabra2)+1)); 
            strcpy(Nodo->sinonimos[Nodo->cantidadS], palabra2); 
            Nodo->cantidadS+=1; 
        
        }
    
    }
    else if(SA == 'a'){ //Verifica si la segunda palabra es un antónimo 
    
        if(VerificarSA(Nodo->antonimos, Nodo->cantidadA, palabra2)==0){
     /*Luego de verifica que el antónimo aun no ha sido agregado, lo agrega en el nodo de la ultima letra de la palabra*/
        
            Nodo->antonimos[Nodo->cantidadA]=(char *)malloc(sizeof(char)*(strlen(palabra2)+1)); 
            strcpy(Nodo->antonimos[Nodo->cantidadA], palabra2); 
            Nodo->cantidadA+=1; 
        
        }
    
    }

}

void Busqueda(Trie *Raiz, char palabra[], char SA){  //Buscar la palabra en el trie e imprimie sus
                                                     //sinónimos y/o antónimos
    Trie *Nodo=Raiz;
    int control=0; 
    
    for(int i=0; palabra[i]; i++){  //Recorre el trie en busca de la palabra 
    
        control=palabra[i]-'a'; 
        
        if(control==67){
        
            control=27; 
        
        }
        
        if(Nodo->hijos[control]==NULL){  //Si alguna letra de la palabra no esta significa que la palabra tampoco
                                         //por lo que sale de la función    
            printf("La palabra solicitada no se encuentra en el diccionario\n");
            
            return; 
        
        }
        
        Nodo=Nodo->hijos[control]; 
    
    }
    
    if(Nodo->palabra==1){  //Confirma que en el nodo actual termina una palabra 
    
        if((SA == 's') || (SA == 'e')){  //Verifica que se pida imprimir los sinónimos
        
            if(Nodo->cantidadS>0){  //Verifica que la palabra tenga sinónimos 
            
                for(int i=0; i<Nodo->cantidadS; i++){
                
                    printf("%s", Nodo->sinonimos[i]); 
                    if (i != Nodo->cantidadS - 1){
                    	
                    	printf(", ");
                    		
                    } 
                    
                }
            
                printf("\n \n"); 
            }            
            else {printf("La palabra solicitada no posee sinónimos en el diccionario\n\n");}
        
        }
        
        if((SA == 'a') || (SA == 'e')){  //Verifica que se pida imprimir los antónimos 
        
            if(Nodo->cantidadA>0){  //Verifica que la palabra tenga antónimos 
            
                for(int i=0; i<Nodo->cantidadA; i++){
                
                    printf("%s", Nodo->antonimos[i]); 
                    if (i != Nodo->cantidadA - 1){
                    	
                    	printf(", ");
                    		
                    } 
                    
                }
                
                printf("\n \n"); 
            }            
            else {printf("La palabra solicitada no posee antónimos en el diccionario\n\n");} 
        
        } 
    
    }

}

/*FUNCIONES PARA MANEJAR LA INTERFAZ*/

void Help(){  //Le imrime por pantalla al usuario todas las entradas posibles del programa 
	printf("\nLISTA DE COMANDOS:\n");
	printf("\nTip: Lo que se encuentra dentro de '*', son las entradas dadas por el usuario.\n");
	printf("\n  cargar Nombre : Carga el diccionario desde 'Nombre.dic'.\n");
	printf("\n  s Palabra : Busca los SINONIMOS de la palabra ingresada.\n");
	printf("\n  a Palabra : Busca los ANTONIMOS de la palabra ingresada.  \n");
	printf("\n  e expresion : Muestra los SINONIMOS y ANTONIMOS de las palabras usadas en la Expresion.\n");
	printf("\n  ayuda : Muestra todos los comandos del programa.\n");
	printf("\n  salir : Finaliza el programa.  \n");
	printf("\n");
	

}



void CommandNotFound(){  //Si el usuario inserta un comando incorrecto, le aconseja llamar al comando ayuda 

	printf(" \nEl Comando introducido es incorrecto, por favor, introduzca 'Ayuda' si necesita\n");
	printf("conocer los comandos disponibles en el programa.\n");
	printf("\n");

}

void ErrorCommand(){  //Eror en la linea de comando relacionado a los argumentos dados por el usuario

	printf("\n*Error*\n");
	printf("\nSolo debe insertar el comando 'a' o 's' seguido de la palabra en la linea de comando\n");
	printf("Por favor, introduzca los argumentos de nuevo.\n");
	printf("El formato es el siguiente:\n");
	printf("s palabra\n");
	printf("a palabra\n");

}

/*FUNCIONES PARA EL MANEJO DE ARCHIVOS*/

Diccionario *extraer(char *filename,int *cont)
{   
    FILE *datos;
    char temp[50];

    //abrir el archivo//
    if ((datos = fopen(filename,"r")) == NULL){
        printf("\nNo se pudo abrir el archivo.\n");
        exit(1);}

    //Cuenta las lineas del archivo para asignar tamaño al arreglo dinamico
    while (!feof(datos)){
        fgets(temp,50, datos);
        (*cont)++;
    }

    //Posiciona el apuntador al principio del archivo
    rewind(datos);

    //Reserva memoria dinamica para arreglo dinamico
    if ((palabras = (Diccionario *)malloc((*cont)*sizeof(Diccionario)))== NULL){
        printf("No se ha podido reservar memoria para el arreglo\n");
        exit(1);
    }


    //Extrae contenido del archivo y almacena en arreglo dinamico
    for (int i=0;!feof(datos);i++)
    {
        fgets(temp,50, datos);
        strcpy(temp, lowcase(temp));
        temp[strlen(temp)-1] = '\0';
        sscanf(temp,"%c %s %s",&palabras[i].SA,palabras[i].palabra1,palabras[i].palabra2);
    }

    //Cierra el archivo
    fclose(datos);
    
    return palabras; 
}

//Carga todas las palabras del archivo en el arbol

Trie *cargarArchivo(Trie *Raiz,int n)
{   
    for (int i=0;i<n-1;i++){  //Agrega al trie ambas palabras como sinónimo o antónimo una de la otra y viceversa
        Insertar(&Raiz,palabras[i].palabra1,palabras[i].palabra2,palabras[i].SA);
        Insertar(&Raiz,palabras[i].palabra2,palabras[i].palabra1,palabras[i].SA);
    }
    return Raiz;
}

void mostrarArchivo(Diccionario *palabras, int n){  //Funcion para mostrar lo guardado en el arreglo dinámico 

    for(int i=0; i<n; i++){
    
        printf("%s %s %c\n", palabras[i].palabra1,palabras[i].palabra2,palabras[i].SA); 
    
    }

}

// Funcion de las Expresiones Regulares

bool repeticion(Diccionario *palabras, int posicion, char *palabra){

    for(int i=0; i<posicion; i++){
    
        if(strcmp(palabras[i].palabra1, palabra)==0)
            return true; 
    
    }
    
    return false; 

}

void filtroRegEx(Diccionario *palabras, char *expresion, int LineCounter, Trie *Raiz){  

    regex_t regex;
    int comprobante;  
    
    comprobante=regcomp(&regex, expresion, REG_EXTENDED);  //Asignar la expresión introducida por el usuario
    
    for(int i=0; i<LineCounter; i++){
    
        comprobante=regexec(&regex, palabras[i].palabra1, 0, NULL, 0);   //Comprobar que palabras coinciden con la expresión 
                                                                         //para mandarlas a imprimir 
        if(comprobante==0){
        
            if(repeticion(palabras, i, palabras[i].palabra1)==false){
            
                printf("%s: \n\n", palabras[i].palabra1);  //Imprime la palabra para luego imprimir sus sinónimos y antónimos 
                Busqueda(Raiz, palabras[i].palabra1, 'e');   //si los tiene 
            
            }
                
        }
    
    }  

}










