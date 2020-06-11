#include <stdio.h>
#include <string.h>

using namespace std;

int parseo(string buffer, vector<string>&arg){
	if(buffer.find("ATACAR") == 0){
		return 1;

	}else if(buffer.find("RENDIRSE") == 0){
		return 2;

	}else if(buffer.find("SALIR") == 0){
		return 3;

	}else{
		return 0;
	}
}
