#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <cassert>

#define ESPERANDO_PARTIDA -1
#define USUARIO_SIN_IDENTIFICAR -2
#define USUARIO_VACIO -3

using namespace std;

class Usuario{

	private:
		string nombre_;
		string password_;
		int estado_;
		int socket_;

	public:
		Usuario(string nombre="", string password="", int socket=0);
		inline string getNombre()const{return nombre_;}
		inline string getPassword()const{return password_;}
		inline int getSocket()const{return socket_;}
		void setNombre(string nuevo);
		void setPassword(string nuevo);
		void setEstado(int nuevo);
		void setSocket(int nuevo);

		inline bool estaEnPartida()const{return estado_ >= 0;};
		inline bool estaEsperandoPartida()const{return estado_ == ESPERANDO_PARTIDA;};
		inline bool estaSinIdentificar()const{return estado_ == USUARIO_SIN_IDENTIFICAR;};
		inline bool estaVacio()const{return estado_ == USUARIO_VACIO;};
		inline int getPartida() const{
			assert(estaEnPartida());
			return estado_;
		}
};

#endif
