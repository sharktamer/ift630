#include <stdio.h>
#include <iostream>
#include "mpi.h"
#define NB_LIGNES_A 5
#define NB_COLONNES_A 4
#define NB_COLONNES_B 3

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
    MPI_Status status;
    double     A[NB_LIGNES_A][NB_COLONNES_A];
    double     B[NB_COLONNES_A][NB_COLONNES_B];
    double     C[NB_LIGNES_A][NB_COLONNES_B];
    int        nextI = 0;
    int        nextJ = 0;

    //Initialisation de la matrice A
    for (int i = 0; i < NB_LIGNES_A; i++){
      for (int j = 0; j < NB_COLONNES_A; j++){
        A[i][j] = 1;
      }
    }

    //Initialisation de la matrice B
    for (int i = 0; i < NB_COLONNES_A; i++){
      for (int j = 0; j < NB_COLONNES_B; j++){
        B[i][j] = i;
      }
    }


    MPI_Comm_size( MPI_COMM_WORLD, &size );
    int bufCoords[2];
    double bufLigne[NB_COLONNES_A];
    double bufColonne[NB_COLONNES_A];
    int sent = 0;
    for (int i = 1; i < size; i++){
      if(sent < NB_LIGNES_A * NB_COLONNES_B){
        bufCoords[0] = nextI;
        bufCoords[1] = nextJ;
        for (int k = 0; k < NB_COLONNES_A; k++){
          bufColonne[k] = B[k][nextJ];
          bufLigne[k] = A[k][nextI];
        }

        MPI_Send(bufCoords, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(bufLigne, NB_COLONNES_A, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        MPI_Send(bufColonne, NB_COLONNES_A, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        nextJ++;
        sent++;
        if (nextJ == NB_COLONNES_B){ //CHANGE J POUR NEXTJ
          nextJ = 0;
          nextI++;
        }
      }
      else{
        bufCoords[0] = -1;
        bufCoords[1] = -1;
        MPI_Send(bufCoords, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
      }
    }

    int bufSlave[3];
    double result[1];
    int received = 0;
    while(received < NB_LIGNES_A * NB_COLONNES_B){

      MPI_Recv(bufSlave, 3, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(result, 1, MPI_DOUBLE, bufSlave[2], 0, MPI_COMM_WORLD, &status);
      received++;
      C[bufSlave[0]][bufSlave[1]] = result[0];

      if(sent < NB_LIGNES_A * NB_COLONNES_B){
        bufCoords[0] = nextI;
        bufCoords[1] = nextJ;
        for (int k = 0; k < NB_COLONNES_A; k++){
          bufColonne[k] = B[k][nextJ];
          bufLigne[k] = A[k][nextI];
        }

        MPI_Send(bufCoords, 2, MPI_INT, bufSlave[2], 0, MPI_COMM_WORLD);
        MPI_Send(bufLigne, NB_COLONNES_A, MPI_DOUBLE, bufSlave[2], 0, MPI_COMM_WORLD);
        MPI_Send(bufColonne, NB_COLONNES_A, MPI_DOUBLE, bufSlave[2], 0, MPI_COMM_WORLD);

        nextJ++;
        sent++;
        if (nextJ == NB_COLONNES_B){
          nextJ = 0;
          nextI++;
        }
      }
      else{
        bufCoords[0] = -1;
        bufCoords[1] = -1;
        MPI_Send(bufCoords, 2, MPI_INT, bufSlave[2], 0, MPI_COMM_WORLD);
      }
    }

    for(int i = 0; i < NB_LIGNES_A; i++){
      for(int j = 0; j < NB_COLONNES_B; j++){
        std::cout << C[i][j] << " ";
      }
      std::cout << std::endl;
    }


    return 0;
}




/* This is the slave */

int slave_io(int rank)
{
    MPI_Status status;
    int bufAnswer[3];
    int bufCoords[2];
    double bufLigne[NB_COLONNES_A];
    double bufColonne[NB_COLONNES_A];
    double result[1];

    while(true){
      MPI_Recv(bufCoords, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      if(bufCoords[0] < 0) break;
      MPI_Recv(bufLigne, NB_COLONNES_A, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(bufColonne, NB_COLONNES_A, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);


      result[0] = 0;
      for(int i = 0; i < NB_COLONNES_A; i++){
        result[0] += bufLigne[i] * bufColonne[i];
      }

      bufAnswer[0] = bufCoords[0];
      bufAnswer[1] = bufCoords[1];
      bufAnswer[2] = rank;
      MPI_Send(bufAnswer, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    }

    return 0;
}
