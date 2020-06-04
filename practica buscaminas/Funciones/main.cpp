#include <iostream>

#include "Partida.hpp"

int main(){
	Partida partida;
	int jugador = 0;
	int comando;
	int i,j;

	do{
		partida.mostrarPartida();
		std::cout << partida.getTablero() << std::endl;

		std::cout << "Jugador: " << jugador <<std::endl;
		std::cout << "Introduzca comando: ";
		std::cin >> comando;

		if(comando < 100){
			i = comando / 10;
			comando = comando - (10 * i);
			j = comando;

			partida.descubrirCasilla(i,j,jugador);
		}
		else{
			comando = comando - 100;
			i = comando / 10;
			comando = comando - (10 * i);
			j = comando;

			partida.setBandera(i,j,jugador);
		}

		jugador = (jugador+1)%2;
	}while(!partida.partidaTerminada());

	partida.mostrarPartida();
	std::cout << partida.getTablero() << std::endl;
}
