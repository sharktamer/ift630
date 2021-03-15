
// Multiplication de matrices en OpenCL


// 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <CL/cl.hpp>
#include "oclUtils.h"
#include "shrQATest.h"
#include <iostream>
#include <fstream>

 

#define WA 100
#define HA 100
#define WB 100
#define HB 100
#define WC 100
#define HC 100

////////////////////////////////////////////////////////////////////////////////
// Fonctions utiles
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

// Allocates a matrix with random float entries.
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

   // Pour la generation aleatoire
   srand(2012);

   // 1. allocation de la mémoire pour les matrices A et B
   unsigned int size_A = WA * HA;
   unsigned int mem_size_A = sizeof(float) * size_A;
   float* h_A = (float*) malloc(mem_size_A);

   unsigned int size_B = WB * HB;
   unsigned int mem_size_B = sizeof(float) * size_B;
   float* h_B = (float*) malloc(mem_size_B);

   // 2. initialisation des matrices
   randomInit(h_A, size_A);
   randomInit(h_B, size_B);

   // 3. impression des matrices
   printf("\n\nMatrix A\n");
   for(unsigned int i = 0; i < size_A; i++)
   {
      printf("%f ", h_A[i]);
      if(((i + 1) % WA) == 0)
      printf("\n");
   }

   printf("\n\nMatrix B\n");
   for(unsigned int i = 0; i < size_B; i++)
   {
      printf("%f ", h_B[i]);
      if(((i + 1) % WB) == 0)
      printf("\n");
   }

   // 4. allocation de l'espace pour le resultat
   unsigned int size_C = WC * HC;
   unsigned int mem_size_C = sizeof(float) * size_C;
   float* h_C = (float*) malloc(mem_size_C);

   /*****************************************/
   // 5. Initialisation de  OpenCL
   /*****************************************/
   // variables necessaires a OpenCL
   cl_command_queue file1;
   cl_program clProgram;
   cl_kernel clKernel;

   size_t dataBytes;
   size_t kernelLength;
   cl_int errcode;

   // espace memoire des matrices sur la carte graphique
   cl_mem d_A;
   cl_mem d_B;
   cl_mem d_C;

   //--------------------------------------------------------
   // obtention de la plate-forme (nvidia, ATI, intel, ...
   //---------------------------------------------------------
   cl_uint nPlatform = 0;
   // obtient le nombre de plateforme....
   clGetPlatformIDs(0,NULL,&nPlatform);
   // on alloue de l'espace pour mettre id des plateformes
   cl_platform_id* plInfos = (cl_platform_id*)malloc(nPlatform * sizeof(cl_platform_id));
   // on obtient les identificateurs des plateformes
   clGetPlatformIDs(nPlatform,plInfos,NULL);

   // on obtient les informations sur la plateforme 0
   char result[1000];
   clGetPlatformInfo(plInfos[0],CL_PLATFORM_PROFILE,1000,result,NULL);
   std::cout << "Profil : " << result << endl;
   clGetPlatformInfo(plInfos[0],CL_PLATFORM_VERSION,1000,result,NULL);
   std::cout << "Version : " << result << endl;
   clGetPlatformInfo(plInfos[0],CL_PLATFORM_NAME,1000,result,NULL);
   std::cout << "Name : " << result << endl;
   clGetPlatformInfo(plInfos[0],CL_PLATFORM_VENDOR,1000,result,NULL);
   std::cout << "Vendeur : " << result << endl;
   clGetPlatformInfo(plInfos[0],CL_PLATFORM_EXTENSIONS,1000,result,NULL);
   std::cout << "Extensions : " << result << endl;

   //----------------------------------------------------
   // obtention du periphirique precis (type de GPU)
   //----------------------------------------------------
   cl_uint nDev = 0;
   // on obtient le nbr de peri pour plateforme 0
   clGetDeviceIDs(plInfos[0],CL_DEVICE_TYPE_GPU,0,0,&nDev);
   std::cout << "Nombre de peripheriques = " << nDev << endl;
   // on alloue l'espace pour l'information
   cl_device_id* peri = (cl_device_id*)malloc(nDev * sizeof(cl_device_id));
   // on obtient les identificateurs des peripheriques
   clGetDeviceIDs(plInfos[0],CL_DEVICE_TYPE_GPU,nDev,peri,0);

   //----------------------------------------------------------------
   // creation d'un contexte pour executer OpenCL
   //---------------------------------------------------------------
   cl_context ctxt1;
   cout << "Creation du contexte....."<< endl;
   ctxt1 = clCreateContext(0, nDev, peri, NULL, NULL, &errcode);
   shrCheckError(errcode, CL_SUCCESS);
   cout << "Contexte cree.....succes....."<< endl;
   // obtention de la liste des GPU assicies au contexte
   clGetContextInfo(ctxt1, CL_CONTEXT_DEVICES, 0, NULL, &dataBytes);
   cl_device_id *dev = (cl_device_id *)  malloc(dataBytes);
   clGetContextInfo(ctxt1, CL_CONTEXT_DEVICES, dataBytes, dev, NULL);

   //-----------------------------------------------------------------------
   // creation d'une file pour les programmes a executer sur le GPU
   //-----------------------------------------------------------------------
   file1 = clCreateCommandQueue(ctxt1, dev[0], CL_QUEUE_PROFILING_ENABLE , &errcode);
   shrCheckError(errcode, CL_SUCCESS);
   cout << "Files d'execution creees .... succes....." << endl;

   //-----------------------------------------------------------------------
   // allocation de la memoire sur le GPU
   //-----------------------------------------------------------------------
   d_C = clCreateBuffer(ctxt1, CL_MEM_READ_WRITE, mem_size_A, NULL, &errcode);
   d_A = clCreateBuffer(ctxt1, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,mem_size_A, h_A, &errcode);
   d_B = clCreateBuffer(ctxt1, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,mem_size_B, h_B, &errcode);


   //-----------------------------------------------------------------------
   // 6. Chargement et compilation du kernel OpenCL
   //-----------------------------------------------------------------------
   char *clMatrixMul = oclLoadProgSource("kernel.cl", "// My comment\n", &kernelLength);
   shrCheckError(clMatrixMul != NULL, shrTRUE);

   clProgram = clCreateProgramWithSource(ctxt1, 1, (const char **)&clMatrixMul, &kernelLength, &errcode);
   shrCheckError(errcode, CL_SUCCESS);

   errcode = clBuildProgram(clProgram, 0, NULL, NULL, NULL, NULL);
   shrCheckError(errcode, CL_SUCCESS);

   clKernel = clCreateKernel(clProgram, "matrixMul", &errcode);
   shrCheckError(errcode, CL_SUCCESS);


   //-----------------------------------------------------------------------
   // 7. Execution du kernel
   //-----------------------------------------------------------------------
   size_t localW[2], globalW[2];

   int wA = WA;
   int wC = WC;
   // on passe les parametres
   errcode = clSetKernelArg(clKernel, 0, sizeof(cl_mem), (void *)&d_C);
   errcode |= clSetKernelArg(clKernel, 1,sizeof(cl_mem), (void *)&d_A);
   errcode |= clSetKernelArg(clKernel, 2,sizeof(cl_mem), (void *)&d_B);
   errcode |= clSetKernelArg(clKernel, 3,sizeof(int), (void *)&wA);
   errcode |= clSetKernelArg(clKernel, 4,sizeof(int), (void *)&wC);
   shrCheckError(errcode, CL_SUCCESS);

   size_t loc;
   clGetKernelWorkGroupInfo(clKernel, dev[0], CL_KERNEL_WORK_GROUP_SIZE, sizeof(loc), &loc, NULL);
   cout << "Taille du groupe selon kernel = " << loc << endl;

   localW[0] = 32;
   localW[1] = 32;

   globalW[0] = shrRoundUp(localW[0], WA);
   globalW[1] = shrRoundUp(localW[1], WA);

   // envoie la demande d'execution
   errcode = clEnqueueNDRangeKernel(file1,clKernel, 2, NULL, globalW, localW, 0, NULL, NULL);
   shrCheckError(errcode, CL_SUCCESS);

   //-----------------------------------------------------------------------
   // 8. Recuperation les resultats
   //-----------------------------------------------------------------------
   errcode = clEnqueueReadBuffer(file1, d_C, CL_TRUE, 0, mem_size_C, h_C, 0, NULL, NULL);
   shrCheckError(errcode, CL_SUCCESS);

   //-----------------------------------------------------------------------
   // 9. impression des resultats
   //-----------------------------------------------------------------------
   printf("\n\nMatrix C (Results)\n");
   for(unsigned int i = 0; i < size_C; i++)
   {
      printf("%f ", h_C[i]);
      if(((i + 1) % WC) == 0)
      printf("\n");
   }
   printf("\n");

   //-----------------------------------------------------------------------
   // 10. Liberation la memoire
   //-----------------------------------------------------------------------
   free(h_A);
   free(h_B);
   free(h_C);

   clReleaseMemObject(d_A);
   clReleaseMemObject(d_C);
   clReleaseMemObject(d_B);

   free(dev);
   free(clMatrixMul);
   clReleaseContext(ctxt1);
   clReleaseKernel(clKernel);
   clReleaseProgram(clProgram);
   clReleaseCommandQueue(file1);

}
