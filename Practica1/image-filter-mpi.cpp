#include <bits/stdc++.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <mpi.h>

typedef unsigned char byte;

using namespace cv;
using namespace std;

int c;
int filtro[5][5] = {
    {0, 0, 0, 0, 0},
    {0, -2, -1, 0, 0},
    {0, -1, 1, 1, 0},
    {0, 0, 1, 2, 0},
    {0, 0, 0, 0, 0},
};

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        return 1;
    }
    int i, tag = 1, tasks, iam;
    unsigned char *original, *solution;
    MPI_Status status;
    // Suma de los elementos de la matriz filtro
    c = 0;
    for (int d = 0; d < 5; d++)
    {
        for (int p = 0; p < 5; p++)
        {
            c += filtro[d][p];
        }
    }

    // Se evalua la condición de que la suma de los elementos de la matriz sea 0
    if (c == 0)
        c = 1;

    // PROCESAMIENTO DE LA IMAGEN
    MPI_Init(&argc, &argv);
    // Se obtiene la cantidad de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    // Se accede al id del proceso
    MPI_Comm_rank(MPI_COMM_WORLD, &iam);
    if (iam == 0)
    {
        // Se lee la imagen
        Mat img;
        // Se obtiene el path de la imagen
        string image_path = samples::findFile(argv[2]);

        // se obtiene el objeto Mat con parámetro "IMREAD_COLOR" para indicar los canales de imagen
        img = imread(image_path, IMREAD_COLOR);
        original = img.data;
        solution = (uchar *)malloc(img.total() * 3 * sizeof(uchar));
    }

    for (int i = id; i < img.rows; i += hilos)
    {
        for (int j = 0; j < img.cols; j++)
        {
            int index = (i * img.cols + j) * 3;
            int index2 = (i * img.cols + j) * 3;
            // Se obtienen los valores de los canales RGB de la imagen original
            *(original + index + 0) = *(img.data + index2 + 0);
            *(original + index + 1) = *(img.data + index2 + 1);
            *(original + index + 2) = *(img.data + index2 + 2);
        }
    }

#pragma omp barrier
    // //APLICACIÓN DEL FILTRO
    // Se accede al id del hilo
    // int id = omp_get_thread_num();

    // Se accede a las filas de la matriz con la imagen
    for (int i = id; i < img.rows; i += hilos)
    {
        // Se accede a las columnas de la matriz con la imagen
        for (int j = 0; j < img.cols; j++)
        {
            // Se accede a cada canal creado anteriormente
            for (int chanel = 0; chanel < 3; chanel++)
            {
                int cont = 0;

                // Se accede a las filas del canal
                for (int d = 0; d < 5; d++)
                {

                    // Se accede  a las columnas del canal
                    for (int p = 0; p < 5; p++)
                    {
                        // Se ejecuta operación del filtro
                        int nx = i + (d - 2);
                        int ny = j + (p - 2);

                        // Se evalua la condición de bordes  o valores negativos
                        if (nx < 0 || ny < 0 || nx >= img.rows || ny >= img.cols)
                            continue;

                        int index = (nx * img.cols + ny) * 3;
                        cont += *(original + index + chanel) * filtros[num_filtro][d][p];
                    }
                }
                // Se evalua la condición de que la variable sea 0
                if (cont < 0)
                    cont = 0;

                // Se evalua la condición de que la variable sea mayor que 255
                if (cont > 255)
                    cont = 255;

                // Se halla la posición donde se guarda el valor del canal actual en la imagen filtrada
                int pos = (i * img.cols + j) * 3;
                *(solution + pos + chanel) = (cont / c) % 256;
            }
        }
    }

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
    MPI_Finalize();
    return 0;
}
