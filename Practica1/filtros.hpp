#pragma once

// Listado de matrices de filtrado
const int filtros[12][5][5] = {
    // Labrado - 0
    {
        {0, 0, 0, 0, 0},
        {0, -2, -1, 0, 0},
        {0, -1, 1, 1, 0},
        {0, 0, 1, 2, 0},
        {0, 0, 0, 0, 0},
    },
    // Enfoque - 1
    {
        {0, 0, 0, 0, 0},
        {0, 0, -1, 0, 0},
        {0, -1, 5, -1, 0},
        {0, 0, -1, 0, 0},
        {0, 0, 0, 0, 0},
    },
    // Realce de Bordes - 2
    {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, -1, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    },
    // Desenfoque - 3
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
    },
    // Deteccion de bordes - 4
    {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, -4, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
    },
    // Sobel - 5
    {
        {0, 0, 0, 0, 0},
        {0, -1, 0, 1, 0},
        {0, -2, 0, 2, 0},
        {0, -1, 0, 1, 0},
        {0, 0, 0, 0, 0},
    },
    // Sharpen - 6
    {
        {0, 0, 0, 0, 0},
        {0, 1, -2, 1, 0},
        {0, -2, 5, -2, 0},
        {0, 1, -2, 1, 0},
        {0, 0, 0, 0, 0},
    },
    // Norte - 7
    {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, -2, 1, 0},
        {0, -1, -1, -1, 0},
        {0, 0, 0, 0, 0},
    },
    // Este - 8
    {
        {0, 0, 0, 0, 0},
        {0, -1, 1, 1, 0},
        {0, -1, -2, 1, 0},
        {0, -1, 1, 1, 0},
        {0, 0, 0, 0, 0},
    },
    // Gaussiano - 9
    {
        {1, 2, 3, 1, 1},
        {2, 7, 11, 7, 2},
        {3, 11, 17, 11, 3},
        {2, 7, 11, 7, 1},
        {1, 2, 3, 2, 1},
    },
    // Laplance - 10
    {
        {0, 0, 0, 0, 0},
        {0, -1, -1, -1, 0},
        {0, -1, 8, -1, 0},
        {0, -1, -1, -1, 0},
        {0, 0, 0, 0, 0},
    },
    // Gradiente - 11
    {
        {0, 0, 0, 0, 0},
        {0, 0, -1, -1, 0},
        {0, 1, 0, -1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0},
    },
};