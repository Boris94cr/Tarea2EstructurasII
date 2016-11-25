#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache.hpp"
using namespace std;

string intTostrBin (int inttobin){
	int dividendo = inttobin; 	
	int resto, divisor;
	string binario = "";

	while(dividendo >= 2){ //Mientras el dividendo sea mayor o igual 2.
		resto = dividendo % 2;
		if(resto == 1)
			binario = "1" + binario; //Si el resto es igual a 1 concatenamos 1 a la variable string llamada binario
		else
			binario = "0" + binario; // Sino concatemanos 0
		dividendo = dividendo/2; // Actualizamos el valor del dividendo dividiendolo entre 2.
	}
	if(dividendo == 1)
		binario = "1" + binario; // Por último sí el valor final del dividendo es 1 concatenamos 1 sino concatenamos 0.
	else
		binario = "0" + binario;
	
	return binario;
}

string banderas (int bandera){
	string mesi;
	if(bandera==0)
		mesi="M";
	if(bandera==1)
		mesi="E";
	if(bandera==2)
		mesi="S";
	if(bandera==3)
		mesi="I";			
	return mesi;
}

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
		cout << "El tamaño de cache L1 es: "<< tamano_cache_L1 << " kB" << endl;
		cout << "El tamaño de cache L2 es: "<< tamano_cache_L2 << " kB" << endl;
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
			if(accion=="L"){ // READ	
				if(linea%2==1){	//CPU0
					if(CacheL1_0.cache[CacheL1_0.index]==CacheL1_0.tag && CacheL1_0.flags[CacheL1_0.index]!=3){ // READ HIT y no es Invalid
						if (CacheL1_1.cache[CacheL1_1.index]==CacheL1_1.tag){ //Hit en CPU1
							if (CacheL1_1.flags[CacheL1_1.index]==0){ //Modify en CPU1
								CacheL2.cache[CacheL2.index]=CacheL2.tag;
								CacheL2.flags[CacheL2.index]=2;
								if (CacheL1_0.flags[CacheL1_0.index]==0){ //Si esta Modify en CPU0 se guarda
									string tagL1_0 = intTostrBin(CacheL1_0.cache[CacheL1_0.index]);
									while(tagL1_0.length()<19){
										tagL1_0 = "0" + tagL1_0;
									}
									string newtag = tagL1_0.substr(0,16);
									int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
									string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
									int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
									CacheL2.cache[newindexL2]=newtagL2;
									CacheL2.flags[newindexL2]=1;									
								}
								CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
								CacheL1_0.flags[CacheL1_0.index]=2;
								CacheL1_1.flags[CacheL1_1.index]=2;
							}							
							if (CacheL1_1.flags[CacheL1_1.index]==1){ //Exclusive en CPU1
								if (CacheL1_0.flags[CacheL1_0.index]==0){ //Si esta Modify en CPU0 se guarda
									string tagL1_0 = intTostrBin(CacheL1_0.cache[CacheL1_0.index]);
									while(tagL1_0.length()<19){
										tagL1_0 = "0" + tagL1_0;
									}
									string newtag = tagL1_0.substr(0,16);
									int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
									string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
									int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
									CacheL2.cache[newindexL2]=newtagL2;
									CacheL2.flags[newindexL2]=1;									
								}
								CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
								CacheL1_0.flags[CacheL1_0.index]=2;
								CacheL1_1.flags[CacheL1_1.index]=2;
								CacheL2.flags[CacheL2.index]=2;
							}
							if (CacheL1_1.flags[CacheL1_1.index]==2){ //Shared en CPU1
								CacheL1_0.flags[CacheL1_0.index]=2;								
								CacheL2.flags[CacheL2.index]=2;
								
							}
							if (CacheL1_1.flags[CacheL1_1.index]==3){ //Invalid en CPU1
								CacheL1_0.flags[CacheL1_0.index]=1;								
								CacheL2.flags[CacheL2.index]=3;
							}
						}
						else{	//Miss en CPU1
							CacheL1_0.flags[CacheL1_0.index]=1;
							CacheL1_1.flags[CacheL1_1.index]=3;
							CacheL2.flags[CacheL2.index]=3;																						
						}						
					}				
					else { //READ MISS o Invalid
						if (CacheL1_0.flags[CacheL1_0.index]==0){ //Si esta Modify en CPU0 se guarda
							string tagL1_0 = intTostrBin(CacheL1_0.cache[CacheL1_0.index]);
							while(tagL1_0.length()<19){
								tagL1_0 = "0" + tagL1_0;
							}
							string newtag = tagL1_0.substr(0,16);
							int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
							string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
							int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
							CacheL2.cache[newindexL2]=newtagL2;
							CacheL2.flags[newindexL2]=1;									
						}
						if (CacheL1_1.cache[CacheL1_1.index]==CacheL1_1.tag){ //Hit en CPU1
							if (CacheL1_1.flags[CacheL1_1.index]==0){ //Modify en CPU1
								CacheL2.cache[CacheL2.index]=CacheL2.tag;
								CacheL2.flags[CacheL2.index]=2;
								CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
								CacheL1_0.flags[CacheL1_0.index]=2;								
								CacheL1_1.flags[CacheL1_1.index]=2;
							}
							if (CacheL1_1.flags[CacheL1_1.index]==2){ //Exclusive en CPU1
								CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
								CacheL2.flags[CacheL2.index]=2;								
								CacheL1_0.flags[CacheL1_0.index]=2;								
								CacheL1_1.flags[CacheL1_1.index]=2;
							}
							if (CacheL1_1.flags[CacheL1_1.index]==2){ //Shared en CPU1
								CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
								CacheL1_0.flags[CacheL1_0.index]=2;								
								CacheL2.flags[CacheL2.index]=2;
							}
							if (CacheL1_1.flags[CacheL1_1.index]==3){ //Invalid en CPU1
								CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
								CacheL1_0.flags[CacheL1_0.index]=1;								
								CacheL2.flags[CacheL2.index]=3;
							}
							
						}
						else if (CacheL2.cache[CacheL2.index]==CacheL2.tag){ //Verifica si esta en L2
							CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
							CacheL1_0.flags[CacheL1_0.index]=2;								
							CacheL2.flags[CacheL2.index]=2;
						}
						else { //Se trae el bloque a L2
							CacheL2.cache[CacheL2.index]=CacheL2.tag;
							CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
							CacheL1_0.flags[CacheL1_0.index]=2;								
							CacheL2.flags[CacheL2.index]=2;
						}												
					}
				}								
				else{ // CPU1 
					if(CacheL1_1.cache[CacheL1_1.index]==CacheL1_1.tag && CacheL1_1.flags[CacheL1_1.index]!=3){ // READ HIT y no es Invalid
						if (CacheL1_0.cache[CacheL1_0.index]==CacheL1_0.tag){ //Hit en CPU0	
							if (CacheL1_0.flags[CacheL1_0.index]==0){ //Modify en CPU0
								CacheL2.cache[CacheL2.index]=CacheL2.tag;
								CacheL2.flags[CacheL2.index]=2;
								if (CacheL1_1.flags[CacheL1_1.index]==0){ //Si esta Modify en CPU1 se guarda
									string tagL1_0 = intTostrBin(CacheL1_1.cache[CacheL1_1.index]);
									while(tagL1_0.length()<19){
										tagL1_0 = "0" + tagL1_0;
									}
									string newtag = tagL1_0.substr(0,16);
									int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
									string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
									int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
									CacheL2.cache[newindexL2]=newtagL2;
									CacheL2.flags[newindexL2]=1;									
								}
								CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
								CacheL1_1.flags[CacheL1_1.index]=2;
								CacheL1_0.flags[CacheL1_0.index]=2;
							}							
							if (CacheL1_0.flags[CacheL1_0.index]==1){ //Exclusive en CPU0
								if (CacheL1_1.flags[CacheL1_1.index]==0){ //Si esta Modify en CPU1 se guarda
									string tagL1_0 = intTostrBin(CacheL1_1.cache[CacheL1_1.index]);
									while(tagL1_0.length()<19){
										tagL1_0 = "0" + tagL1_0;
									}
									string newtag = tagL1_0.substr(0,16);
									int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
									string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
									int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
									CacheL2.cache[newindexL2]=newtagL2;
									CacheL2.flags[newindexL2]=1;									
								}
								CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
								CacheL1_1.flags[CacheL1_1.index]=2;
								CacheL1_0.flags[CacheL1_0.index]=2;
								CacheL2.flags[CacheL2.index]=2;
							}
							if (CacheL1_0.flags[CacheL1_0.index]==2){ //Shared en CPU0
								CacheL1_1.flags[CacheL1_1.index]=2;								
								CacheL2.flags[CacheL2.index]=2;
							}
							if (CacheL1_1.flags[CacheL1_1.index]==3){ //Invalid en CPU1
								CacheL1_1.flags[CacheL1_1.index]=1;								
								CacheL2.flags[CacheL2.index]=3;
							}
						}
						else{ 
							CacheL1_0.flags[CacheL1_0.index]=3;
							CacheL1_1.flags[CacheL1_1.index]=1;								
							CacheL2.flags[CacheL2.index]=3;
						}					
					}					
					else { //READ MISS o Invalid
						if (CacheL1_1.flags[CacheL1_1.index]==0){ //Si esta Modify en CPU1 se guarda
							string tagL1_0 = intTostrBin(CacheL1_1.cache[CacheL1_1.index]);
							while(tagL1_0.length()<19){
								tagL1_0 = "0" + tagL1_0;
							}
							string newtag = tagL1_0.substr(0,16);
							int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
							string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
							int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
							CacheL2.cache[newindexL2]=newtagL2;
							CacheL2.flags[newindexL2]=1;									
						}
						if (CacheL1_0.cache[CacheL1_0.index]==CacheL1_0.tag){ //Hit en CPU0
							if (CacheL1_0.flags[CacheL1_0.index]==0){ //Modify en CPU0
								CacheL2.cache[CacheL2.index]=CacheL2.tag;
								CacheL2.flags[CacheL2.index]=2;
								CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
								CacheL1_1.flags[CacheL1_1.index]=2;								
								CacheL1_0.flags[CacheL1_0.index]=2;
							}
							if (CacheL1_0.flags[CacheL1_0.index]==2){ //Exclusive en CPU0
								CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
								CacheL2.flags[CacheL2.index]=2;								
								CacheL1_0.flags[CacheL1_0.index]=2;								
								CacheL1_1.flags[CacheL1_1.index]=2;
							}
							if (CacheL1_0.flags[CacheL1_0.index]==2){ //Shared en CPU0
								CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
								CacheL1_1.flags[CacheL1_1.index]=2;								
								CacheL2.flags[CacheL2.index]=2;
							}
							if (CacheL1_0.flags[CacheL1_0.index]==3){ //Invalid en CPU0
								CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
								CacheL1_1.flags[CacheL1_1.index]=1;								
								CacheL2.flags[CacheL2.index]=3;
							}
							
						}
						else if (CacheL2.cache[CacheL2.index]==CacheL2.tag){ //Verifica si esta en L2
							CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
							CacheL1_1.flags[CacheL1_1.index]=2;								
							CacheL2.flags[CacheL2.index]=2;
						}
						else { //Se trae el bloque a L2
							CacheL2.cache[CacheL2.index]=CacheL2.tag;
							CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
							CacheL1_1.flags[CacheL1_1.index]=2;								
							CacheL2.flags[CacheL2.index]=2;
						}							
					}
				}	
			}
			
			//************* Escritura ***************
			else{ //WRITE
				if(linea%2==1){ // CPU0					
					if(CacheL1_0.cache[CacheL1_0.index]==CacheL1_0.tag && CacheL1_0.flags[CacheL1_0.index]!=3){ // READ HIT y no es Invalid
						//Nada
					}					
					else{ //READ Miss o Invalid
						if (CacheL1_0.flags[CacheL1_0.index]==0){ //Si esta Modify en CPU0 se guarda
							string tagL1_0 = intTostrBin(CacheL1_0.cache[CacheL1_0.index]);
							while(tagL1_0.length()<19){
								tagL1_0 = "0" + tagL1_0;
							}
							string newtag = tagL1_0.substr(0,16);
							int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
							string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
							int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
							CacheL2.cache[newindexL2]=newtagL2;
							CacheL2.flags[newindexL2]=1;									
						}
						if (CacheL2.cache[CacheL2.index]!=CacheL2.tag){ //Verifica si esta en L2
							CacheL2.cache[CacheL2.index]=CacheL2.tag;
						}
						CacheL1_0.cache[CacheL1_0.index]=CacheL1_0.tag;
					}
					if (CacheL1_1.cache[CacheL1_1.index]==CacheL1_1.tag){ //Si está en otro cachese invalida	
						CacheL1_1.flags[CacheL1_1.index]=3;
					}
					CacheL1_0.flags[CacheL1_0.index]=0;								
					CacheL2.flags[CacheL2.index]=1;
				}
				else{ //CPU1
					if(CacheL1_1.cache[CacheL1_1.index]==CacheL1_1.tag && CacheL1_1.flags[CacheL1_1.index]!=3){ // READ HIT y no es Invalid
						//Nada
					}					
					else{ //READ Miss o Invalid
						if (CacheL1_1.flags[CacheL1_1.index]==0){ //Si esta Modify en CPU1 se guarda
							string tagL1_0 = intTostrBin(CacheL1_1.cache[CacheL1_1.index]);
							while(tagL1_0.length()<19){
								tagL1_0 = "0" + tagL1_0;
							}
							string newtag = tagL1_0.substr(0,16);
							int newtagL2 = stoi(newtag, NULL, 2); //Nuevo tag de L2
							
							string newindex = tagL1_0.substr(16,19) + intTostrBin(CacheL1_0.index);
							int newindexL2 = stoi(newindex, NULL, 2); // Nuevo index de L2
							CacheL2.cache[newindexL2]=newtagL2;
							CacheL2.flags[newindexL2]=1;									
						}
						if (CacheL2.cache[CacheL2.index]!=CacheL2.tag){ //Verifica si esta en L2
							CacheL2.cache[CacheL2.index]=CacheL2.tag;
						}
						CacheL1_1.cache[CacheL1_1.index]=CacheL1_1.tag;
					}
					if (CacheL1_0.cache[CacheL1_0.index]==CacheL1_0.tag){ //Si está en otro cachese invalida	
						CacheL1_0.flags[CacheL1_0.index]=3;
					}
					CacheL1_1.flags[CacheL1_1.index]=0;								
					CacheL2.flags[CacheL2.index]=1;
				}
			}
		
		//!!!!!!!!! imprimir las ultimas 10 lineas !!!!!!!!!!!!!!!!!
			if (linea>49642108){
				
				cout << linea << endl << "Lectura/Escritura: " << accion << "\tCache L2: " << CacheL2.cache[CacheL2.index] << "\t\tCache L1_0: " << CacheL1_0.cache[CacheL1_0.index] << "\tCache L1_1: " << CacheL1_1.cache[CacheL1_1.index] << "\tFlag L2: " << banderas(CacheL2.flags[CacheL2.index]) << "\tFlag L1_0: " << banderas(CacheL1_0.flags[CacheL1_0.index]) << "\tFlag L1_1: " << banderas(CacheL1_1.flags[CacheL1_1.index]) << endl;
			}	
		}			
	}	  	
    	return 0;
}
