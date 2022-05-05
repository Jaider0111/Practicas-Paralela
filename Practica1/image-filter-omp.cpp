#include <bits/stdc++.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
//#include <pthread.h>
#include "filtros.hpp"
#include "omp.h"

typedef unsigned char byte;

using namespace cv;
using namespace std;

Mat img;
uchar *original;
uchar *solution;

int hilos;
int num_filtro;
int c;


int filtro(string image_i, string image_o)
{
    // Se obtiene el path de la imagen
    string image_path = samples::findFile(image_i);

    // se obtiene el objeto Mat con parámetro "IMREAD_COLOR" para indicar los canales de imagen
    img = imread(image_path, IMREAD_COLOR);
    original = (uchar *)malloc(img.total() * 3 * sizeof(uchar));
    solution = (uchar *)malloc(img.total() * 3 * sizeof(uchar));

    // Suma de los elementos de la matriz filtro
    c = 0;
    for (int d = 0; d < 5; d++)
    {
        for (int p = 0; p < 5; p++)
        {
            c += filtros[num_filtro][d][p];
        }
    }

    // Se evalua la condición de que la suma de los elementos de la matriz sea 0
    if (c == 0)
        c = 1;

    //PROCESAMIENTO DE LA IMAGEN
    #pragma omp parallel num_threads(hilos)
    {
        // Se accede al id del hilo
        int id = omp_get_thread_num();

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

    }
    //APLICACIÓN DEL FILTRO
    #pragma omp parallel num_threads(hilos)
    {
        // Se accede al id del hilo
        int id = omp_get_thread_num();

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
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        return 1;
    }
    hilos = atoi(argv[4]);
    num_filtro = atoi(argv[3]) - 1;
    filtro(argv[1], argv[2]);
}
