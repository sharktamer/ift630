#include <stdio.h>
#include <iostream>
#include <cmath>
#include "mpi.h"
#define EPSILON 1e-2
#define NB_LIGNES 6
#define NB_COLONNES 6

int master_io();
int slave_io(int);

int main(int  argc, char **argv )
{
    int rank, size;
    MPI_Comm new_comm;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_split( MPI_COMM_WORLD, rank == 0, 0, &new_comm );
    if (rank == 0)
        master_io();
    else
        slave_io(rank);

    MPI_Finalize( );
    return 0;
}



/* This is the master */

int master_io()
{
    int        size;
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Status status;
    double     A[NB_LIGNES][NB_COLONNES];

    //Initialisation de la matrice A selon l'exemple
    for (int i = 0; i < NB_LIGNES; i++){
      for (int j = 0; j < NB_COLONNES; j++){
        if(i==0 || i == NB_LIGNES -1){
          A[i][j] = -1;
        }
        else if(j==0 || j==NB_COLONNES-1){
          A[i][j] = -1;
        }
        else{
          A[i][j] = i;
        }
      }
    }


    //Separation de la matrice
    int nbLignesParProcessus = (NB_LIGNES - 2) / (size - 1);
    int reste = (NB_LIGNES - 2) % (size - 1);
    int lignes;
    int nextLine = 1;
    for (int i = 1; i < size; i++){
      lignes = nbLignesParProcessus;
      if (reste > 0) {
        lignes++;
        reste--;
      }
      MPI_Send(&lignes, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      for(int j = -1; j <= lignes; j++){
        MPI_Send(A[nextLine+j], NB_COLONNES, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      }
      nextLine += lignes;
    }

    //Iterations
    double diffNorme = 0;
    double diffNormeGlobale = 0;
    double bufLigne[NB_COLONNES];
    while(true){
      MPI_Allreduce(&diffNorme, &diffNormeGlobale, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
      diffNormeGlobale = sqrt(diffNormeGlobale);
      if (diffNormeGlobale > EPSILON){
        MPI_Send(A[0], NB_COLONNES, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
        for(int i = 1; i < size; i++){
          MPI_Recv(bufLigne, NB_COLONNES, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
          if (i > 1){
            MPI_Send(bufLigne, NB_COLONNES, MPI_DOUBLE, i-1, 1, MPI_COMM_WORLD);
          }
          MPI_Recv(bufLigne, NB_COLONNES, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
          if (i < (size - 1)){
            MPI_Send(bufLigne, NB_COLONNES, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD);
          }
        }
        MPI_Send(A[NB_LIGNES - 1], NB_COLONNES, MPI_DOUBLE, size - 1, 1, MPI_COMM_WORLD);
      }
      else{
        reste = (NB_LIGNES - 2) % (size - 1);
        nextLine = 1;
        for (int i = 1; i < size; i++){
          lignes = nbLignesParProcessus;
          if (reste > 0) {
            lignes++;
            reste--;
          }
          for(int j = 0; j < lignes; j++){
            MPI_Recv(A[nextLine+j], NB_COLONNES, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
          }
          nextLine += lignes;
        }
        break;
      }
    }

    //Impression de la matrice
    for(int i = 0; i < NB_LIGNES; i++){
      for(int j = 0; j < NB_COLONNES; j++){
        if(A[i][j]>0) std::cout << " ";
        printf("%.2f ", A[i][j]);
      }
      std::cout << std::endl;
    }

    return 0;
  }

  int slave_io(int rank)
  {
    MPI_Status status;


    //Construction de la sous matrice
    int lignes;
    MPI_Recv(&lignes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    lignes += 2;
    double A[lignes][NB_COLONNES];
    for (int i = 0; i < lignes; i++){
      MPI_Recv(A[i], NB_COLONNES, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    //Iterations
    double diffNorme = 0;
    double diffNormeGlobale = 0;
    double newA[lignes][NB_COLONNES];
    while(true){
      diffNorme = 0;
      //Calcul
      for(int i = 1; i < lignes - 1; i++){
        for(int j = 1; j < NB_COLONNES - 1; j++){
          newA[i][j] =  (A[i-1][j] + A[i+1][j] + A[i][j-1] + A[i][j+1]) / 4.;
          diffNorme += (newA[i][j] - A[i][j]) * (newA[i][j] - A[i][j]);
        }
      }

      //Mise a jour de A
      for(int i = 1; i < lignes - 1; i++){
        for(int j = 1; j < NB_COLONNES - 1; j++){
          A[i][j] =  newA[i][j];
        }
      }

      /*
      for(int i = 0; i < lignes; i++){
        for(int j = 0; j < NB_COLONNES; j++){
          if(A[i][j]>0) std::cout << " ";
          printf("%.2f ", A[i][j]);
        }
        std::cout << std::endl;
      }*/

      //Envoi des resultats
      MPI_Allreduce(&diffNorme, &diffNormeGlobale, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
      diffNormeGlobale = sqrt(diffNormeGlobale);
      //printf("%.2f\n", diffNormeGlobale);
      if (diffNormeGlobale > EPSILON){
        MPI_Send(A[1], NB_COLONNES, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(A[lignes - 2], NB_COLONNES, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        MPI_Recv(A[0], NB_COLONNES, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(A[lignes - 1], NB_COLONNES, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
      }
      else{
        for(int j = 1; j < lignes - 1 ; j++){
          MPI_Send(A[j], NB_COLONNES, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
        break;
      }
    }

    return 0;
  }
