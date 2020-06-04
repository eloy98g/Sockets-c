#include <vector>
#include <iostream>
#include <string>
#include <cassert>

#include "macros.hpp"
#include "Partida.hpp"
#include "Tablero.hpp"
#include "FuncionesAuxiliares.hpp"

void Partida::pierde(int jugador){
	#ifndef NDEBUG
		assert(jugador >= 0);
		assert(jugador < 2);
	#endif

	switch(jugador){
		case 0:
			gana(1);
		break;
		case 1:
			gana(0);
		break;
	}
}

void Partida::gana(int jugador){
	#ifndef NDEBUG
		assert(jugador >= 0);
		assert(jugador < 2);
		assert(!partidaTerminada());
	#endif

	finPartida();
	ganador_ = jugador;
}

int Partida::getGanador() const{
	#ifndef NDEBUG
		assert(partidaTerminada());
	#endif

	if(ganador_ == 0) return getJ1();

	return getJ2();
}


void Partida::explorar(int i, int j){
	setEstadoCasilla(i, j, ESTADO_DESCUBIERTA);

	if(tablero_.getCasilla(i,j) == 0){
		if(checkCoords(i - 1, j - 1)){
			if(esDesconocida(i - 1, j - 1)){
				explorar(i - 1, j - 1);
			}
		}
		if(checkCoords(i - 1, j)){
			if(esDesconocida(i - 1, j)){
				explorar(i - 1, j);
			}
		}
		if(checkCoords(i - 1, j + 1)){
			if(esDesconocida(i - 1, j + 1)){
				explorar(i - 1, j + 1);
			}
		}
		if(checkCoords(i, j - 1)){
			if(esDesconocida(i, j - 1)){
				explorar(i, j - 1);
			}
		}
		if(checkCoords(i, j + 1)){
			if(esDesconocida(i, j + 1)){
				explorar(i, j + 1);
			}
		}
		if(checkCoords(i + 1, j - 1)){
			if(esDesconocida(i + 1, j - 1)){
				explorar(i + 1, j - 1);
			}
		}
		if(checkCoords(i + 1, j)){
			if(esDesconocida(i + 1, j)){
				explorar(i + 1, j);
			}
		}
		if(checkCoords(i + 1, j + 1)){
			if(esDesconocida(i + 1, j + 1)){
				explorar(i + 1, j + 1);
			}
		}
	}
}

bool Partida::checkBanderas(int jugador) const{
	for(int i; i < 10; i++){
		for(int j; j < 10; j++){
			if(getBandera(i,j) == jugador){
				if(tablero_.hasBomba(i,j) == false){
					return false;
				}
			}
		}
	}

	return true;
}

Partida::Partida():estadosCasillas_(10),banderasJugadores_(2){

	for(int i = 0; i < 10; i++){
		estadosCasillas_[i].resize(10);
	}

	partidaTerminada_ = true;
}

void Partida::restart(){
	tablero_.iniciarTablero();

	partidaTerminada_ = false;

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			setEstadoCasilla(i, j, ESTADO_DESCONOCIDA);
		}
	}

	for(int i = 0; i < 2; i++) setNBanderas(i, 0);

	ganador_ = -1;
	setTurno(0);
}

void Partida::mostrarPartida() const{
	std::cout << "┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐" <<std::endl;
	std::cout << "│   │ A │ B │ C │ D │ E │ F │ G │ H │ I │ J │" <<std::endl;

	for(int i = 0; i < 10; i++){
		std::cout << "├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤" <<std::endl;

		std::cout << "│ "<<i<<" │";

		for(int j = 0; j < 10; j++){
			if(esDesconocida(i,j)) std::cout << "   │";
			else if(tieneBandera(i,j)){
				switch(getBandera(i,j)){
					case 0:
						std::cout << BIRED << " þ " << RESET << "│";
					break;
					case 1:
						std::cout << BIBLUE << " þ " << RESET << "│";
					break;
					case 2:
						std::cout << BIBLUE << "þ " << BIRED << "þ" << RESET << "│";
					break;
				}
			}
			else{
				if(tablero_.hasBomba(i,j)) std::cout << " * │";
				else{
					switch(tablero_.getCasilla(i,j)){
						case 0:
							std::cout << " 0 │";
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
		}

		std::cout << std::endl;
	}

	std::cout << "└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘" <<std::endl;
}

void Partida::descubrirCasilla(int i, int j){
	#ifndef NDEBUG
		assert(!partidaTerminada());
		assert(checkCoords(i, j));
		assert(esDesconocida(i, j));
	#endif

	if(tablero_.hasBomba(i,j)){
		pierde(getTurno());

		for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++){
				if(tablero_.hasBomba(i,j)) setEstadoCasilla(i, j, ESTADO_DESCUBIERTA);
			}
		}
	}
	else{
		explorar(i,j);
	}

	setTurno(1 - getTurno());
}

void Partida::setBandera(int i, int j){
	#ifndef NDEBUG
		assert(!partidaTerminada());
		assert(checkCoords(i, j));
		assert(puedePonerBandera(i, j));
	#endif

	if(!tieneBandera(i,j)){
		setEstadoCasilla(i, j, getTurno());
		setNBanderas(getTurno(), getNBanderas(getTurno()) + 1);
	}
	else{
		setEstadoCasilla(i, j, 2);
		setNBanderas(getTurno(), getNBanderas(getTurno()) + 1);
	}

	if(getNBanderas(getTurno()) == 10){
		if(checkBanderas(getTurno())) gana(getTurno());
		else pierde(getTurno());
	}

	setTurno(1 - getTurno());
}

std::string Partida::getTablero() const{
	std::string tablero = "";

	for(int i = 0; i < 10; i++){

		for(int j = 0; j < 10; j++){
			if(esDesconocida(i,j)) tablero = tablero + "-";
			else if(tieneBandera(i,j)){
				switch(getBandera(i,j)){
					case 0:
						tablero = tablero + "a";
					break;
					case 1:
						tablero = tablero + "b";
					break;
					case 2:
						tablero = tablero + "ab";
					break;
				}
			}
			else{
				if(tablero_.hasBomba(i,j)) tablero = tablero + "*";
				else{
					switch(tablero_.getCasilla(i,j)){
						case 0:
							tablero = tablero + "0";
						break;
						case 1:
							tablero = tablero + "1";
						break;
						case 2:
							tablero = tablero + "2";
						break;
						case 3:
							tablero = tablero + "3";
						break;
						case 4:
							tablero = tablero + "4";
						break;
						case 5:
							tablero = tablero + "5";
						break;
						case 6:
							tablero = tablero + "6";
						break;
						case 7:
							tablero = tablero + "7";
						break;
						case 8:
							tablero = tablero + "8";
						break;
					}
				}
			}

			if(j != 9) tablero = tablero + ",";
			else tablero = tablero + ";";
		}
	}

	return tablero;
}
