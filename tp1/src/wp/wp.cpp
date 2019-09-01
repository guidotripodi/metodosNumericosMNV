#include "wp.h"
using namespace std;

double* wp(instancia* ins){
    int i = 0;
    int cantEquipos = ins->getTotalEquipos();
    double* res = new double[cantEquipos];
    //limpio array
    //
    for (i = 0; i < cantEquipos; ++i) {
        res [i]=0.0;
    }

    for (i = 0; i < cantEquipos; ++i) {
        res[i] = double(ins->getTotalGanados(i)+1.)/double(ins->getTotalJugados(i)+2.);
    }


    return res;
};
