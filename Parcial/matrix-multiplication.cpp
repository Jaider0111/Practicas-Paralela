#include <bits/stdc++.h>
#include "omp.h"
#include <stdlib.h>
#include <iostream>
#include <chrono>

using namespace std;

int hilos;
int *matriz_a;
int *matriz_b;

// Se declara el tamaño de las matrices
int n;

// Se declara la matriz resultante
int *resultado;

void readMatrixs()
{
    cin >> n;
    matriz_a = (int *)malloc(n * n * sizeof(int));
    matriz_b = (int *)malloc(n * n * sizeof(int));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> matriz_a[i * n + j];
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> matriz_b[i * n + j];
        }
    }
}

// Función para hacer la mostrar el resultado de la multiplicación de las matrices
int result_mult_matrix()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << *(resultado + i * n + j) << " ";
        }
        cout << "\n";
    }
    return 0;
}

// Función para multiplicar dos matrices nxn
int matrix_mult()
{
    auto start = chrono::high_resolution_clock::now();
    resultado = (int *)malloc(n * n * sizeof(int));
    memset(resultado, 0, n * n * sizeof(int));

// Se inicia la paralelización
#pragma omp parallel num_threads(hilos)
    {
        // Se accede al id del hilo
        int id = omp_get_thread_num();

        // Se accede a las filas de las matrices
        for (int i = 0; i < n; i++)
        {
            // Se accede a las columnas de las matrices con saltos de tamaño hilos
            for (int j = id; j < n; j += hilos)
            {
                // Se accede a los elementos de la columna para la matriz a y la fila de la matriz b
                for (int k = 0; k < n; k++)
                {

                    // Se realiza la sumatoria de la multiplicación de los elementos de las matrices
                    *(resultado + i * n + j) += matriz_a[i * n + k] * matriz_b[k * n + j];
                }
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();

    auto int_s = chrono::duration_cast<chrono::microseconds>(end - start);

    // cerr << "MatrixMult elapsed time is " << int_s.count() / (float)1e6 << " seconds " << endl;
    cerr << int_s.count() / (float)1e6 << endl;
    result_mult_matrix();
    free(resultado);
    free(matriz_a);
    free(matriz_b);
    return 0;
}

int main(int argc, char *argv[])
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    // Se validan argumentos
    if (argc != 2)
    {
        return 1;
    }
    // se obtienen número de hilos
    hilos = atoi(argv[1]);

    // Se obtienen las matrices
    readMatrixs();

    // Se llama a la función paralelizada
    matrix_mult();
    // Compile: g++ matrix-multiplication.cpp -o multiplicacionMatrices -fopenmp
    // run: ./multiplicacionMatrices {hilos} {matriz A} {matriz b}
}