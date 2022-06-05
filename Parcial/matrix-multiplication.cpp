#include <bits/stdc++.h>
#include "omp.h"
#include <stdlib.h>
#include <iostream>
#include "matrices.hpp"
#include "matrices2.hpp"
;
typedef unsigned char byte;

using namespace std;


int hilos;
int matriz_a;
int matriz_b;
const int n = 1000;
int resultado[n][n];


int init_matrix(){
    for(int i = 0; i < n; ++i)
        for( int j = 0; j < n; ++j)
        {
            resultado[i][j]=0;
        }
    return 0;
}
int result_mult_matrix(){

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout<<resultado[i][j]<<" ";
            
        }
        std::cout<<"\n";
    }
    return 0;

}

int matrix_mult(){


    #pragma omp parallel num_threads(hilos)
    {
        // Se accede al id del hilo
        int id = omp_get_thread_num();

        init_matrix();
        for (int i = 0; i < n; i++)
        {
            for (int j = id; j < n; j+=hilos)
            {
                for(int k = 0; k < n; k++){
                    resultado[i][j] +=  matrices2[matriz_a][i][k] * matrices2[matriz_b][k][j];

                }
                
            }
        }
        //result_mult_matrix();
    } 
    return 0;

}

int main(int argc, char *argv[])
{
    std::cout<<argc<<endl;

    if (argc != 4)
    {
        return 1;
    }
    hilos = atoi(argv[1]);
    matriz_a = atoi(argv[2]) - 1;
    matriz_b = atoi(argv[3]) - 1;
    matrix_mult();


}