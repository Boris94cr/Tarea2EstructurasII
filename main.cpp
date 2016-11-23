#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache.hpp"
using namespace std;

int main(int argc, char const *argv[]) {
    
	srand(time(NULL));

	//guarda los parametros enviados en variables tipo enteroo
    	int asociatividad = atoi(argv[1]);
    	int tamano_cache = atoi(argv[2]);
    	int tamano_bloque = atoi(argv[3]);
	
	if (asociatividad != 1 && asociatividad != 2 && asociatividad != 4){
		cout << "La asociatividad seleccionada no es correcta, debe ser un valor de: 1, 2 o 4" << endl;
	}
	
	else {
	    	//para estar seguro de que se tomaron bien los parametros se imprimen
	    	cout << "La asociatividad es: "<< asociatividad << "-way associative" << endl;
		cout << "El tamaño de cache es: "<< tamano_cache << " kB" << endl;
		cout << "El tamaño de bloque es: "<< tamano_bloque << " bytes" << endl << endl;

	    	//se crea el objeto
	    	Cache miCache(asociatividad, tamano_cache, tamano_bloque);

	    	//se llama a los diferentes metodos necesarios
	    	miCache.setCache();
	    	miCache.createCache();
	    	miCache.limpiarCache();
		miCache.simulacion();
	}
	
    	return 0;
}
