#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>

#include "registro.hpp"
#include "usuario.hpp"
#include "Partida.hpp"
#include "funcionservidor.c"

#define MSG_SIZE 250
#define MAX_CLIENTS 30
#define MAX_PARTIDAS 10

/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[], std::vector<Usuario> & usuariosConectados);

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
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];

    int on, ret;

    /*---------------------------------------------------
		Variables para el seguimiento de partidas
	---------------------------------------------------*/
	Registro registroUsers;
	registroUsers.leeUsuarios();

	std::vector<Usuario> usuariosConectados;
	usuariosConectados.resize(MAX_CLIENTS);

	std::vector<Partida> partidas;
	partidas.resize(MAX_PARTIDAS);

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
	sockname.sin_port = htons(2050);
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

            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)

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
                                if(numClientes < MAX_CLIENTS){
                                    arrayClientes[numClientes] = new_sd;
									usuariosConectados[numClientes].setSocket(new_sd);
									usuariosConectados[numClientes].setEstado(USUARIO_SIN_IDENTIFICAR);
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);

                                    strcpy(buffer, "+Ok. Usuario conectado\n");

                                    send(new_sd,buffer,strlen(buffer),0);
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"-Err. Demasiados usuarios conectados\n");
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

                                for (j = 0; j < numClientes; j++){
                                    send(arrayClientes[j], "-Err. Servidor desconectado\n", strlen("-Err. Servidor desconectado\n"),0);
                                    close(arrayClientes[j]);
                                    FD_CLR(arrayClientes[j],&readfds);
                                }
                                    close(sd);
                                    exit(-1);


                            }
                            //Mensajes que se quieran mandar a los clientes (implementar)

                        }
                        else{
                            bzero(buffer,sizeof(buffer));

                            recibidos = recv(i,buffer,sizeof(buffer),0);

                            if(recibidos > 0){

								//TODO PARTE QUE HAY QUE CAMBIAR

								std::string strbuffer(buffer);
								std::vector <std::string> arg;
								arg.resize(2);
								int n = parseo(strbuffer, arg);
								int index;
								for(index = 0; index < MAX_CLIENTS; index++)
									if(usuariosConectados[index].getSocket() == i);
										break;

								std::cout << "i: " << i << " index: " << index << std::endl; 

								switch(n){
									//USUARIO
									case 1:
/*
										if(usuariosConectados[index].estaSinIdentificar()){
											
										}
										else{
				                            bzero(buffer,sizeof(buffer));
				                            strcpy(buffer,"-Err. No puede volver a identificarse\n");
				                            send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
										}
*/
							            bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Caso 1 ");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
									break;
									//PASSWORD
									case 2:
							            bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Caso 2 ");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
									break;
									//REGISTRO
									case 3:
							            bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Caso 3 ");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
									break;
									//INICIAR_PARTIDA
									case 4:
							            bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Caso 4 ");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
									break;
									//DESCUBRIR
									case 5:
							            bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Caso 5 ");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
									break;
									//PONER_BANDERA
									case 6:
							            bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Caso 6 ");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
									break;
									//SALIR
									case 7:
							            bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Caso 7 ");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
										//salirCliente(i,&readfds,&numClientes,arrayClientes,usuariosConectados);
									break;
									default:
										bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"Error");
		                                send(usuariosConectados[index].getSocket(),buffer,strlen(buffer),0);
								}
								/////////////////////////////////


                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos == 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,arrayClientes,usuariosConectados);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;

}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[], std::vector<Usuario> & usuariosConectados){

    char buffer[250];
    int j;

    close(socket);
    FD_CLR(socket,readfds);

    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);

    //Re-estructurar el array de usuarios
    for (j = 0; j < (*numClientes) - 1; j++)
        if (usuariosConectados[j].getSocket() == socket){
            break;
		}
    for (; j < (*numClientes) - 1; j++)
        (usuariosConectados[j] = usuariosConectados[j+1]);

	usuariosConectados[j].setEstado(USUARIO_VACIO);

    (*numClientes)--;

    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);

    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}
