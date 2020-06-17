#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include "funcionservidor.c"

using namespace std;

#define MSG_SIZE 250
#define MAX_CLIENTS 4


/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numGenerales);
int getNumVotos();
int existeComandante();
void algBizantino(int opcion);

struct votacion {
    int socket;
    int voto;
    int comandante;
};
struct votacion generales[MAX_CLIENTS];;

int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int numGenerales = 0;
    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];
    int opcion = 1;
    int on, ret;
    int comandante = 0;//socket del comandante

    for(int i = 0; i < MAX_CLIENTS;i++){
        generales[i].socket = 0;
        generales[i].voto = 0;
        generales[i].comandante = 0;
    }


    
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}
    
    // Activaremos una propiedad del socket que permitir· que otros
    // sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    // Esto permitir· en protocolos como el TCP, poder ejecutar un
    // mismo programa varias veces seguidas y enlazarlo siempre al
    // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    // quedase disponible (TIME_WAIT en el caso de TCP)
    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}
	

   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
		from_len = sizeof (from);


		if(listen(sd,1) == -1){
			perror("Error en la operación de listen");
			exit(1);
		}
    
    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);
    
   	
    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Esperamos recibir mensajes de los Generales (nuevas conexiones o mensajes de los Generales ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
                
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        
                        if( i == sd){

                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                if(numGenerales < MAX_CLIENTS){
                                    int i_aux;
									bool cont = true;
									for(i_aux = 0; i_aux < MAX_CLIENTS && cont; i_aux++){
										if(generales[i_aux].socket == 0){
											cont = false;
											generales[i_aux].socket = new_sd;
                                        }
									}

                                    numGenerales++;
                                    FD_SET(new_sd,&readfds);
                                
                                    printf("Nuevo General conectado | sd: %d\n", new_sd);

                                    strcpy(buffer, "[General conectado]");
                                    send(new_sd,buffer,strlen(buffer),0);
                                
                                    for(j=0; j<(numGenerales-1);j++){
                                        if(generales[j].socket != new_sd){
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "Nuevo General conectado: %d\n",new_sd);
                                            send(generales[j].socket,buffer,sizeof(buffer),0);
                                        }
                                    }
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Demasiados generales conectados\n");
                                    send(new_sd,buffer,strlen(buffer),0);
                                    close(new_sd);
                                }
                                
                            }

                        }
                        else if (i == 0){
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer));
                            fgets(buffer, sizeof(buffer),stdin);

                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0){

                                for (j = 0; j < numGenerales; j++){
                                    bzero(buffer,sizeof(buffer));
                                    sprintf(buffer, "[Error] Servidor desconectado]\n");
                                    send(generales[j].socket,buffer,sizeof(buffer),0);
                                    close(generales[j].socket);
                                    FD_CLR(generales[j].socket,&readfds);
                                }
                                    close(sd);
                                    exit(-1);


                            }
                            if(strcmp(buffer,"OPCIONES\n") == 0){

                                printf("OPCIONES DEL ALGORITMO:\n");
                                printf("1. Todos se comunican con todos (por defecto)\n");
                                printf("2. No todos se comunican con todos\n");
                                printf("Introduzca la opcion: ");
                            }
                            if(strcmp(buffer,"1\n") == 0){
                                printf("HA ELEGIDO LA OPCION 1: TODOS CON TODOS\n");
                                opcion = 1;
                            }
                            if(strcmp(buffer,"2\n") == 0){
                                opcion = 2;
                                printf("HA ELEGIDO LA OPCION 2: NO TODOS CON TODOS\n");
                            }

                        }
                        else{
                            bzero(buffer,sizeof(buffer));
                            
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            
                            if(recibidos > 0){

								std::string strbuffer(buffer);
								std::vector <std::string> arg;
								arg.resize(2);
								int n = parseo(strbuffer, arg);
								int index;
								for(index = 0; index < MAX_CLIENTS && generales[index].socket != i; index++);

								switch(n){
									//ATACAR
									case 1:
                                        if(existeComandante() == 1 && numGenerales == 4){
                                            generales[index].voto = 1;
                                            printf("Voto: Atacar | socket: %d | Num Votos: %d\n", generales[index].socket, getNumVotos());
                                            if(getNumVotos() == 4){
                                                printf("Todos los votos han sido realizados\n");
                                                printf("Lanzando el algoritmo bizantino...\n");
                                                algBizantino(opcion);
                                            }else{
                                                bzero(buffer,sizeof(buffer));
                                                sprintf(buffer, "Faltan generales por votar\n");
                                                send(generales[index].socket,buffer,sizeof(buffer),0);
                                            }
                                        }else if(existeComandante() == 1 && numGenerales < 4){
                                            bzero(buffer,sizeof(buffer));
                                            if(4-numGenerales == 1){
                                                sprintf(buffer, "Falta %d General por conectarse\n", 4-numGenerales);
                                            }else{
                                                sprintf(buffer, "Faltan %d Generales por conectarse\n", 4-numGenerales);
                                            }
                                            send(generales[index].socket,buffer,sizeof(buffer),0);
                                        }
                                        else{
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "Se debe establecer Comandante\n");
                                            send(generales[index].socket,buffer,sizeof(buffer),0);
                                        }
										break;
									//RENDIRSE
									case 2:
                                        if(existeComandante() == 1 && numGenerales == 4){
                                            generales[index].voto = 2;
                                            printf("Voto: Rendir | socket: %d | Num Votos: %d\n", generales[index].socket, getNumVotos());
                                            if(getNumVotos() == 4){
                                                printf("Todos los votos han sido realizados\n");
                                                printf("Lanzando el algoritmo bizantino...\n");
                                                algBizantino(opcion);
                                            }else{
                                                bzero(buffer,sizeof(buffer));
                                                sprintf(buffer, "Faltan generales por votar\n");
                                                send(generales[index].socket,buffer,sizeof(buffer),0);
                                            }
                                        }else if(existeComandante() == 1 && numGenerales < 4){
                                            bzero(buffer,sizeof(buffer));
                                            if(4-numGenerales == 1){
                                                sprintf(buffer, "Falta %d General por conectarse\n", 4-numGenerales);
                                            }else{
                                                sprintf(buffer, "Faltan %d Generales por conectarse\n", 4-numGenerales);
                                            }
                                            send(generales[index].socket,buffer,sizeof(buffer),0);
                                        }
                                        else{
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "Se debe establecer Comandante\n");
                                            send(generales[index].socket,buffer,sizeof(buffer),0);
                                        }
							            
										break;
                                    //COMANDANTE
                                    case 3:
                                        if(existeComandante() == 0){
                                            generales[index].comandante = 1;
                                            comandante = generales[index].socket;
                                            printf("Comandante asignado al General %d\n", comandante);
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "General %d establecido como Comandante\n", generales[index].socket);
                                            send(generales[index].socket,buffer,sizeof(buffer),0);
                                        }else{
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "Ya existe Comandante\n");
                                            send(generales[index].socket,buffer,sizeof(buffer),0);
                                        }
                                        break;
									//SALIR
									case 4:
                                        generales[index].voto = 0;
                                        if(generales[index].socket == comandante){
                                            generales[index].comandante = 0;
                                        }
                                        numGenerales--;
                                        printf("El socket %d, se ha desconectado\n", generales[index].socket);
										salirCliente(i,&readfds,&numGenerales);
										break;
								}
								////////////////////////////////
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numGenerales);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numGenerales){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);

    
    //Re-estructurar el array de Generales
    for (j = 0; j < (*numGenerales) - 1; j++)
        if (generales[j].socket == socket)
            break;
    for (; j < (*numGenerales) - 1; j++)
        (generales[j].socket = generales[j+1].socket);
    
    (*numGenerales)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numGenerales); j++)
        if(generales[j].socket != socket)
            send(generales[j].socket,buffer,strlen(buffer),0);


}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
}

int getNumVotos(){
    int count=0;
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(generales[i].voto != 0){
            count++;
        }
    }
    return count;
}

int existeComandante(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(generales[i].comandante == 1){
            return 1;
        }
    }
    return 0;
}

void algBizantino(int opcion){

    switch(opcion){
        case 1:
            for(int i = 0; i< MAX_CLIENTS;i++){
                
            }
            printf("-");
            break;
        case 2:

            break;
        default:
            break;
    }
}

