
//Main program (Listing 4)

// Multiply two matrices A * B = C
 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <CL/cl.h>
#include <random>
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
double executionTime(cl_event &event)
{
    cl_ulong start, end;
    
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
   
    return (double)1.0e-9 * (end - start); // convert nanoseconds to seconds on return
}


/////////////////////////////////////////////////////////
// Retourne une valeur aleatoire ente val1 et val2
/////////////////////////////////////////////////////////
int valeurAleatoire(int val1, int val2)
{
   static std::default_random_engine         e{};
   static std::uniform_int_distribution<int> d{val1, val2};
   //return d(e);
   return(3);
} 


/////////////////////////////////////////////////////////
// Program principale qui multiplie deux matrices
/////////////////////////////////////////////////////////
using namespace std;
 
int main()
{
 
   // set seed for rand()
   srand(2012);
 
   //===========================================
   // Allocation de la memoire pour les matrices
   int size_A = WA * HA;
   float* h_A = new float[size_A]; 
   int size_B = WB * HB;
   float* h_B = new float[size_B];   
   // La matrice de résultats 
   int size_C = WC * HC;
   float* h_C = new float[size_C];
   //------------------

   // Initialisation des matrices
   for(int i = 0; i < size_A; i++)
   {  h_A[i] = valeurAleatoire(1,10);  }
   for(int i = 0; i < size_B; i++)
   {  h_B[i] = valeurAleatoire(1,10);  }
 
   //=============================================
   // Impression des matrices
   cout << "\n\nMatrice A" << endl;
   for(int i = 0; i < size_A; i++)
   {
      cout << h_A[i]<< " " ;
      if(((i + 1) % WA) == 0) cout << endl;
   }
 
   cout << "\n\nMatrix B" << endl;;
   for(int i = 0; i < size_B; i++)
   {
      cout << h_B[i]<< " " ;   
      if(((i + 1) % WB) == 0) cout << endl ;
   }
   //------------------
 
   //===============================================
   // On initialise et execute le programme opencl
   Contexte mon_contexte;
   Program mon_prog(mon_contexte, "./kernel.cl", "matrixMul");
   mon_prog.initPara(h_C, size_C, SORTIE, h_A, size_A, ENTREE, h_B, size_B, ENTREE, WA, WC);
   mon_prog.execute(2, 10, 10, h_C);
   //------------
 
   //===============================================
   // On affiche les résultats
   cout << endl << "Matrice C (Resultats)" << endl;
   for(int i = 0; i < size_C; i++)
   {
      cout << h_C[i] << " ";
      if(((i + 1) % WC) == 0)  cout << endl;
   }
   cout << endl;
   //---------------------------------------------


}
