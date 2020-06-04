#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

int parseo(string buffer, vector<string>&arg){
	if(buffer.find("USUARIO") == 0){
		arg[0] = strstr(buffer.c_str(), "USUARIO ");
		return 1;

	}else if(buffer.find("PASSWORD") == 0){
		arg[0] = strstr(buffer.c_str(), "PASSWORD ");
		return 2;

	}else if(buffer.find("REGISTRO") == 0){
		size_t user = buffer.find("-u");
		size_t pass = buffer.find("-p");

		if(pass > user){
			arg[0] = buffer.substr(user+3, pass-user-3);
			arg[1] = buffer.substr(pass+3);
		}else{
			arg[0] = buffer.substr(user+3);
			arg[1] = buffer.substr(pass+3, user-pass-3);
		}
		return 3;

	}else if(buffer.find("INICIAR-PARTIDA") == 0){
		return 4;

	}else if(buffer.find("DESCUBRIR") == 0){
		size_t coma = buffer.find(",");
		arg[0] = buffer.substr(coma-1, 1);
		arg[1] = buffer.substr(coma+1, 1);
		return 5;

	}else if(buffer.find("PONER-BANDERA") == 0){
		size_t coma = buffer.find(",");
		arg[0] = buffer.substr(coma-1, 1);
		arg[1] = buffer.substr(coma+1, 1);
		return 6;
		
	}else if(buffer.find("SALIR") == 0){
		return 7;

	}else{
		return 0;
	}
}
