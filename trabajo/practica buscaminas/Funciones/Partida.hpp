#include <vector>
#include <string>
#include <cassert>

#include "Tablero.hpp"
#include "FuncionesAuxiliares.hpp"

#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#define ESTADO_DESCONOCIDA -2
#define ESTADO_DESCUBIERTA -1

class Partida{
	private:
		std::vector < std::vector <int> > estadosCasillas_;
		Tablero tablero_;
		std::vector <int> banderasJugadores_;
		bool partidaTerminada_;
		int ganador_;

		void inline setEstadoCasilla(int i, int j, int estado){estadosCasillas_[i][j] = estado;}

		void inline setNBanderas(int i, int n){
			#ifndef NDEBUG
				assert(i >= 0);
				assert(i < 2);
				assert(n >= 0);
				assert(n < 10);
			#endif

			banderasJugadores_[i] = n;
		}
		int inline getNBanderas(int i) const{
			#ifndef NDEBUG
				assert(i >= 0);
				assert(i < 2);
			#endif

			return banderasJugadores_[i];
		}

		void gana(int jugador);

		int getGanador() const;

		void pierde(int jugador);

		bool inline estaDescubierta(int i, int j) const{
			#ifndef NDEBUG
				assert(checkCoords(i,j));
			#endif

			return estadosCasillas_[i][j] == ESTADO_DESCUBIERTA;
		}

		bool inline esDesconocida(int i, int j) const{
			#ifndef NDEBUG
				assert(checkCoords(i,j));
			#endif

			return estadosCasillas_[i][j] == ESTADO_DESCONOCIDA;
		}

		bool inline tieneBandera(int i, int j) const{
			#ifndef NDEBUG
				assert(checkCoords(i,j));
			#endif

			return (estadosCasillas_[i][j] != ESTADO_DESCONOCIDA && estadosCasillas_[i][j] != ESTADO_DESCUBIERTA);
		}

		int inline getBandera(int i, int j) const{
			#ifndef NDEBUG
				assert(checkCoords(i,j));
				assert(tieneBandera(i,j));
			#endif

			return estadosCasillas_[i][j];
		}

		void explorar(int i, int j);

		bool checkBanderas(int jugador) const;

		void inline finPartida(){ partidaTerminada_ = true;}
	public:
		Partida();

		void restart();
		void mostrarPartida() const;

		void descubrirCasilla(int i, int j, int jugador);
		void setBandera(int i, int j, int jugador);

		bool inline partidaTerminada() const{return partidaTerminada_;}

		std::string getTablero() const;

		bool inline puedeDescubrirCasilla(int i, int j) const{
			return esDesconocida(i,j);
		}

		bool inline puedePonerBandera(int i, int j, int jugador) const{
			if(tieneBandera(i,j) == false) return true;
			if(getBandera(i,j) == 2) return false;
			if(getBandera(i,j) != jugador) return true;
			return false;
		}
};

#endif
