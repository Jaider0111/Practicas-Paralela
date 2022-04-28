## Practica 1 - Filtrado de imagenes

Esta practica consiste en una aplicación que permite filtrar una imagen usando un filtro de convolución.
Los filtros disponibles son:

- Labrado - 1
- Enfoque - 2
- Realce de Bordes - 3
- Desenfoque - 4
- Deteccion de bordes - 5
- Sobel - 6
- Sharpen - 7
- Norte - 8
- Este - 9
- Gaussiano - 10
- Laplance - 11
- Gradiente - 12

Para usar la aplicacion se debe hacer desde linea de comando. Primero se debe compilar\* el codigo fuente, para esto se debe hacer desde la linea de comando usando el siguiente comando:

```
g++ image-filter.cpp -o image-filter -lpthread `pkg-config --cflags --libs opencv4`
```

\*Nota: Para compilar el codigo fuente es necesario tene instalado el paquete `pkg-config` ademas de las librerias de opencv.

Luego se ejecuta la aplicación usando el siguiente formato:

```
./image-filter <path_imagen_entrada> <path_imagen_salida> <numero_filtro> <numero_hilos>
```

De lo anterior se debe tener en cuenta que el numero de hilos debe ser un numero entero mayor que 0, ademas,
el numero de filtro debe ser un numero entero entre 1 y 12 correspondiente a un filtro de convolución de la
lista de filtros disponibles.

Si se desea ejecutar los ejemplos se tiene disponible el archivo `run_examples.sh` que contiene los comandos necesarios para ejecutar los ejemplos, este script recibe como parametro el numero de hilos a utilizar y el archivo donde se almacenan los tiempos de ejecucion. Para ejecutar el script se debe hacer desde la linea de comando usando el siguiente comando:

```
./run_examples.sh <numero_hilos> <archivo_salida>
```

\*Nota: Es importante que para ejecutar cualquiera de los comandos disponibles en este documento se debe estar
ubicado en la carpeta que contiene los archivos de este documento.
