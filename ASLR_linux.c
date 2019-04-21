// para que los segmentos de texto, datos y BSS sean también aleatorios,
// hay que compilar con la opción -fPIE -pie: gcc -fPIE -pie -o output ASLR_linux.out
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/auxv.h>
#include <unistd.h>
#define MAX_PATH 255

//se comprueba el comienzo del código (text segment)
int verifyCode(FILE *fileCSV)
{
  extern void _start();
  fprintf(stdout,"%p\n",&_start);
  fprintf(fileCSV,"%p\n",&_start);
  return 0;
}

//se comprueba una dirección del segmento de datos (data segment)
int verifyData(FILE *fileCSV)
{
  static int staticVar = 3;
  fprintf(stdout,"%p,",&staticVar);
  fprintf(fileCSV,"%p,",&staticVar);
  return 0;
}

//se comprueba una dirección del BSS, donde están las variables no inicializadas
int verifyBSS(FILE *fileCSV)
{
  static char *uninitVar;
  fprintf(stdout,"%p,",&uninitVar);
  fprintf(fileCSV,"%p,",&uninitVar);
  return 0;
}

//se comprueba el comienzo del heap
int verifyHeap(FILE *fileCSV)
{
  char *mallocVar;
  mallocVar=(char *) malloc(100);
  if(mallocVar==NULL)
  {
    fprintf(stdout,"error,");
    return 1;
  } 
  fprintf(stdout,"%p,",mallocVar);
  fprintf(fileCSV,"%p,",mallocVar);
  free(mallocVar);
  return 0;
}

//se comprueba el comienzo del brk (final del heap)
int verifyBrk(FILE *fileCSV)
{
  void *b = sbrk(0);
  int *p = (int *)b;
  fprintf(stdout,"%p,",p);
  fprintf(fileCSV,"%p,",p);
  brk(b);
  return 0;
}

//se comprueba una direción de la zona de memoria compartida
int verifyMmap(FILE *fileCSV)
{
  void *p;
  if ((p=mmap (NULL,sizeof(int),PROT_READ, MAP_SHARED |MAP_ANONYMOUS, -1, 0)) 
    == MAP_FAILED)
  {
    fprintf(stdout,"error,");   
    return 1;
  }
  fprintf(stdout,"%p,",p);
  fprintf(fileCSV,"%p,",p);
  if (munmap(p, sizeof(int)) == -1)
    return 1;
  return 0;
}

//se comprueba la dirección de la pila
int verifyStack(FILE *fileCSV)
{
  int stack=1;
  fprintf(stdout,"%p,",&stack);
  fprintf(fileCSV,"%p,",&stack);
  return 0;
}

//se comprueba la dirección de un Virtual Dynamic Shared Object
int verifyVDSO(FILE *fileCSV)
{
  unsigned long vdso = getauxval(AT_SYSINFO_EHDR);
  fprintf(stdout,"%p,",(unsigned long*) vdso);
  fprintf(fileCSV,"%p,",(unsigned long*) vdso);
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
  printf("+-------------+--------------+--------------+--------------+" 
    "--------------+--------------+--------------+-------------+\n");
  printf("     VDSO     |    Stack     |     Mmap     |     Brk      |"
    "     Heap     |     BSS      |     Data     |     Code     \n");
  printf("+-------------+--------------+--------------+--------------+"
    "--------------+--------------+--------------+-------------+\n");

  verifyVDSO(fileCSV);
  verifyStack(fileCSV);
  verifyMmap(fileCSV);
  verifyBrk(fileCSV);
  verifyHeap(fileCSV);
  verifyBSS(fileCSV);
  verifyData(fileCSV); 
  verifyCode(fileCSV);
  fclose(fileCSV);
  return 0; 
}
