#include <bits/stdc++.h>
using namespace std;
// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#define MAX_DIM_X 4000

using namespace cv;
int blocks;
int threads;

__global__ void convolutionMatrix(unsigned char *image, unsigned char *solution, int c, int DIM_X, int DIM_Y, int numBlocks)
{
    __shared__ unsigned char segment_image[MAX_DIM_X][3][3];
    int i, channel, d, j;
    int new_row;
    int new_col;
    int filter[3][3] = {
        {-2, -1, 0},
        {-1, 1, 1},
        {0, 1, 2}};
    for (int row = blockIdx.x; row < DIM_Y; row += numBlocks)
    {
        for (i = threadIdx.x; i < DIM_X; i += blockDim.x)
        {
            new_row = row + ((i * 3) / DIM_X) - 1;
            new_col = (i * 3) % DIM_X;
            for (int j = 0; j < 3; j++)
            {
                if (new_col == DIM_X)
                {
                    new_col = 0;
                    new_row++;
                }
                if (new_row >= 0 && new_row < DIM_Y)
                {
                    segment_image[row - new_row - 1][new_col][0] = *(image + (new_col + (DIM_X * (new_row - 1))) * 3);
                    segment_image[row - new_row - 1][new_col][1] = *(image + (new_col + (DIM_X * (new_row - 1))) * 3 + 1);
                    segment_image[row - new_row - 1][new_col][2] = *(image + (new_col + (DIM_X * (new_row - 1))) * 3 + 2);
                }
            }
        }
        __syncthreads();
        for (i = threadIdx.x; i < DIM_X; i += blockDim.x)
        {
            for (channel = 0; channel < 3; channel++)
            {
                int cont = 0;
                for (d = 0; d < 3; d++)
                {
                    for (j = 0; j < 3; j++)
                    {
                        int nx = i - d;
                        if (nx < 0 || nx >= DIM_X)
                            continue;
                        cont += segment_image[nx][d][channel] * filter[d][j];
                    }
                }
                // Se evalua la condición de que la variable sea 0
                if (cont < 0)
                    cont = 0; // Se evalua la condición de que la variable sea mayor que 255
                if (cont > 255)
                    cont = 255;
                int pos = (i + DIM_Y * row) * 3;
                *(solution + pos + channel) = (cont / c) % 256;
            }
        }
        __syncthreads();
    }
}

/*****************************************************************************/

int filtro(string image_i, string image_o)
{
    // Se obtiene el path de la imagen
    string image_path = samples::findFile(image_i);

    // se obtiene el objeto Mat con parámetro "IMREAD_COLOR" para indicar los canales de imagen
    Mat img = imread(image_path, IMREAD_COLOR);

    // Tamaño de la imagen
    int img_size = img.total() * 3 * sizeof(unsigned char);

    // Se crea un apuntador a la memoria de la imagen
    unsigned char *original = img.data;
    // Se crea un apuntador a la memoria de la imagen en CUDA
    unsigned char *originalCUDA;
    // Se reserva memoria para la imagen en CUDA
    cudaMalloc((void **)&originalCUDA, img_size);
    // Se copia la imagen a la memoria de la imagen en CUDA
    cudaMemcpy(originalCUDA, original, img_size, cudaMemcpyHostToDevice);
    // Se crea un apuntador para la imagen de salida
    unsigned char *solution = (unsigned char *)malloc(img_size);
    // Se crea un apuntador para la imagen de salida en CUDA
    unsigned char *solutionCUDA;
    // Se asigna la memoria de la imagen de salida en CUDA
    cudaMalloc((void **)&solutionCUDA, img_size);

    int filtro[3][3] = {
        {-2, -1, 0},
        {-1, 1, 1},
        {0, 1, 2},
    },

        // Suma de los elementos de la matriz filtro
        int c = 0;
    for (int d = 0; d < 3; d++)
    {
        for (int p = 0; p < 3; p++)
        {
            c += filtro[d][p];
        }
    }

    // Se evalua la condición de que la suma de los elementos de la matriz sea 0
    if (c == 0)
        c = 1;

    // PROCESAMIENTO DE LA IMAGEN
    // #pragma omp parallel num_threads(hilos)
    //     {
    //         // Se accede al id del hilo
    //         int id = omp_get_thread_num();

    //         for (int i = id; i < img.rows; i += hilos)
    //         {
    //             for (int j = 0; j < img.cols; j++)
    //             {
    //                 int index = (i * img.cols + j) * 3;
    //                 int index2 = (i * img.cols + j) * 3;
    //                 // Se obtienen los valores de los canales RGB de la imagen original
    //                 *(original + index + 0) = *(img.data + index2 + 0);
    //                 *(original + index + 1) = *(img.data + index2 + 1);
    //                 *(original + index + 2) = *(img.data + index2 + 2);
    //             }
    //         }
    //     }
    const int cu_rows = img.rows, cu_cols = img.cols, cu_c = c, cu_b = blocks;
    convolutionMatrix<<<blocks, threads>>>(originalCUDA, solutionCUDA, cu_c, cu_rows, cu_cols, cu_b);

    img = Mat(img.rows, img.cols, CV_8UC3, solution);
    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }
    imwrite(image_o, img);
    int k = waitKey(0);
    free(original);
    free(solution);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Usage: " << argv[0] << " <image_i> <image_o> <bloques> <hilos>" << endl;
        return 1;
    }
    blocks = atoi(argv[3]);
    threads = atoi(argv[4]);
    filtro(argv[1], argv[2]);
}