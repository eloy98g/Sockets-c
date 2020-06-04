#include <cstdlib>
#include <iostream>
#include "usuario.hpp"
using namespace std;

Usuario::Usuario(string nombre, string usuario, int socket){
	nombre_ = nombre;
	password_ = usuario;
	socket_ = socket;

	estado_ = USUARIO_VACIO;
}

void Usuario::setNombre(string nuevo){
	nombre_ = nuevo;
}

void Usuario::setPassword(string nuevo){
	password_ = nuevo;
}

void Usuario::setEstado(int nuevo){
	estado_ = nuevo;
}

void Usuario::setSocket(int nuevo){
	socket_ = nuevo;
}
