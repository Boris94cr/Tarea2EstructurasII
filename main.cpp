#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache.hpp"
using namespace std;

int main(int argc, char const *argv[]) {
    
	srand(time(NULL));

	//guarda los parametros enviados en variables tipo enteroo
    	//int asociatividad = atoi(argv[1]);
    	//int tamano_cache = atoi(argv[2]);
    	//int tamano_bloque = atoi(argv[3]);
	
	int asociatividad = 1;
	int tamano_cache_L1 = 8;
	int tamano_cache_L2 = 64;
	int tamano_bloque = 16;
		
	if (asociatividad != 1 && asociatividad != 2 && asociatividad != 4){
		cout << "La asociatividad seleccionada no es correcta, debe ser un valor de: 1, 2 o 4" << endl;
	}
	
	else {
	    	//para estar seguro de que se tomaron bien los parametros se imprimen
	    	cout << "La asociatividad es: "<< asociatividad << "-way associative" << endl;
		cout << "El tamaño de cache es: "<< tamano_cache << " kB" << endl;
		cout << "El tamaño de bloque es: "<< tamano_bloque << " bytes" << endl << endl;

	    	//se crean los caches
	    	Cache CacheL1_0(asociatividad, tamano_cache_L1, tamano_bloque);
		Cache CacheL1_1(asociatividad, tamano_cache_L1, tamano_bloque);
		Cache CacheL2(asociatividad, tamano_cache_L2, tamano_bloque);


	    	//se llama a los diferentes metodos necesarios
	    	CacheL1_0.setCache();
	    	CacheL1_0.createCache();
	    	CacheL1_0.limpiarCache();
	    	CacheL1_1.setCache();
	    	CacheL1_1.createCache();
	    	CacheL1_1.limpiarCache();
	    	CacheL2.setCache();
	    	CacheL2.createCache();
	    	CacheL2.limpiarCache();
		//miCache.simulacion();
		
		//************************************************************
		// ******************* Simulacion ****************************
		//************************************************************
		
		string address,accion;
		int linea = 0;
	
		ifstream memoria("aligned.trace");	//Leer el archivo en el objeto memoria
	
		//Recorrer la memoria
		while(memoria >> address >> accion){
			linea+=1;
		
			//int lugar=0;
			CacheL1_0.calcularTag_Index(address);
			CacheL1_1.calcularTag_Index(address);
			CacheL2.calcularTag_Index(address);
		
			//************* Lectura ***************
			if(accion=="L"){	
				
				if(linea%2==1){	//!!!!!! CPU0 !!!!!!!!
					
					if(CacheL1_0.cache[CacheL1_0.index]==CacheL1_0.tag){	// Hit en CPU0
						if (CacheL1_0.flags[CacheL1_0.index]==3){ //Hit pero Invalid	
							
							if (CacheL1_1.cache[CacheL1_1.index]==CacheL1_1.tag){ //Hit en CPU1
								if (CacheL1_1.flags[CacheL1_1.index]==3){ //Invalid en CPU1
									CacheL2.cache[CacheL2.index]=CacheL2.tag;
									CacheL2.flags[CacheL2.index]=2;
									CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
									CacheL1_0.flags[CacheL1_0.index]=2;
								}
								else {
									CacheL2.cache[CacheL2.index]=CacheL2.tag;
									CacheL2.flags[CacheL2.index]=2;
									CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
									CacheL1_0.flags[CacheL1_0.index]=2;
									CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
									CacheL1_1.flags[CacheL1_1.index]=2;
								}
							}
						}
					}
				
					else { //miss en CPU0
						
					}
				}
				
				//!!!!!! CPU1 !!!!!!!!
				
			}
			
			//************* Escritura ***************
			
			//Recorrer el set
			/*for(int i=0; i<asociatividad; i++){
						
				if(cache[index][i]==tag){
					hits = hits + 1;
					i=asociatividad; //cuando hay hit se sale del for
					break;			
				}
			
				else if(i==asociatividad-1){//Si hay miss
					misses = misses + 1;
					lugar = rand() % asociatividad; //Se elige un lugar aleatorio para guardar
					cache[index][lugar] = tag; //Se guarda el tag en el cache
				}			
			}*/			
		}
	
		cout << "Hits: " << hits << endl;
		cout << "Misses: " << misses << endl;
	
		int miss_rate = (misses*100)/(hits+misses);

		cout << "Miss rate: " << miss_rate << "%" << endl << endl;
	}
	
    	return 0;
}
