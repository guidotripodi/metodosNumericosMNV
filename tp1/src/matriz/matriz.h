#ifndef __Matriz_H_INCLUDED__   //   #define this so the compiler knows it has been included
#define __Matriz_H_INCLUDED__   //   #define this so the compiler knows it has been included

class Matriz {
    private:
        int m;
        int n;
        double** matrix;
        bool posicionValida(int x, int y);
        Matriz * L;
        Matriz * U;

    public:
        Matriz(int a, int b);
        ~Matriz();
        int getM();
        int getN();
        double** getMatrix();
        void setVal(int x, int y, double val);
        double getVal(int x, int y);
        void printM();
        Matriz *copy();
        void gauss(Matriz *m,double *b);
        void restar_fila(Matriz *m, int fila_minuendo, int fila_sustraendo, double *b);
};
#endif
