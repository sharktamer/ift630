#include <CL/cl.h>
#include <iostream>
#include "../contexte.h"
#include "../program.h"

/////////////////////////////////////////////////////////
// Programme principal
/////////////////////////////////////////////////////////
using namespace std;
 
int main()
{
    const  int WA = 50;
 
    int* h_A = new int[WA];
    int* h_B = new int[10];
    int* flag = new int[1];
    int* test1 = new int[1];

    h_A[0]=1000;
    for(int i = 0 ; i < 10; i++)
         h_B[i]=1000+i;
    flag[0]=9;
    Contexte gpu;
    Program mon_prog(gpu, "kernel.cl", "obtenir_valeur");

    mon_prog.initPara(h_A,  WA, SORTIE, h_B, 10, ENTREE, flag, 1, ENTREE, test1, 1, ENTREE, 10);
    mon_prog.execute(1, 10, 1, h_A);
//-----------------------------------------------------------------------
// 9. imprime les resultats
//-----------------------------------------------------------------------
    cout << endl << "Resultats " << endl;
    cout <<" Nombre de dimensions : " <<  h_A[0] << endl;
    cout <<" Dimension globale        (x,y,z) : " <<  h_A[4]  << " " << h_A[5]  << " " << h_A[6]  << endl;
    cout <<" Identificateurs globaux  (x,y,z) : " <<  h_A[1]  << " " << h_A[2]  << " " << h_A[3]  << endl;
    cout <<" Offset global            (x,y,z) : " <<  h_A[7]  << " " << h_A[8]  << " " << h_A[9]  << endl;
    cout <<" Nombre de groupes dans   (x,y,z) : " <<  h_A[19] << " " << h_A[20] << " " << h_A[21] << endl;
    cout <<" Identificateur du groupe (x,y,z) : " <<  h_A[10] << " " << h_A[11] << " " << h_A[12] << endl;
    cout <<" Dimension locale         (x,y,z) : " <<  h_A[16] << " " << h_A[17] << " " << h_A[18] << endl;
    cout <<" Identificatuers locaux   (0,1,2) : " <<  h_A[13] << " " << h_A[14] << " " << h_A[15] << endl;
    cout <<" Valeurs quelconques : " <<  h_A[22]  << " " << h_A[23]  << " " << h_A[24] << endl;
    cout << endl;

return 0;
}


