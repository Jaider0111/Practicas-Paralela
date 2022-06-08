#include <bits/stdc++.h>
#include "omp.h"
#include <stdlib.h>
#include <iostream>
#include "matrices.hpp"
#include "matrices1000.hpp"
#include "matrices2000.hpp"
;
typedef unsigned char byte;

using namespace std;


int hilos;
int matriz_a;
int matriz_b;

//Se inicializa el tamaño de las matrices
const int n = 2000;

//Se declara la matriz resultante
int resultado[n][n];


//Función para inicializar la matriz resultante en 0
int init_matrix(){
    for(int i = 0; i < n; ++i)
        for( int j = 0; j < n; ++j)
        {
            resultado[i][j]=0;
        }
    return 0;
}

//Función para hacer la mostrar el resultado de la multiplicación de las matrices
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

//Función para multiplicar dos matrices nxn 
int matrix_mult(){

    //Se inicia la paralelización 
    #pragma omp parallel num_threads(hilos)
    {
        // Se accede al id del hilo
        int id = omp_get_thread_num();

        //Se inicializa la matriz resultante
        init_matrix();

        //Se accede a las filas de las matrices
        for (int i = 0; i < n; i++)
        {
            //Se accede a las columnas de las matrices con saltos de tamaño hilos
            for (int j = id; j < n; j+=hilos)
            {
                //Se accede a los elementos de la columna para la matriz a y la fila de la matriz b
                for(int k = 0; k < n; k++){

                    //Se realiza la sumatoria de la multiplicación de los elementos de las matrices
                    resultado[i][j] +=  matrices2000[matriz_a][i][k] * matrices2000[matriz_b][k][j];
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

    //Se validan argumentos
    if (argc != 4)
    {
        return 1;
    }
    //se obtienen número de hilos
    hilos = atoi(argv[1]);

    //Se obtiene la matriz a
    matriz_a = atoi(argv[2]) - 1;

    //se obtiene la matriz b
    matriz_b = atoi(argv[3]) - 1;

    //Se llama a la función paralelizada
    matrix_mult();


    //Compile: g++ matrix-multiplication.cpp -o multiplicacionMatrices -fopenmp
    //run: ./multiplicacionMatrices {hilos} {matriz A} {matriz b}



}