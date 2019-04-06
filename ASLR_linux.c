#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/shm.h>
#define MAX_PATH 255

int verifySharedMemory(FILE *fileCSV)
{
  void *r;
  int mem_id;
  if ((mem_id = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0777))== -1)
  {
    fprintf(stdout,"error,");   
    return 1;
  }
  if ((r = shmat(mem_id,NULL,0))==(void*) -1)
  {
    fprintf(stdout,"error,");  
    return 1;
  }
  fprintf(stdout,"%8p,",r);
  fprintf(fileCSV,"%8p,",r);
  if ((shmctl( mem_id, IPC_RMID, r)) == -1) 
      return 1;
  return 0;
}

int verifyMmap(FILE *fileCSV)
{
  void *p;
  if ((p=mmap (NULL,sizeof(int),PROT_READ, MAP_SHARED |MAP_ANONYMOUS, -1, 0)) 
    == MAP_FAILED)
  {
    fprintf(stdout,"error,");   
    return 1;
  }
  fprintf(stdout,"%8p,",p);
  fprintf(fileCSV,"%8p,",p);
  if (munmap(p, sizeof(int)) == -1)
    return 1;
	return 0;
}

int verifyHeapMalloc(FILE *fileCSV)
{
	char *mallocVar;
	mallocVar=(char *) malloc(100);
	if(mallocVar==NULL)
  {
		fprintf(stdout,"error,");
		return 1;
	} 
  fprintf(stdout,"%8p,",mallocVar);
  fprintf(fileCSV,"%8p,",mallocVar);
  free(mallocVar);
	return 0;
}

int verifyStack(FILE *fileCSV)
{
	int stack=1;
	fprintf(stdout,"%8p,",&stack);
	fprintf(fileCSV,"%8p,",&stack);
	return 0;
}

int main(int argc, char *argv[])
{
  FILE *fileCSV;
  char filename[MAX_PATH];
  if (argc == 2)
  {
    strcpy(filename, argv[1]);
    printf("%s\n", filename);
    fileCSV = fopen(filename,"a");
  } 
  else 
  {
    fileCSV=fopen("ASLR.csv","a");
    strcpy(filename, "ASLR.csv");
  }
  if(fileCSV==NULL)
  {
    printf("Cannot open an input file. Exiting.\n");
    return 1;
  }
  printf("+-------+--------+--------+--------+\n");
  printf(" Stack | Heap | Mmap | Shared Mem\n");
  printf("+-------+--------+--------+--------+\n");

	verifyStack(fileCSV);
	verifyHeapMalloc(fileCSV);
	verifyMmap(fileCSV);
	verifySharedMemory(fileCSV);
	fprintf(stdout,"\n");
	fprintf(fileCSV,"\n");
  fclose(fileCSV);	
  return 0; 
}
