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


#define MSG_SIZE 250
#define MAX_CLIENTS 4


/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numGenerales, int arrayGenerales[]);

struct votacion {
    int socket;
    int voto;
};

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
    int arrayGenerales[MAX_CLIENTS];
    int numGenerales = 0;
    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];
    
    int on, ret;

    votacion votaciones[MAX_CLIENTS];

    for(int i = 0; i < MAX_CLIENTS;i++){
        votaciones[i].socket = 0;
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
                                    votaciones[numGenerales].socket = new_sd;

                                    int i_aux;
									bool cont = true;
									for(i_aux = 0; i_aux < MAX_CLIENTS && cont; i_aux++){
										if(votaciones[i_aux] == 0){
											cont = false;
											votaciones[i_aux].socket = new_sd;
										}
									}

                                    numGenerales++;
                                    FD_SET(new_sd,&readfds);
                                
                                    strcpy(buffer, "[General conectado]\n");
                                
                                    send(new_sd,buffer,strlen(buffer),0);
                                
                                    for(j=0; j<(numGenerales-1);j++){
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "Nuevo General conectado: %d\n",new_sd);
                                        send(arrayGenerales[j],buffer,strlen(buffer),0);
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
                                    send(arrayGenerales[j], "-Err. Servidor desconectado\n", strlen("-Err. Servidor desconectado\n"),0);
                                    close(arrayGenerales[j]);
                                    FD_CLR(arrayGenerales[j],&readfds);
                                }
                                    close(sd);
                                    exit(-1);


                            }
                            //Mensajes que se quieran mandar a los Generales (implementar)

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
								for(index = 0; index < MAX_CLIENTS && votaciones[index].socket != i; index++);

								switch(n){
									//ATACAR
									case 1:
                                        votaciones[sd].voto = n;
										if(numGenerales == 4){
                                            //AlgBizantino(votaciones);
                                        }else{
                                            for(j=0; j<(numGenerales-1);j++){
                                                bzero(buffer,sizeof(buffer));
                                                sprintf(buffer, "Faltan generales por votar\n");
                                                send(arrayGenerales[j],buffer,strlen(buffer),0);
                                            }
                                        }
										break;
									//RENDIRSE
									case 2:
							            votaciones[sd].voto = n;
										if(numGenerales == 4){
                                            //AlgBizantino(votaciones);
                                        }else{
                                            for(j=0; j<(numGenerales-1);j++){
                                                bzero(buffer,sizeof(buffer));
                                                sprintf(buffer, "Faltan generales por votar\n");
                                                send(arrayGenerales[j],buffer,strlen(buffer),0);
                                            }
                                        }
										break;
									//SALIR
									case 3:
										salirCliente(i,&readfds,&numGenerales,arrayGenerales,usuariosConectados);
										break;
								}
								////////////////////////////////
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numGenerales,arrayGenerales);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numGenerales, int arrayGenerales[]){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de Generales
    for (j = 0; j < (*numGenerales) - 1; j++)
        if (arrayGenerales[j] == socket)
            break;
    for (; j < (*numGenerales) - 1; j++)
        (arrayGenerales[j] = arrayGenerales[j+1]);
    
    (*numGenerales)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numGenerales); j++)
        if(arrayGenerales[j] != socket)
            send(arrayGenerales[j],buffer,strlen(buffer),0);


}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}

int AlgBizantino(struct votacion votos){
    
}