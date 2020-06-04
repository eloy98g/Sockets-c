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
		int id_j1_, id_j2_;
		int turno_;

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

		int inline getTurno() const{return turno_;};
		void inline setTurno(int turno){turno_ = turno;};

		void explorar(int i, int j);

		bool checkBanderas(int jugador) const;

		void inline finPartida(){ partidaTerminada_ = true;}
	public:
		Partida();

		void restart();
		void mostrarPartida() const;

		void descubrirCasilla(int i, int j);
		void setBandera(int i, int j);

		void inline setJ1(int id){id_j1_ = id;};
		void inline setJ2(int id){id_j2_ = id;};

		int inline getJ1() const{return id_j1_;};
		int inline getJ2() const{return id_j2_;};

		int inline getOponente(int id) const{
			if(id == getJ1()) return getJ2();
			else return getJ1();};

		bool inline partidaTerminada() const{return partidaTerminada_;}
		void inline terminarPartida() {partidaTerminada_ = true;}

		std::string getTablero() const;

		bool inline puedeDescubrirCasilla(int i, int j) const{
			return esDesconocida(i,j);
		}

		bool inline puedePonerBandera(int i, int j) const{
			if(!esDesconocida(i,j)) return false;
			if(tieneBandera(i,j) == false) return true;
			if(getBandera(i,j) == 2) return false;
			if(getBandera(i,j) != getTurno()) return true;
			return false;
		}

		int inline getIDJugadorActual() const{
			if(getTurno() == 0) return getJ1();
			if(getTurno() == 1) return getJ2();
		}

		int getGanador() const;

		void inline mostrarPartidaCompleta() const{tablero_.mostrarTablero();};
};

#endif
