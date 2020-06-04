#ifndef REGISTRO_HPP
#define REGISTRO_HPP
#include "usuario.hpp"
#include <list>

class Registro{

	private:
		list <Usuario> usuarios_;

	public:
		bool checkNombre(string nombre);
		bool checkPassword(string nombre, string password);
		void addUsuario(Usuario u);
		int deleteUsuario(string nombre);
		void escribeUsuarios();
		void leeUsuarios();
		inline list<Usuario> getUsuarios() const{return usuarios_;}
};

#endif
