#include <bits/stdc++.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <mpi.h>
#include <chrono>

#define MAX_PROCESS 32

typedef unsigned char byte;

using namespace cv;
using namespace std;

int filtro[3][3] = {
    {-2, -1, 0},
    {-1, 1, 1},
    {0, 1, 2},
};

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        return 1;
    }
    int i, tag = 1, tasks, iam, c, imgRows, imgCols, root = 0;
    MPI_Status status;
    MPI_Request req[MAX_PROCESS];
    uchar *solutionPros, *solution, *original;
    // Se lee la imagen
    Mat img;

    // PROCESAMIENTO DE LA IMAGEN
    MPI_Init(&argc, &argv);
    auto start = chrono::high_resolution_clock::now();
    // Se obtiene la cantidad de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    // Se accede al id del proceso
    MPI_Comm_rank(MPI_COMM_WORLD, &iam);
    // Suma de los elementos de la matriz filtro
    c = 0;
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
    // Se obtiene el path de la imagen
    string image_path = samples::findFile(argv[1]);

    // se obtiene el objeto Mat con parámetro "IMREAD_COLOR" para indicar los canales de imagen
    img = imread(image_path, IMREAD_COLOR);
    imgRows = img.rows;
    imgCols = img.cols;
    original = img.data;
    if (iam == root)
    {
        solution = (uchar *)malloc(imgRows * imgCols * 3 * sizeof(uchar));
    }
    int processRows = iam < imgRows % tasks ? ceil(imgRows / tasks) : floor(imgRows / tasks);
    solutionPros = (uchar *)malloc(processRows * imgCols * 3 * sizeof(uchar));

    // //APLICACIÓN DEL FILTRO
    int ri;

    int lim = imgRows % tasks == 0 ? tasks : imgRows % tasks;
    int aux = iam < lim ? iam * processRows - 1 : lim * (processRows + 1) - 1 + (iam - lim) * processRows;
    aux = iam == root ? 0 : aux;
    cout << iam << ' ' << aux << '\n';
    // Se accede a las filas de la matriz con la imagen
    for (int i = 0; i < processRows; i++)
    {
        if (iam == root && i == processRows)
        {
            ri = imgRows - 1;
        }
        else
        {
            ri = i;
        }
        // Se accede a las columnas de la matriz con la imagen
        for (int j = 0; j < imgCols; j++)
        {

            // Se accede a cada canal creado anteriormente
            for (int chanel = 0; chanel < 3; chanel++)
            {
                int cont = 0;

                // Se accede a las filas del canal
                for (int d = 0; d < 3; d++)
                {

                    // Se accede  a las columnas del canal
                    for (int p = 0; p < 3; p++)
                    {
                        // Se ejecuta operación del filtro
                        int nx = aux + ri + (d - 1);
                        int ny = j + (p - 1);

                        // Se evalua la condición de bordes  o valores negativos
                        if (nx < 0 || ny < 0 || nx >= imgRows || ny >= imgCols)
                            continue;

                        int index = (nx * imgCols + ny) * 3;
                        cont += *(original + index + chanel) * filtro[d][p];
                    }
                }
                // Se evalua la condición de que la variable sea 0
                if (cont < 0)
                    cont = 0;

                // Se evalua la condición de que la variable sea mayor que 255
                if (cont > 255)
                    cont = 255;

                // Se halla la posición donde se guarda el valor del canal actual en la imagen filtrada
                int pos = (ri * imgCols + j) * 3;
                *((iam == root ? solution : solutionPros) + pos + chanel) = (cont / c) % 256;
            }
        }
    }
    if (iam == 0)
    {
        int lim = imgRows % tasks == 0 ? tasks : imgRows % tasks;
        for (int i = 1; i < lim; i++)
        {
            MPI_Recv(solution + (i * processRows - 1) * imgCols * 3, processRows * imgCols * 3, MPI_UNSIGNED_CHAR, i, tag, MPI_COMM_WORLD, &status);
        }
        for (int i = lim; i < tasks; i++)
        {
            int aux = lim * processRows + (i - lim) * (processRows - 1) - 1;
            MPI_Recv(solution + aux * imgCols * 3, (processRows - 1) * imgCols * 3, MPI_UNSIGNED_CHAR, i, tag, MPI_COMM_WORLD, &status);
        }
    }
    else
    {
        MPI_Send(solutionPros, processRows * imgCols * 3, MPI_UNSIGNED_CHAR, root, tag, MPI_COMM_WORLD);
    }

    if (iam == root)
    {
        img = Mat(imgRows, imgCols, CV_8UC3, solution);
        imwrite(argv[2], img);
        free(solution);
        auto end = chrono::high_resolution_clock::now();

        auto int_s = chrono::duration_cast<chrono::microseconds>(end - start);

        // cerr << "MatrixMult elapsed time is " << int_s.count() / (float)1e6 << " seconds " << endl;
        cerr << int_s.count() / (float)1e6 << endl;
    }
    free(solutionPros);
    MPI_Finalize();
    return 0;
}
