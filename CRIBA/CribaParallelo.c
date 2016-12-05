
#include <stdio.h>
#include "mpi.h"
#include "stdbool.h"
#include <time.h>

void criba(int m[], int tam, int id ){

    int k=0;
      int n[tam];
      // el 0 y 1 no son primos por regla
    m[0] = false;
    m[1] = false;
    printf("CORRO ");
    //se colocan todos en 1 para dar un valor y no tener segmentation fault
    
    for(int i = 2; i <= tam; ++i) 
        m[i] = true;
    if (id == 0){
       
		for(int i = 2; i*i <= 2500; ++i) {
			if(m[i]==1) {
				for(int h = 2; i*h <= tam; ++h)
					m[i*h] = false;
			}
		}
    }
    if(id== 1){
      for(int i = 2500 ; i*i <= tam; ++i) {
        if(m[i]==1) {
            for(int h = 2; i*h <= tam; ++h)
                m[i*h] = false;
        }
      }
    }
    MPI_Reduce(m, n, tam, MPI_INT, MPI_MIN,0,MPI_COMM_WORLD);
  
    if (id==0){
      for(int i = 0; i<tam; i++){
        if(n[i]){
          k=k+1;
          printf("Primo %d: %d \n",k,i);
        }
      }
    }
        printf("Termino \n");
}

int main(){
	
	clock_t begin, end;
  double time_spent;
  int inicio,fin;
int my_id, root_process, ierr, num_procs;
      MPI_Status status;
		int tam = 3541;
		int m[tam];
      /* Create child processes, each of which has its own variables.
       * From this point on, every process executes a separate copy
       * of this program.  Each process has a different process ID,
       * ranging from 0 to num_procs minus 1, and COPIES of all
       * variables defined in the program. No variables are shared.
       */

       MPI_Init(NULL, NULL);
     
      /* find out MY process ID, and how many processes were started.*/ 
     
       MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
     MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
     if(my_id==0){
    begin = clock();
	}
	//Ejecuta la 
	criba(m,tam,my_id);
	 MPI_Finalize();

if(my_id==0){
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time spent %f\n",time_spent );
  }
  
}
 
