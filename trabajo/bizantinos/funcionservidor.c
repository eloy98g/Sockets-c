#include <stdio.h>
#include <string.h>

using namespace std;

int parseo(string buffer, vector<string>&arg){
	if(buffer.find("VOTAR") == 0){
		/*size_t espacio = buffer.find(" ");
		size_t coma = buffer.find(",");
		size_t endl = buffer.find("\n");

		if(espacio == std::string::npos || coma == std::string::npos) return 0;

		arg[0] = buffer.substr(espacio+1, coma - espacio - 1);
		arg[1] = buffer.substr(coma+1, endl - coma - 1);*/
		return 1;

	}else if(buffer.find("ORDEN") == 0){
		return 2;

	}else if(buffer.find("COMANDANTE") == 0){
		return 3;

	}else if(buffer.find("SALIR") == 0){
		return 4;

	}else{
		return 0;
	}
}

/*
CAMBIAR LOS NUMEROS DEL PARSEO EN EL SWICH
EL COMANDANTE VOTA CON ORDEN ATACAR (O RENDIR)
IF COMANDANTE HA VOTADO SE PERMITE A LOS GENERALES VOTAR
LES APARECE EL VOTO DEL COMANDANTE
LOS GENERALES VOTAN CON VOTAR 0 1 (O VOTAR ATACAR RENDIR)
*/
