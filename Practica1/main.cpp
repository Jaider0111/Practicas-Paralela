#include <bits/stdc++.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

typedef unsigned char byte;

using namespace cv;
using namespace std;


//Matriz filtro
int matriz[5][5] = {
    {0, 0, 0, 0, 0},
    {0, -2, -1, 0, 0},
    {0, -1, 1, 1, 0},
    {0, 0, 1, 2, 0},
    {0, 0, 0, 0, 0}};
    
    int filtro( string image_path  , int hilos){
		
		
		
	// Se obtiene el path de la imagen
    string image_path = samples::findFile(image_path);
    
    // se obtiene el objeto Mat con parámetro "IMREAD_COLOR" para indicar los canales de imagen
    Mat img = imread(image_path, IMREAD_COLOR);
    uchar *original = (uchar *)malloc(img.total() * 3 * sizeof(uchar));
    uchar *solution = (uchar *)malloc(img.total() * 3 * sizeof(uchar));
    
    int rows_per_thread = img.rows/hilos;
    int  cols_per_thread = img.cols/hilos;
    
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            int index = (i * img.cols + j) * 3;
            int index2 = (i * img.cols + j) * 3;
            // copy while converting to RGBA order
            *(original + index + 0) = *(img.data + index2 + 0);
            *(original + index + 1) = *(img.data + index2 + 1);
            *(original + index + 2) = *(img.data + index2 + 2);
        }
    }
    
    //Suma de los elementos de la matriz filtro
    int c = 0;
    for (int d = 0; d < 5; d++)
    {
        for (int p = 0; p < 5; p++)
        {
            c += matriz[d][p];
        }
    }
    
    //Se evalua la condición de que la suma de los elementos de la matriz sea 0
    if (c == 0)
        c = 1;
     //Se accede a las filas de la matriz con la imagen
    for (int i = 0; i < img.rows; i++)
    {
		//Se accede a las columnas de la matriz con la imagen
        for (int j = 0; j < img.cols; j++)
        {
			//Se accede a cada canal creado anteriormente
            for (int chanel = 0; chanel < 3; chanel++)
            {
                int cont = 0;
                
				//Se accede a las filas del canal
                for (int d = 0; d < 5; d++)
                {
					
					//Se accede  a las columnas del canal
                    for (int p = 0; p < 5; p++)
                    {
						//Se ejecuta operación del filtro
                        int nx = i + (d - 2);
                        int ny = j + (p - 2);
                        
                        //Se evalua la condición de bordes  o valores negativos
                        if (nx < 0 || ny < 0 || nx >= img.rows || ny >= img.cols)
                            continue;
                            
                        int index = (nx * img.cols + ny) * 3;
                        cont += *(original + index + chanel) * matriz[d][p];
                    }
                }
                //Se evalua la condición de que la variable sea 0
                if (cont < 0)
                    cont = 0;
                   
                  //Se evalua la condición de que la variable sea mayor que 255
                if (cont > 255)
                    cont = 255;
                    
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
    imwrite("perritoFiltro.png", img);
    int k = waitKey(0);
    free(original);
    free(solution);
    return 0;
		
}
int main()
{
int filtro( "p2.png"   ,1)
    
}
