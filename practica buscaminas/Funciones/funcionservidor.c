#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

int parseo(string buffer, vector<string>&arg){
	if(buffer.find("USUARIO") == 0){
		size_t espacio = buffer.find(" ");
		arg[0] = buffer.substr(espacio+1);
		if (!arg[0].empty() && arg[0][arg[0].length()-1] == '\n') {
			arg[0].erase(arg[0].length()-1);
		}
		return 1;

	}else if(buffer.find("PASSWORD") == 0){
		size_t espacio = buffer.find(" ");
		arg[0] = buffer.substr(espacio+1);
		if (!arg[0].empty() && arg[0][arg[0].length()-1] == '\n') {
			arg[0].erase(arg[0].length()-1);
		}
		return 2;

	}else if(buffer.find("REGISTRO") == 0){
		size_t user = buffer.find("-u");
		size_t pass = buffer.find("-p");
		size_t fin = buffer.find("\n");

		if(user == std::string::npos || pass == std::string::npos) return 0;

		if(pass > user){
			arg[0] = buffer.substr(user+3, pass-user-4);
			if (!arg[0].empty() && arg[0][arg[0].length()-1] == '\n') {
				arg[0].erase(arg[0].length()-1);
			}
			arg[1] = buffer.substr(pass+3);
			if (!arg[1].empty() && arg[1][arg[1].length()-1] == '\n') {
				arg[1].erase(arg[1].length()-1);
			}
		}else{
			arg[0] = buffer.substr(user+3);
			if (!arg[0].empty() && arg[0][arg[0].length()-1] == '\n') {
				arg[0].erase(arg[0].length()-1);
			}
			arg[1] = buffer.substr(pass+3, user-pass-4);
			if (!arg[1].empty() && arg[1][arg[1].length()-1] == '\n') {
				arg[1].erase(arg[1].length()-1);
			}
		}
		return 3;

	}else if(buffer.find("INICIAR-PARTIDA") == 0){
		return 4;

	}else if(buffer.find("DESCUBRIR") == 0){
		size_t espacio = buffer.find(" ");
		size_t coma = buffer.find(",");
		size_t endl = buffer.find("\n");

		if(espacio == std::string::npos || coma == std::string::npos) return 0;

		arg[0] = buffer.substr(espacio+1, coma - espacio - 1);
		arg[1] = buffer.substr(coma+1, endl - coma - 1);
		return 5;

	}else if(buffer.find("PONER-BANDERA") == 0){
		size_t espacio = buffer.find(" ");
		size_t coma = buffer.find(",");
		size_t endl = buffer.find("\n");

		if(espacio == std::string::npos || coma == std::string::npos) return 0;

		arg[0] = buffer.substr(espacio+1, coma - espacio - 1);
		arg[1] = buffer.substr(coma+1, endl - coma - 1);
		return 6;
		
	}else if(buffer.find("SALIR") == 0){
		return 7;

	}else{
		return 0;
	}
}
