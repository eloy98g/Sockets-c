#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Tablero.hpp"
#include "macros.hpp"

void Tablero::asignarValorCasilla(int i, int j){
	if(!hasBomba(i,j)){
		int valor = 0;

		for(int coord_i = i - 1; coord_i <= i + 1; coord_i++){
			for(int coord_j = j - 1; coord_j <= j + 1; coord_j++){
				if(coord_i >= 0 && coord_i < 10 && coord_j >= 0 && coord_j < 10){
					if(hasBomba(coord_i, coord_j)) valor++;
				}
			}
		}

		setCasilla(i,j,valor);
	}
}

void Tablero::iniciarTablero(){
	srand(time(NULL));

	int coord_i, coord_j;

	for(int i = 0; i < 20; i++){
		do{
			coord_i = rand()%10;
			coord_j = rand()%10;
		}while(hasBomba(coord_i, coord_j));

		setCasilla(coord_i, coord_j, BOMBA);
	}

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			asignarValorCasilla(i,j);
		}
	}
}

void Tablero::mostrarTablero() const{
	std::cout << "┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐" <<std::endl;

	for(int i = 0; i < 10; i++){
		if(i != 0) std::cout << "├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤" <<std::endl;

		std::cout << "│";

		for(int j = 0; j < 10; j++){
			if(hasBomba(i,j)) std::cout << " * │";
			else{
				switch(getCasilla(i,j)){
					case 0:
						std::cout << "   │";
					break;
					case 1:
						std::cout << BBLUE << " 1" << RESET << " │";
					break;
					case 2:
						std::cout << BGREEN << " 2" << RESET << " │";
					break;
					case 3:
						std::cout << BRED << " 3" << RESET << " │";
					break;
					case 4:
						std::cout << BPURPLE << " 4" << RESET << " │";
					break;
					case 5:
						std::cout << BWHITE << " 5" << RESET << " │";
					break;
					case 6:
						std::cout << BCYAN << " 6" << RESET << " │";
					break;
					case 7:
						std::cout << BYELLOW << " 7" << RESET << " │";
					break;
					case 8:
						std::cout << BBLACK << " 8" << RESET << " │";
					break;
				}
			}
		}

		std::cout << std::endl;
	}

	std::cout << "└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘" <<std::endl;
}
