#include "matriz.h"

#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

/**
 * Constructor de Matriz banda
 *
 * @param {int} dimension
 * @param {int} longitud de banda
 *
 * @return {Matriz} nueva instancia de Matriz
 */

Matriz::Matriz(int _n,int _m) {
    m = _m;
    n = _n;
    // Seteo el tamano del vector que representa la matriz
    // con tamano banda*2 + 1
    matrix = new double*[n*m];
    //inicializo la matriz de manera segura (evitando problemas de manejo de memoria)
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[n];
        for (int j = 0; j < m; j++) {
            matrix[i][j] = 0.0;
        }
    }
}

Matriz::~Matriz() {
    for (int i = 0; i <n ; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

/**
 * matrix getter
 */

double** Matriz::getMatrix() {
    return matrix;
}

/**
 * n getter
 */

int Matriz::getN() {
    return n;
}

/**
 * m getter
 */

int Matriz::getM() {
    return m;
}

/**
 * Obtener el valor de la matriz en un punto
 *
 * @param {int} x
 * @param {int} y
 *
 * @return {double} m(x, y)
 *
 * x1   x2  0   0   0
 * x3   x4  x5  0   0
 * 0    x6  x7  x8  0
 * 0    0   x9  x10 x11
 *
 * Para ahorrar los 0s lo transformo en =>
 * x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11
 */

double Matriz::getVal(int x, int y) {
    if (posicionValida(x, y)) {
        return matrix[x][y];
    } else {
        return 0.0;
    }
}

/**
 * Setear valor en un punto
 *
 * @param {int} x
 * @param {int} y
 * @param {double} valor a setear
 */

    void Matriz::setVal(int x, int y, double val) {
        if (posicionValida(x, y)) {
            matrix[x][y] = val;
        } else {
            // Si x, y están fuera de rango tira una excepcion
            cout << "Intento de seteo en cordenadas: (" << x << "," << y << ")" << endl;
            throw 0;
        }
    }

/**
 * Devuelve true si x e y forman una posicion valida en la matriz
 *
 * @param {int} x
 * @param {int} y
 *
 * @return {boo} true si es valida, false si no
 *
 * x1   x2  0   0   0   0
 * 0    x5  x6  0   0   0
 * 0    0   x9  x10 0   0
 */

bool Matriz::posicionValida(int x, int y) {
    // Si no cumple ninguna de las condiciones previas, es verdadero
    return x<this->getN() && y<this->getM();
}

/**
 * Imprime la matriz de un modo conveniente
 */

void Matriz::printM() {
    int l = 0;
    int i, j;
    int n = this->getN();
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (this->getVal(i, j) == 0) {
                cout <<  this->getVal(i, j);
            } else {
                cout <<  this->getVal(i, j);
                l++;
            }
            cout << "\t";
        }
        cout << endl;
    }
}


Matriz *Matriz::copy() {
    Matriz* aux = new Matriz(n,m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            aux->matrix[i][j] = matrix[i][j];
        }
    }
    return aux;
}

