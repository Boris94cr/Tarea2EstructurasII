#ifndef CACHE_H
#define CACHE_H
#endif
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <math.h>
#include <string.h>
#include <fstream>

using namespace std;


class Cache{

	private:
		//diferentes variables necesarias hasta el momento para hacer la descripción de la cache
	 	int tamano_cache;
		int tamano_bloque;
		int asociatividad;
		double numero_de_bloques;
		int numero_de_sets;
		double byte_offset;
		int tag;
	    	int index;
	    	int **cache;
	    	int fin_index;
	    	double tamano_tag;
	    	int hits =0;
	    	int misses=0;

	public:
	    	//constructor
	    	Cache(int asociatividad, int tamano_cache, int tamano_bloque){
			this -> asociatividad = asociatividad;
			this -> tamano_cache = tamano_cache;
			this -> tamano_bloque = tamano_bloque;
	    	}

	    	//métodos de la clase
	    	void setCache();
	    	void createCache();
	    	void limpiarCache();
	    	void calcularTag_Index(string address);
	    	string Hex_String_to_Bin_String (string sHex);
	    	void simulacion();
};


//se encarga de calcular los parámetros del cache con base en los parámetros ingresados
void Cache::setCache(){
	
	numero_de_bloques = (1024*tamano_cache)/(tamano_bloque);
	numero_de_sets = (numero_de_bloques)/(asociatividad);
	byte_offset = log2(tamano_bloque);
	tamano_tag = 32 - log2(1024*tamano_cache/asociatividad);
	fin_index = 32 - tamano_tag - byte_offset;
	    
	//cout << "byte_offset: "<< byte_offset << endl;
	//cout << "numero_de_bloques: "<< numero_de_bloques << endl;
	//cout << "numero_de_sets: "<< numero_de_sets << endl;
	//cout << "index: "<< index << endl;
	//cout << "tag: "<< tag << endl;
}

//crea una matriz con el tamaño correspondiente a la cantidad de sets y la asociatividad
void Cache::createCache(){
	
	cache = new int *[numero_de_sets];
	    
	for(int i = 0; i < numero_de_sets; i++){
		cache[i] = new int[asociatividad];
	}
	
	for(int i = 0; i < numero_de_sets; i++){
		for(int j = 0; j < asociatividad; j++){
			cache[i][j] = 0;
		}
	}
}

//se encarga de poner en cero todos los bloques del cache para "limpiarla"
void Cache::limpiarCache() {
    	
    	//inicializa la matriz en ceros
   	for(int i = 0; i < numero_de_sets; i++){
        	for(int j = 0; j < asociatividad; j++){
            		cache[i][j] = 0;
        	}
    	}
}

//Cargar el index y el tag
void Cache::calcularTag_Index(string address){
	
	string direccion = Hex_String_to_Bin_String(address);//Se convierte a de Hex a binario, pero siempre string
		
	string tag_bin = direccion.substr(0,tamano_tag);
	string index_bin = direccion.substr(tamano_tag,fin_index);
		
	tag = stoi(tag_bin, NULL, 2);//Se convierte de string a int
	index = stoi(index_bin, NULL, 2);//Se convierte de string a int
}

//Se encarga de convertir un string de hexadecimales a un string de binarios
string Cache::Hex_String_to_Bin_String (string sHex){
	
	string sReturn = "";
                
	if(sHex.length()==7){
		sReturn.append ("0000");
	}

	for (int i = 0; i < sHex.length (); ++i){

		switch (sHex [i]){

			case '0': sReturn.append ("0000"); break;
			case '1': sReturn.append ("0001"); break;
			case '2': sReturn.append ("0010"); break;
			case '3': sReturn.append ("0011"); break;
			case '4': sReturn.append ("0100"); break;
			case '5': sReturn.append ("0101"); break;
			case '6': sReturn.append ("0110"); break;
			case '7': sReturn.append ("0111"); break;
			case '8': sReturn.append ("1000"); break;
			case '9': sReturn.append ("1001"); break;
			case 'a': sReturn.append ("1010"); break;
			case 'b': sReturn.append ("1011"); break;
			case 'c': sReturn.append ("1100"); break;
			case 'd': sReturn.append ("1101"); break;
			case 'e': sReturn.append ("1110"); break;
			case 'f': sReturn.append ("1111"); break;
		}
	}
		
	return sReturn;
}

//Simulacion		
void Cache::simulacion(){
	
	string address,b;
	
	ifstream memoria("aligned.trace");	//Leer el archivo en el objeto memoria
	
	//Recorrer la memoria
	while(memoria >> address >> b){
		int lugar=0;
		calcularTag_Index(address);
		
		//Recorrer el set
		for(int i=0; i<asociatividad; i++){
						
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
		}			
	}
	
	cout << "Hits: " << hits << endl;
	cout << "Misses: " << misses << endl;
	
	int miss_rate = (misses*100)/(hits+misses);

	cout << "Miss rate: " << miss_rate << "%" << endl << endl;
}
