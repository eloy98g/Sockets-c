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
#include "macros.hpp"
#include "funcionservidor.c"

#define MSG_SIZE 250
#define MAX_CLIENTS 30
#define MAX_PARTIDAS 10

/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[], std::vector<Usuario> & usuariosConectados);
bool is_number(const std::string& s);

int arrayClientes[MAX_CLIENTS];
int numClientes = 0;
int sd;

int main ( )
{

	/*----------------------------------------------------
		Descriptor del socket y buffer de datos
	-----------------------------------------------------*/
	int new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
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

	int socket1, socket2, idPartida;

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

									int i_aux;
									bool cont = true;
									for(i_aux = 0; i_aux < MAX_CLIENTS && cont; i_aux++){
										if(usuariosConectados[i_aux].estaVacio()){
											cont = false;
											usuariosConectados[i_aux].setSocket(new_sd);
										}
									}
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);

									if(numClientes <= (MAX_CLIENTS / 2)){
										std::cout << "Nuevo usuario conectado " << BGREEN << "(" << numClientes << "/" << MAX_CLIENTS << ")" << RESET << std::endl;
									}
									else if(numClientes != MAX_CLIENTS){
										std::cout << "Nuevo usuario conectado " << BYELLOW << "(" << numClientes << "/" << MAX_CLIENTS << ")" << RESET << std::endl;
									}
									else{
										std::cout << "Nuevo usuario conectado " << BRED << "(" << numClientes << "/" << MAX_CLIENTS << ")" << RESET << std::endl;
									}

                                    strcpy(buffer, "+Ok. Usuario conectado\n");
                                    send(usuariosConectados[i_aux - 1].getSocket(),buffer,sizeof(buffer),0);
									usuariosConectados[i_aux - 1].setEstado(USUARIO_DESCONOCIDO);
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"-Err. Demasiados usuarios conectados\n");
                                    send(new_sd,buffer,sizeof(buffer),0);
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

								std::string strbuffer(buffer);
								std::vector <std::string> arg;
								arg.resize(2);
								int n = parseo(strbuffer, arg);
								int index;
								for(index = 0; index < MAX_CLIENTS && usuariosConectados[index].getSocket() != i; index++);

								switch(n){
									//USUARIO
									case 1:
										if(registroUsers.checkNombre(arg[0])){
											bool estaYaIniciado = false;
											for(int i_aux = 0; i_aux < MAX_CLIENTS && !estaYaIniciado; i_aux++){
												if(usuariosConectados[i_aux].getNombre() == arg[0] && !usuariosConectados[i_aux].estaVacio())
													estaYaIniciado = true;
											}
											if(estaYaIniciado){
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"-Err. Este usuario ya está siendo usado\n");
												send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
											}
											else{
												usuariosConectados[index].setEstado(USUARIO_SIN_IDENTIFICAR);
												usuariosConectados[index].setNombre(arg[0]);
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"+Ok. Usuario correcto\n");
												send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
											}
										}else if(usuariosConectados[index].estaDesconocido()){
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. Usuario incorrecto\n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}else if(usuariosConectados[index].estaIdentificado()){
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. El usuario ya esta identificado, debe introducir: INICIAR-PARTIDA\n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}else{
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. \n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}
										break;
									//PASSWORD
									case 2:
							            if(usuariosConectados[index].estaSinIdentificar()){
											if(registroUsers.checkPassword(usuariosConectados[index].getNombre(), arg[0])){
												usuariosConectados[index].setEstado(USUARIO_IDENTIFICADO);
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"+Ok. El usuario se ha identificado correctamente\n");
												send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
											}else{
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"-Err. Clave incorrecta\n");
												send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
											}
										}else if(usuariosConectados[index].estaDesconocido()){
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. El usuario no es conocido, debe introducir: REGISTRO\n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}else{
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. El usuario ya esta identificado, debe introducir: INICIAR-PARTIDA\n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}
										break;
									//REGISTRO
									case 3:
							            if(usuariosConectados[index].estaDesconocido()){
											if(!registroUsers.checkNombre(arg[0])){
												Usuario New_user(arg[0], arg[1]);
												usuariosConectados[index].setEstado(USUARIO_IDENTIFICADO);
												usuariosConectados[index].setNombre(arg[0]);
												usuariosConectados[index].setPassword(arg[1]);
												registroUsers.addUsuario(New_user);
												registroUsers.escribeUsuarios();
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"+Ok. El usuario se ha registrado correctamente\n");
												send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
											}else{
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"-Err. El usuario ya existe\n");
												send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
											}
										}
										else if(usuariosConectados[index].estaSinIdentificar()){
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. El usuario esta registrado, debe introducir: PASSWORD\n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}else{
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. El usuario ya esta identificado, debe introducir: INICIAR-PARTIDA\n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}
										break;
									//INICIAR_PARTIDA
									case 4:
										if(usuariosConectados[index].estaEnPartida()){
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. No puede comenzar una partida porque ya está en una\n");
											send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										}
										else{
											if(!usuariosConectados[index].estaIdentificado()){
						                        bzero(buffer,sizeof(buffer));
						                        strcpy(buffer,"-Err. No puede iniciar una partida sin identificarse\n");
						                        send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
											}
											else{
												int foundOponent = -1;
												for(int i_aux = 0; i_aux < MAX_CLIENTS && foundOponent == -1; i_aux++){
													if(usuariosConectados[i_aux].estaEsperandoPartida()) foundOponent = i_aux;
												}

												if(foundOponent == -1){
													usuariosConectados[index].setEstado(ESPERANDO_PARTIDA);

								                    bzero(buffer,sizeof(buffer));
								                    strcpy(buffer,"+Ok. Ahora mismo no hay ningún contrincante disponible, espere por favor.\n");
								                    send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
												}
												else{
													int foundPartida = -1;

													for(int i_aux = 0; i_aux < MAX_PARTIDAS && foundPartida == -1; i_aux++){
														if(partidas[i_aux].partidaTerminada()) foundPartida = i_aux;
													}

													if(foundPartida == -1){
														usuariosConectados[index].setEstado(ESPERANDO_PARTIDA);

										                bzero(buffer,sizeof(buffer));
										                strcpy(buffer,"+Ok. No se pueden comenzar más partidas, espere por favor.\n");
										                send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
													}
													else{
														usuariosConectados[index].setEstado(foundPartida);
														usuariosConectados[foundOponent].setEstado(foundPartida);

														partidas[foundPartida].restart();

										                bzero(buffer,sizeof(buffer));
										                strcpy(buffer,"+Ok. Se ha encontrado una partida, pueden empezar.\n");
										                send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										                send(usuariosConectados[foundOponent].getSocket(),buffer,sizeof(buffer),0);

														std::string tablero = partidas[foundPartida].getTablero();
														tablero = "+Ok. Partida:" + tablero;

														bzero(buffer,sizeof(buffer));
										                strcpy(buffer,tablero.c_str());
										                send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
										                send(usuariosConectados[foundOponent].getSocket(),buffer,sizeof(buffer),0);

														std::cout << "Comienza la partida " << foundPartida << std::endl;
														partidas[foundPartida].mostrarPartidaCompleta();

														partidas[foundPartida].setJ1(index);
														partidas[foundPartida].setJ2(foundOponent);

										                bzero(buffer,sizeof(buffer));
										                strcpy(buffer,"+Ok. Su turno:\n");
														if(partidas[foundPartida].getIDJugadorActual() == index){
															send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
														}
														else{
															send(usuariosConectados[foundOponent].getSocket(),buffer,sizeof(buffer),0);
														}
													}
												}
											}
										}
										break;
									//DESCUBRIR
									case 5:
										socket1 = usuariosConectados[index].getSocket();

										if(!usuariosConectados[index].estaEnPartida()){
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. No puede usar este comando si no esta en partida\n");
											send(socket1,buffer,sizeof(buffer),0);
										}
										else{
											idPartida = usuariosConectados[index].getPartida();
											socket2 = usuariosConectados[partidas[idPartida].getOponente(index)].getSocket();

											if(partidas[idPartida].getIDJugadorActual() != index){
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"-Err. Debe esperar su turno\n");
												send(socket1,buffer,sizeof(buffer),0);
											}
											else{
												if (is_number(arg[0]) == false && is_number(arg[1]) == false ||
													is_number(arg[0]) == true && is_number(arg[1]) == true ||
													is_number(arg[0]) == false && arg[0].length() != 1 ||
													is_number(arg[1]) == false && arg[1].length() != 1 ){
													bzero(buffer,sizeof(buffer));
													strcpy(buffer,"-Err. Coordenadas erróneas\n");
													send(socket1,buffer,sizeof(buffer),0);
												}
												else{
													int coord1;
													int coord2;
													if(is_number(arg[0])){
														coord1 = atoi(arg[0].c_str());
														coord2 = tolower(arg[1].c_str()[0]) - 97;
													}
													else{
														coord1 = atoi(arg[1].c_str());
														coord2 = tolower(arg[0].c_str()[0]) - 97;
													}

													if(checkCoords(coord1, coord2) == false){
														bzero(buffer,sizeof(buffer));
														strcpy(buffer,"-Err. Coordenadas erróneas\n");
														send(socket1,buffer,sizeof(buffer),0);
													}
													else{
														if(partidas[idPartida].puedeDescubrirCasilla(coord1,coord2) == false){
															bzero(buffer,sizeof(buffer));
															strcpy(buffer,"-Err. No puede descubrir esa casilla\n");
															send(socket1,buffer,sizeof(buffer),0);
														}
														else{
															partidas[idPartida].descubrirCasilla(coord1,coord2);

															std::string tablero = partidas[idPartida].getTablero();
															tablero = "+Ok. Partida:" + tablero;

															bzero(buffer,sizeof(buffer));
												            strcpy(buffer,tablero.c_str());
												            send(socket1,buffer,sizeof(buffer),0);
												            send(socket2,buffer,sizeof(buffer),0);

															if(partidas[idPartida].partidaTerminada()){
																if(partidas[idPartida].getGanador() == index){
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Enhorabuena, has ganado.\n");
																    send(socket1,buffer,sizeof(buffer),0);
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Lo siento, has perdido.\n");
																    send(socket2,buffer,sizeof(buffer),0);
																}
																else{
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Enhorabuena, has ganado.\n");
																    send(socket2,buffer,sizeof(buffer),0);
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Lo siento, has perdido.\n");
																    send(socket1,buffer,sizeof(buffer),0);
																}
																usuariosConectados[index].setEstado(USUARIO_IDENTIFICADO);
																usuariosConectados[partidas[idPartida].getOponente(index)].setEstado(USUARIO_IDENTIFICADO);

																//Comenzamos una nueva partida
																int j1 = -1, j2 = -1;

																for(int i_aux = 0; i_aux < MAX_CLIENTS; i_aux++){
																	if(usuariosConectados[i_aux].estaEsperandoPartida() && j1 == -1) j1 = i_aux;
																	else if(usuariosConectados[i_aux].estaEsperandoPartida() && j2 == -1) j2 = i_aux;
																}

																if(j1 != -1 && j2 != -1){
																	usuariosConectados[j1].setEstado(idPartida);
																	usuariosConectados[j2].setEstado(idPartida);

																	partidas[idPartida].restart();

																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Se ha encontrado una partida, pueden empezar.\n");
																    send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
																    send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

																	std::string tablero = partidas[idPartida].getTablero();
																	tablero = "+Ok. Partida:" + tablero;

																	bzero(buffer,sizeof(buffer));
																    strcpy(buffer,tablero.c_str());
																    send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
																    send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

																	std::cout << "Comienza la partida " << idPartida << std::endl;
																	partidas[idPartida].mostrarPartidaCompleta();

																	partidas[idPartida].setJ1(j1);
																	partidas[idPartida].setJ2(j2);

																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Su turno:\n");
																	if(partidas[idPartida].getIDJugadorActual() == j1){
																		send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
																	}
																	else{
																		send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);
																	}
																}
															}
															else{
														        bzero(buffer,sizeof(buffer));
														        strcpy(buffer,"+Ok. Su turno:\n");
																if(partidas[idPartida].getIDJugadorActual() == index){
																	send(socket1,buffer,sizeof(buffer),0);
																}
																else{
																	send(socket2,buffer,sizeof(buffer),0);
																}
															}
														}
													}
												}
											}
										}
										break;
									//PONER_BANDERA
									case 6:
										socket1 = usuariosConectados[index].getSocket();

										if(!usuariosConectados[index].estaEnPartida()){
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-Err. No puede usar este comando si no esta en partida\n");
											send(socket1,buffer,sizeof(buffer),0);
										}
										else{
											idPartida = usuariosConectados[index].getPartida();
											socket2 = usuariosConectados[partidas[idPartida].getOponente(index)].getSocket();

											if(partidas[idPartida].getIDJugadorActual() != index){
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"-Err. Debe esperar su turno\n");
												send(socket1,buffer,sizeof(buffer),0);
											}
											else{
												if (is_number(arg[0]) == false && is_number(arg[1]) == false ||
													is_number(arg[0]) == true && is_number(arg[1]) == true ||
													is_number(arg[0]) == false && arg[0].length() != 1 ||
													is_number(arg[1]) == false && arg[1].length() != 1 ){
													bzero(buffer,sizeof(buffer));
													strcpy(buffer,"-Err. Coordenadas erróneas\n");
													send(socket1,buffer,sizeof(buffer),0);
												}
												else{
													int coord1;
													int coord2;
													if(is_number(arg[0])){
														coord1 = atoi(arg[0].c_str());
														coord2 = tolower(arg[1].c_str()[0]) - 97;
													}
													else{
														coord1 = atoi(arg[1].c_str());
														coord2 = tolower(arg[0].c_str()[0]) - 97;
													}

													if(checkCoords(coord1, coord2) == false){
														bzero(buffer,sizeof(buffer));
														strcpy(buffer,"-Err. Coordenadas erróneas\n");
														send(socket1,buffer,sizeof(buffer),0);
													}
													else{
														if(partidas[idPartida].puedePonerBandera(coord1,coord2) == false){
															bzero(buffer,sizeof(buffer));
															strcpy(buffer,"-Err. No puede poner bandera en esa casilla\n");
															send(socket1,buffer,sizeof(buffer),0);
														}
														else{
															partidas[idPartida].setBandera(coord1,coord2);

															std::string tablero = partidas[usuariosConectados[index].getPartida()].getTablero();
															tablero = "+Ok. Partida:" + tablero;

															bzero(buffer,sizeof(buffer));
												            strcpy(buffer,tablero.c_str());
												            send(socket1,buffer,sizeof(buffer),0);
												            send(socket2,buffer,sizeof(buffer),0);

															if(partidas[idPartida].partidaTerminada()){
																if(partidas[idPartida].getGanador() == index){
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Enhorabuena, has ganado.\n");
																    send(socket1,buffer,sizeof(buffer),0);
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Lo siento, has perdido.\n");
																    send(socket2,buffer,sizeof(buffer),0);
																}
																else{
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Enhorabuena, has ganado.\n");
																    send(socket2,buffer,sizeof(buffer),0);
																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Lo siento, has perdido.\n");
																    send(socket1,buffer,sizeof(buffer),0);
																}
																usuariosConectados[index].setEstado(USUARIO_IDENTIFICADO);
																usuariosConectados[partidas[idPartida].getOponente(index)].setEstado(USUARIO_IDENTIFICADO);

																//Comenzamos una nueva partida
																int j1 = -1, j2 = -1;

																for(int i_aux = 0; i_aux < MAX_CLIENTS; i_aux++){
																	if(usuariosConectados[i_aux].estaEsperandoPartida() && j1 == -1) j1 = i_aux;
																	else if(usuariosConectados[i_aux].estaEsperandoPartida() && j2 == -1) j2 = i_aux;
																}

																if(j1 != -1 && j2 != -1){
																	usuariosConectados[j1].setEstado(idPartida);
																	usuariosConectados[j2].setEstado(idPartida);

																	partidas[idPartida].restart();

																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Se ha encontrado una partida, pueden empezar.\n");
																    send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
																    send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

																	std::string tablero = partidas[idPartida].getTablero();
																	tablero = "+Ok. Partida:" + tablero;

																	bzero(buffer,sizeof(buffer));
																    strcpy(buffer,tablero.c_str());
																    send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
																    send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

																	std::cout << "Comienza la partida " << idPartida << std::endl;
																	partidas[idPartida].mostrarPartidaCompleta();

																	partidas[idPartida].setJ1(j1);
																	partidas[idPartida].setJ2(j2);

																    bzero(buffer,sizeof(buffer));
																    strcpy(buffer,"+Ok. Su turno:\n");
																	if(partidas[idPartida].getIDJugadorActual() == j1){
																		send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
																	}
																	else{
																		send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);
																	}
																}
															}
															else{
														        bzero(buffer,sizeof(buffer));
														        strcpy(buffer,"+Ok. Su turno:\n");
																if(partidas[idPartida].getIDJugadorActual() == index){
																	send(socket1,buffer,sizeof(buffer),0);
																}
																else{
																	send(socket2,buffer,sizeof(buffer),0);
																}
															}
														}
													}
												}
											}
										}
										break;
									//SALIR
									case 7:
							            if(usuariosConectados[index].estaEnPartida()){
											idPartida = usuariosConectados[index].getPartida();
											socket1 = usuariosConectados[index].getSocket();
											socket2 = usuariosConectados[partidas[idPartida].getOponente(index)].getSocket();

											partidas[idPartida].terminarPartida();

											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"+Ok. Tu oponente ha terminado la partida.\n");
											send(socket2,buffer,sizeof(buffer),0);

											usuariosConectados[partidas[idPartida].getOponente(index)].setEstado(USUARIO_IDENTIFICADO);

											//Comenzamos una nueva partida
											int j1 = -1, j2 = -1;

											for(int i_aux = 0; i_aux < MAX_CLIENTS; i_aux++){
												if(usuariosConectados[i_aux].estaEsperandoPartida() && j1 == -1) j1 = i_aux;
												else if(usuariosConectados[i_aux].estaEsperandoPartida() && j2 == -1) j2 = i_aux;
											}

											if(j1 != -1 && j2 != -1){
												usuariosConectados[j1].setEstado(idPartida);
												usuariosConectados[j2].setEstado(idPartida);

												partidas[idPartida].restart();

												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"+Ok. Se ha encontrado una partida, pueden empezar.\n");
												send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
												send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

												std::string tablero = partidas[idPartida].getTablero();
												tablero = "+Ok. Partida:" + tablero;

												bzero(buffer,sizeof(buffer));
												strcpy(buffer,tablero.c_str());
												send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
												send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

												std::cout << "Comienza la partida " << idPartida << std::endl;
												partidas[idPartida].mostrarPartidaCompleta();

												partidas[idPartida].setJ1(j1);
												partidas[idPartida].setJ2(j2);

												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"+Ok. Su turno:\n");
												if(partidas[idPartida].getIDJugadorActual() == j1){
													send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
												}
												else{
													send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);
												}
											}
										}

										salirCliente(i,&readfds,&numClientes,arrayClientes,usuariosConectados);
										break;
									default:
										bzero(buffer,sizeof(buffer));
		                                strcpy(buffer,"-Err. Comando desconocido.\n");
		                                send(usuariosConectados[index].getSocket(),buffer,sizeof(buffer),0);
								}
								/////////////////////////////////


                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos == 0){
								int index;
								for(index = 0; index < MAX_CLIENTS && usuariosConectados[index].getSocket() != i; index++);

								if(usuariosConectados[index].estaEnPartida()){
									idPartida = usuariosConectados[index].getPartida();
									socket1 = usuariosConectados[index].getSocket();
									socket2 = usuariosConectados[partidas[idPartida].getOponente(index)].getSocket();

									partidas[idPartida].terminarPartida();

									bzero(buffer,sizeof(buffer));
									strcpy(buffer,"+Ok. Tu oponente ha terminado la partida.\n");
									send(socket2,buffer,sizeof(buffer),0);

									usuariosConectados[partidas[idPartida].getOponente(index)].setEstado(USUARIO_IDENTIFICADO);

									//Comenzamos una nueva partida
									int j1 = -1, j2 = -1;

									for(int i_aux = 0; i_aux < MAX_CLIENTS; i_aux++){
										if(usuariosConectados[i_aux].estaEsperandoPartida() && j1 == -1) j1 = i_aux;
										else if(usuariosConectados[i_aux].estaEsperandoPartida() && j2 == -1) j2 = i_aux;
									}

									if(j1 != -1 && j2 != -1){
										usuariosConectados[j1].setEstado(idPartida);
										usuariosConectados[j2].setEstado(idPartida);

										partidas[idPartida].restart();

										bzero(buffer,sizeof(buffer));
										strcpy(buffer,"+Ok. Se ha encontrado una partida, pueden empezar.\n");
										send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
										send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

										std::string tablero = partidas[idPartida].getTablero();
										tablero = "+Ok. Partida:" + tablero;

										bzero(buffer,sizeof(buffer));
										strcpy(buffer,tablero.c_str());
										send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
										send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);

										std::cout << "Comienza la partida " << idPartida << std::endl;
										partidas[idPartida].mostrarPartidaCompleta();

										partidas[idPartida].setJ1(j1);
										partidas[idPartida].setJ2(j2);

										bzero(buffer,sizeof(buffer));
										strcpy(buffer,"+Ok. Su turno:\n");
										if(partidas[idPartida].getIDJugadorActual() == j1){
											send(usuariosConectados[j1].getSocket(),buffer,sizeof(buffer),0);
										}
										else{
											send(usuariosConectados[j2].getSocket(),buffer,sizeof(buffer),0);
										}
									}
								}

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

	usuariosConectados[j].setEstado(USUARIO_VACIO);

    (*numClientes)--;

	if(*numClientes <= (MAX_CLIENTS / 2)){
		std::cout << "Un usuario se ha desconectado " << BGREEN << "(" << *numClientes << "/" << MAX_CLIENTS << ")" << RESET << std::endl;
	}
	else if(*numClientes != MAX_CLIENTS){
		std::cout << "Un usuario se ha desconectado " << BYELLOW << "(" << *numClientes << "/" << MAX_CLIENTS << ")" << RESET << std::endl;
	}
	else{
		std::cout << "Un usuario se ha desconectado " << BRED << "(" << *numClientes << "/" << MAX_CLIENTS << ")" << RESET << std::endl;
	}
}


void manejador (int signum){
    std::cout << BRED << std::endl << "Cerrando el servidor" << RESET << std::endl;
    signal(SIGINT,manejador);

    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor

    for (int i = 0; i < numClientes; i++){
		send(arrayClientes[i], "-Err. Servidor desconectado\n", strlen("-Err. Servidor desconectado\n"),0);
		close(arrayClientes[i]);
	}

	close(sd);
    exit(-1);
}

bool is_number(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
