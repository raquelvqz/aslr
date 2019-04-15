#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#define MAX_PATH 255

//se comprueba el comienzo del código (text segment)
int verifyCode(FILE *fileCSV)
{
  fprintf(stdout,"0x%x\n",&verifyCode);
  fprintf(fileCSV,"0x%x\n",&verifyCode);
  return 0;
}

//se comprueba una dirección del segmento de datos (data segment)
int verifyData(FILE *fileCSV)
{
  static int staticVar = 3;
  fprintf(stdout,"0x%x,",&staticVar);
  fprintf(fileCSV,"0x%x,",&staticVar);
  return 0;
}

//se comprueba una dirección del BSS, donde están las variables no inicializadas
int verifyBSS(FILE *fileCSV)
{
  static char *uninitVar;
  fprintf(stdout,"0x%x,",&uninitVar);
  fprintf(fileCSV,"0x%x,",&uninitVar);
  return 0;
}

// se comprueba la localización de un nuevo heap
int verifyHeapCreate(FILE *fileCSV)
{
  void *hHeap;
  void *p;
  hHeap=(void *)(size_t)HeapCreate(NULL,1024,2048);
  if(hHeap==NULL){
    fprintf(stdout,"error,");
    return 1;
  } else {
    p=(void *)(size_t) HeapAlloc(hHeap,0,100);
    if(p==NULL){
      fprintf(stdout,"error,");
      return 1;
    } else {
      fprintf(stdout,"0x%x,",p);
      fprintf(fileCSV,"0x%x,",p);
      HeapFree(hHeap,0,p);
      return 0;
    }
    HeapDestroy(hHeap);
  }
  return 0;
}

//se comprueba una dirección del heap
int verifyHeapAlloc(FILE *fileCSV)
{
  void *p;
  p = (void *)(size_t) HeapAlloc(GetProcessHeap(),0,100);
  if(p==NULL){
    fprintf(stdout,"error,");
    return 1;
  } else {
    fprintf(stdout,"0x%x,",p);
    fprintf(fileCSV,"0x%x,",p);
    HeapFree(GetProcessHeap(),0,p);
    return 0;
  }
  return 0;
}

//se comprueba el comienzo del heap
int verifyMalloc(FILE *fileCSV)
{
  char *mallocVar;
  mallocVar=(char *) malloc(100);
  if(mallocVar==NULL)
  {
    fprintf(stdout,"error,");
    return 1;
  } 
  fprintf(stdout,"0x%x,",mallocVar);
  fprintf(fileCSV,"0x%x,",mallocVar);
  free(mallocVar);
  return 0;
}

//se comprueba la dirección de la pila
int verifyStack(FILE *fileCSV)
{
  int stack=1;
  fprintf(stdout,"0x%x,",&stack);
  fprintf(fileCSV,"0x%x,",&stack);
  return 0;
}

__declspec(dllexport) //equivale a la opción /DYNAMICBASE
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
  printf("    Stack     |    Heap 1   |    Heap 2   |    Heap 3   |"
    "     BSS      |     Data     |     Code     \n");
  printf("+-------------+--------------+--------------+--------------+"
    "--------------+--------------+--------------+-------------+\n");

  verifyStack(fileCSV);
  verifyHeapCreate(fileCSV);
  verifyHeapAlloc(fileCSV);
  verifyMalloc(fileCSV);
  verifyBSS(fileCSV);
  verifyData(fileCSV); 
  verifyCode(fileCSV);
  fclose(fileCSV);
  return 0; 
}
