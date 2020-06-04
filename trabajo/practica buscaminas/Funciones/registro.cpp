#include "registro.hpp"
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <fstream>
using namespace std;

bool Registro::checkNombre(string nombre){
	list<Usuario>::iterator i;

	if(usuarios_.empty()){return false;}

	for(i = usuarios_.begin(); i != usuarios_.end(); i++){
		if(i->getNombre() == nombre){
			return true;
		}
	}
	return false;
}

bool Registro::checkPassword(string nombre, string password){
	list<Usuario>::iterator i;

	if(usuarios_.empty()){return false;}

	for(i = usuarios_.begin(); i != usuarios_.end(); i++){
		if((i->getNombre() == nombre)&&(i->getPassword() == password)){
			return true;
		}
	}
	return false;
}


void Registro::addUsuario(Usuario u){
	usuarios_.push_back(u);
}

int Registro::deleteUsuario(string nombre){

	list<Usuario>::iterator i;

	if(usuarios_.empty()){return -1;}

	for(i = usuarios_.begin(); i != usuarios_.end(); i++){
		if(i->getNombre() == nombre){
			usuarios_.erase(i);
			return 1;
		}
	}
	return -2;
}

void Registro::escribeUsuarios(){
	ofstream f;
	f.open ("jugadores.txt");
	list<Usuario>::iterator i;

	for(i = usuarios_.begin(); i !=usuarios_.end(); i++){
		f<<i->getNombre()<<","<<i->getPassword()<<"\n";
	}
	f.close();
}

void Registro::leeUsuarios(){
	ifstream f("jugadores.txt");
	list<Usuario>::iterator i;

	char Nombre[50], Password[50];

	usuarios_.clear();
	Usuario u("aux","aux");

	if(f.is_open()){
		while(f.getline(Nombre,256,',')){
			f.getline(Password,256,'\n');
			u.setNombre(Nombre);
			u.setPassword(Password);
			usuarios_.push_back(u);
		}
		f.close();
	}

}
