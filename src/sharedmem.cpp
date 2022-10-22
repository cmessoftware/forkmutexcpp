#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <semaphore.h>

#include "include/sharedmem.h"
#include "include/Utils.h"
#include "include/Sem.h"

using namespace std;


void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
}

float get_promedio(int *buffer)
 {
        //calcular el promedio
        float suma = 0, promedio = 0;

        for(int i=0; i < BUFFER_SIZE; i++)
        {
             suma += buffer[i];   
        }

        promedio = suma/(float)BUFFER_SIZE;

        return promedio;
 }



int main() 
{
  //Creo una fifo para avisar al hijo que hay mensajes para leer.
  char * FIFO_IN  = "FIFO_IN";
  char * FIFO_OUT = "FIFO_OUT";
  Utils *utils = new Utils();
  Sem *sem = new Sem();
  //int fdIn, fdOut;

  // Creating the named file(FIFO)
  // mkfifo(<pathname>, <permission>)
  mkfifo(FIFO_IN, 0640);
  mkfifo(FIFO_OUT, 0640);
  ///fdOut = open(FIFO_OUT, O_RDONLY);
 
  int numero_compartido = 0;

  int* shmem = (int*)create_shared_memory(1);

  if(shmem == MAP_FAILED)
  {
      perror("mmap error");
  }
  else
  {
      // perror("mmap size: %d\n", shmem.size);
  }
 
  int pid = fork();
  //int pid=0;

  if (pid == 0) 
  {
  printf(" Child read: %s\n", (char*)shmem);
	int contador = 0;
	int numeros[BUFFER_SIZE];
	char buffer_archivo[BUFFER_ARCHIVO];
  char data[2];
  int fdIN, fdOUT;
    
	while(contador < BUFFER_SIZE)
	{
	  sleep(2);
      char data_ack[1] = {'a'};
      
      Semaphore* mutex = sem->MakeSemaphore(1);
      //Protejo el acceso a la memoria compartida con semaforos.
      sem->SemaphoreWait(mutex);
         fdIN = open(FIFO_IN, O_RDONLY);
         fdOUT = open(FIFO_OUT, O_WRONLY);
         read(fdIN, data, sizeof(data));
         memcpy(&numero_compartido,(int*)shmem, sizeof(shmem));
         write(fdOUT, data_ack, strlen(data_ack)+1);
      sem->SemaphoreSignal(mutex);
    	
    	printf(" %d - Hijo:leyendo: %d\n",contador, numero_compartido); //escribe el numero 
		
        numeros[contador] = numero_compartido;
        contador++;
      // contador_hijo++;
      // if(contador_hijo == BUFFER_SIZE)
      //   exit(0);
	}

   for(int i=0 ; i < BUFFER_SIZE; i++)
       numeros[i] = rand()%10+1;


	//Envio al archivo compartido.
    printf("HIJO: Abro archivo resultado.txt\n"); //escribe el numero 
	 
    FILE *fp = fopen("resultados.txt", "a");  //habre el archivo "resultados.txt" para escribir en el
    if (fp == NULL) {
        perror("Error en la apertura del archivo");  //informa de un error en la aperura del archivo "resultados.txt"
        exit(1);
    }
  	  
    int i=0;
    for( ; i < 2*BUFFER_SIZE;i++)
    {
      char* caracter = utils->Itoa(numeros[i],(char*)(&buffer_archivo[i]), 10);
      //buffer_archivo[i] = (char)numeros[i];
      char car = ',';
      i = i+1;
      buffer_archivo[i] = car;
      //strcat(buffer_archivo[i+1],(const char*)',');
    }
    char cnum[4];
    utils->Ftoa(get_promedio(numeros),cnum,2);
    int j=i;
    int c = 0;

    for( ;j < i+4;j++,c++)
    {
      if(c == 4) continue;
      buffer_archivo[j] = cnum[c];
    }

    buffer_archivo[j+1] = '\0';

    i=0;
    while( i < BUFFER_ARCHIVO)
    {
        if(buffer_archivo[i+1] == '\0') break;
        printf("%c",buffer_archivo[i]);
        fputc(buffer_archivo[i],fp);
        i++;
    }
    fputc('\n',fp);

    // printf(" Escribo en archivo resultado.txt: %s\n,",buffer_archivo); //escribe el numero 
    // size_t ret = fprintf(fp,"%s",buffer_archivo);
 	  
	
	  fflush(fp);
	  fclose(fp);
   // close(fdIn);
   // close(fdOut);

    exit(3);

  } 
  else {
    //printf("Parent read: %s\n", shmem);
	////////////////////////////////////////////////////////////
        //crea un numero aleatorio entre el 0 y el 10
      int contador = 0;
      int numeros[BUFFER_SIZE];
      int numero;
      int fdIn, fdOut;  
      char data[1] = {'e'};
      time_t t1; 

      do{
           
            srand ((unsigned)time(&t1)); 
            numero = rand()%10+1; 
            Semaphore* mutex = sem->MakeSemaphore(1);
            printf("%d - Padre:escribiendo: %d\n",contador, numero ); //escribe el numero 
            
            sem->SemaphoreWait(mutex);
              fdIn = open(FIFO_IN, O_WRONLY);
              fdOut = open(FIFO_OUT, O_RDONLY);
              memcpy(shmem, (int*)&numero, sizeof(numero));
              write(fdIn, data, strlen(data)+1);
              read(fdOut, data, sizeof(data));
            sem->SemaphoreSignal(mutex);

            numeros[contador] = numero;
            sleep(2); //espera 2 segundos
            contador++;

            // contador_padre++;
            // if(contador_padre == BUFFER_SIZE)
            //   exit(0);


        }while(contador < BUFFER_SIZE); //se repite el proceso 10 veces 
        ////////////////////////////////////////////////////////////
        //Abro archivo compartido
        printf("PADRE: Abro archivo resultado.txt:\n"); //escribe el numero 
        
        // Semaphore *mutex = make_semaphore(1);
        // semaphore_wait(mutex);
        FILE *fp = fopen("resultados.txt", "r");  //habre el archivo "resultados.txt" para escribir en el
          if (fp == NULL) {
              perror("Error en la apertura del archivo");  //informa de un error en la aperura del archivo "resultados.txt"
              exit(1);
          }
        //semaphore_signal(mutex);

        char buffer[1024];

        fread(buffer,sizeof(char),1024, fp);
        printf(" Leo de archivo resultado.txt: %s\n,",buffer); //escribe el numero 

        fflush(fp);
        fclose(fp);
        //close(fdIn);
        //close(fdOut);

        //Muestro contenido de resultados.txt en pantalla.
        for(int i=0; i < BUFFER_SIZE; i++)
            printf("%d,",buffer[i]);
     
  }

   exit(4);
}


 