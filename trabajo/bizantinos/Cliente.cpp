#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>

#include "macros.hpp"

void imprimeTablero(std::string str);

int main (int argc,char const *argv[]){


	if(argc!=2) {
		printf("[Error]Sintaxis: ./[programa] [ip]\n");
		exit(1);
	}

	/*----------------------------------------------------
		Descriptor del socket y buffer de datos
	-----------------------------------------------------*/
	int sd;
	struct sockaddr_in sockname;
	char buffer[250];
	socklen_t len_sockname;
    fd_set readfds, auxfds;
    int salida;
    int fin = 0;


	/* --------------------------------------------------
		Se abre el socket
	---------------------------------------------------*/
	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
		exit (1);
	}



	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2050);
	sockname.sin_addr.s_addr =  inet_addr(argv[1]);

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	len_sockname = sizeof(sockname);

	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}

    //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);

    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);


	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do
	{
        auxfds = readfds;
        salida = select(sd+1,&auxfds,NULL,NULL,NULL);

        //Tengo mensaje desde el servidor
        if(FD_ISSET(sd, &auxfds)){

            bzero(buffer,sizeof(buffer));
            recv(sd,buffer,sizeof(buffer),0);

			std::string strbuffer(buffer);
            if(strbuffer.find("+Ok. Partida:") == 0){
				std::cout << buffer << std::endl;
				size_t pointer = strbuffer.find("+Ok. Partida:");
				pointer = pointer + 13;
				imprimeTablero(strbuffer.substr(pointer));
			}
			else{
				if(strbuffer.find("+Ok") == 0){
					std::cout << BGREEN << "+Ok." << RESET << strbuffer.substr(strbuffer.find("+Ok.") + 4);
				}
				else{
					std::cout << BRED << "+Err." << RESET << strbuffer.substr(strbuffer.find("+Err.") + 6);
				}
			}

            if(strcmp(buffer,"-Err. Servidor desconectado\n") == 0)
                fin =1;
            if(strcmp(buffer,"-Err. Demasiados usuarios conectados\n") == 0)
                fin =1;

        }
        else
        {

            //He introducido información por teclado
            if(FD_ISSET(0,&auxfds)){
                bzero(buffer,sizeof(buffer));

                fgets(buffer,sizeof(buffer),stdin);

                if(strcmp(buffer,"SALIR\n") == 0){
                        fin = 1;

                }

                send(sd,buffer,sizeof(buffer),0);

            }


        }



    }while(fin == 0);

    close(sd);

    return 0;

}

void imprimeTablero(std::string str){
	char tablero[300];
	strcpy(tablero, str.c_str());

	int indice = 0;

	std::cout << "┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐" <<std::endl;
	std::cout << "│   │ A │ B │ C │ D │ E │ F │ G │ H │ I │ J │" <<std::endl;

	for(int i = 0; i < 10; i++){
		std::cout << "├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤" <<std::endl;

		std::cout << "│ "<<i<<" │";

		for(int j = 0; j < 10; j++){
			if(tablero[indice] == '-'){
				std::cout << "   │";
			}
			else if(tablero[indice] == 'a' || tablero[indice] == 'b'){
				if((tablero[indice] == 'a' && tablero[indice + 1] == ',') || (tablero[indice] == 'a' && tablero[indice + 1] == ';'))
					std::cout << BIRED << " þ " << RESET << "│";
				if((tablero[indice] == 'b' && tablero[indice + 1] == ',') || (tablero[indice] == 'b' && tablero[indice + 1] == ';'))
					std::cout << BIBLUE << " þ " << RESET << "│";
				if(tablero[indice] == 'a' && tablero[indice + 1] == 'b'){
					std::cout << BIBLUE << "þ " << BIRED << "þ" << RESET << "│";
					indice++;
				}
			}
			else{
				if(tablero[indice] == '*') std::cout << " * │";
				else{
					if(tablero[indice] == '0') std::cout << " 0 │";
					if(tablero[indice] == '1') std::cout << BBLUE << " 1" << RESET << " │";
					if(tablero[indice] == '2') std::cout << BGREEN << " 2" << RESET << " │";
					if(tablero[indice] == '3') std::cout << BRED << " 3" << RESET << " │";
					if(tablero[indice] == '4') std::cout << BPURPLE << " 4" << RESET << " │";
					if(tablero[indice] == '5') std::cout << BWHITE << " 5" << RESET << " │";
					if(tablero[indice] == '6') std::cout << BCYAN << " 6" << RESET << " │";
					if(tablero[indice] == '7') std::cout << BYELLOW << " 7" << RESET << " │";
					if(tablero[indice] == '8') std::cout << BBLACK << " 8" << RESET << " │";
				}
			}

			indice = indice + 2;
		}

		std::cout << std::endl;
	}

	std::cout << "└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘" <<std::endl;
}
