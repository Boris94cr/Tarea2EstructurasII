
#include <stdio.h>

#include "stdbool.h"
#include <time.h>

void criba(int m[], int tam){

    int k=0;
    //  int n[tam];
      // el 0 y 1 no son primos por regla
    m[0] = false;
    m[1] = false;
    printf("CORRO ");
    //se colocan todos en 1 para dar un valor y no tener segmentation fault
    
    for(int i = 2; i <= tam; ++i) 
        m[i] = true;
   
       
		for(int i = 2; i*i <= tam; ++i) {
			if(m[i]) {
				for(int h = 2; i*h <= tam; ++h)
					m[i*h] = false;
			}
		}
    

   
    
      for(int i = 0; i<tam; i++){
        if(m[i]){
          k=k+1;
          printf("Primo %d: %d \n",k,i);
        }
      }
    
        printf("Termino \n");
}

int main(){
	int tam = 3571;
	int m[tam];
	clock_t begin, end;
	double time_spent;
    
    begin = clock();
	
	criba(m,tam);
    
    end = clock();
    
    time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
    printf("time spent %f\n",time_spent );
  
  
}
 
