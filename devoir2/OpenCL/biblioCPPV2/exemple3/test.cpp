
//Main program (Listing 4)

// Multiply two matrices A * B = C
 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <CL/cl.h>
#include <iostream>
#include "../contexte.h"
#include "../program.h"

 #define MAX_SOURCE_SIZE (0x100000)
#define WA 10
#define HA 10
#define WB 10
#define HB 10
#define WC 10
#define HC 10
 

////////////////////////////////////////////////////////////////////////////////
// Fonctions utilitaires
////////////////////////////////////////////////////////////////////////////////
size_t shrRoundUp(size_t localWorkSize, size_t numItems) {
    size_t result = localWorkSize;
    while (result < numItems)
        result += localWorkSize;
    
    return result;
}
//--------------------------------
double executionTime(cl_event &event)
{
    cl_ulong start, end;
    
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
   
    return (double)1.0e-9 * (end - start); // convert nanoseconds to seconds on return
}

//////////////////////////////////////////////////
// Initiatlisation des matrices
void randomInit(float* data, int size)
{
   for (int i = 0; i < size; ++i)
//   data[i] = rand() / (float)RAND_MAX;
   data[i] = 3;
}
 
/////////////////////////////////////////////////////////
// Programme principal
/////////////////////////////////////////////////////////
using namespace std;
 
int main()
{
 
// set seed for rand()
   srand(2012);
 
   // 1. allocation des matrices 
   int size_A = WA * HA;
   unsigned int mem_size_A = sizeof(float) * size_A;
   float* h_A = (float*) malloc(mem_size_A);
 
   int size_B = WB * HB;
   unsigned int mem_size_B = sizeof(float) * size_B;
   float* h_B = (float*) malloc(mem_size_B);
 
   // 2. Initialisation
   randomInit(h_A, size_A);
   randomInit(h_B, size_B);
 
   // 3. Impression de matrices
   printf("\n\nMatrix A\n");
   for(int i = 0; i < size_A; i++)
   {
      printf("%f ", h_A[i]);
      if(((i + 1) % WA) == 0)
      printf("\n");
   }
   printf("\n\nMatrix B\n");
   for(int i = 0; i < size_B; i++)
   {
      printf("%f ", h_B[i]);
      if(((i + 1) % WB) == 0)
      printf("\n");
   }
 
   // 4. Allocation pour la matrice résultante
   int size_C = WC * HC;
   unsigned int mem_size_C = sizeof(float) * size_C;
   float* h_C = (float*) malloc(mem_size_C);
 
   // 5. Initialisation de OpenCL
   //    Le contexte
   Contexte mon_contexte;
   //    le programme
   Program mon_prog(mon_contexte, "./kernel.cl", "matrixMul");
   //    les parametres
   mon_prog.initPara(h_C, size_C, SORTIE);
   mon_prog.initPara(h_A, size_A, ENTREE);
   mon_prog.initPara(h_B, size_B, ENTREE);
   mon_prog.initPara(WA);
   mon_prog.initPara(WC);
   // On execute le programme
   mon_prog.execute(3, 10, 1, h_C);
 
   //-----------------------------------------------------------------------
   // 6. impression des resultats
   //-----------------------------------------------------------------------
   printf("\n\nMatrix C (Results)\n");
   for(int i = 0; i < size_C; i++)
   {
      printf("%f ", h_C[i]);
      if(((i + 1) % WC) == 0)
      printf("\n");
   }
   printf("\n");
 
}
