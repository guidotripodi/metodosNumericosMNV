#include "instancia.h"

instancia::~instancia(){
    delete ganados;
    delete totales;
}


void instancia::print(){
    int i, totalEquipos;

    totalEquipos=this->getTotalEquipos();

    cout <<"Matriz de Partidos enfrentamientos ganados" << endl;
    ganados->printM();
    cout <<"vector de totales" << endl;
    for (i = 0; i < totalEquipos; ++i) {
        cout <<totales[i] <<endl;
    }

    cout <<"Matriz CMM" << endl;
    CMM->printM();

    cout <<"vector b" << endl;
    for (i = 0; i < totalEquipos; ++i) {
        cout <<b[i] <<endl;
    }

}

void instancia::setTotalPartidos(int _totales){
    this->totalPartidos = _totales;
};

int instancia::getTotalPartidos(){
    return this->totalPartidos;
};

int instancia::getTotalEquipos(){
    return ganados->getN();
}

double instancia::getTotalGanados(int numeroDeEquipo){
    int totalEquipos = getTotalEquipos();
    int i;
    double resultado = 0.0 ;

    for (i = 0; i < totalEquipos; ++i) {
        resultado += ganados->getVal(numeroDeEquipo,i);
    }
    return resultado;
}

//Total partidos perdidos = total partidos jugados - partidos ganados
double instancia::getTotalPerdidos(int numeroDeEquipo){
    return this->getTotalJugados(numeroDeEquipo)-this->getTotalGanados(numeroDeEquipo);
}


// ESTA FUNCION SIRVE PARA ARMAR CMM
// n_i,j numero de enfrentamientos entre i y j= los ganados por i + los ganados por j
double instancia::getTotalJugadosEntreEquipos(int numeroDeEquipo1,int numeroDeEquipo2){
    return this->ganados->getVal(numeroDeEquipo1,numeroDeEquipo2)+this->ganados->getVal(numeroDeEquipo2,numeroDeEquipo1);
}

double instancia::getTotalJugados(int numeroDeEquipo){
    return this->totales[numeroDeEquipo];
}

// Cij = −nij si i!=j,
// Cij = 2 + ni si i = j.
void instancia::generarCMM(){
    int i,j;
    double nij;
    int totalEquipos = this->getTotalEquipos();
    CMM = new Matriz(totalEquipos,totalEquipos);

    for (i = 0; i < totalEquipos; ++i) {
        for (j = 0; j < totalEquipos; ++j) {
            if(i==j){
                nij =this->getTotalJugados(i)+2.0;
            }else{
                nij =(-1.0)* this->getTotalJugadosEntreEquipos(i,j);
            }
            CMM->setVal(i,j,nij);
        }
    }
}
// setters
void instancia::setTotales(int* _totales){
    totales=_totales;
};

void instancia::setEloRaiting(double* _raitings){
    raitings=_raitings;
};

void instancia::setGanados(Matriz* _ganados){
    ganados = _ganados;
};

// getters
double* instancia::getEloRaiting(){
    return raitings;
};
int* instancia::getTotales(){
    return totales;
};

Matriz *instancia::getGanados(){
    return ganados;
};

Matriz *instancia::getCMM(){
    int i, j;

    Matriz * newCMM = new Matriz(this->getTotalEquipos(),this->getTotalEquipos());

    for (i = 0; i < this->getTotalEquipos(); i++) {
        for (j = 0; j < this->getTotalEquipos(); j++) {
            newCMM->setVal(i,j,this->CMM->getVal(i,j));
        }
    }

    return CMM;
};


// esta funcion genera el vector B pedido por CMM
void instancia::generarVectorB(){
    b = new double[ganados->getN()];
    int i;

    for (i = 0; i < ganados->getN(); ++i) {
        b[i]= 1.0+((double)this->getTotalGanados(i)-(double)this->getTotalPerdidos(i))/2.0;
    }

}


int instancia::getEquipoPerdido(int equipo1){
    int totalEquipos = this->getTotalEquipos();
    int equipo = -1;
    for(int i =0;i< totalEquipos;i++){
        if(i!= equipo1 && this->ganados->getVal(i,equipo1)>0){
            equipo = i;
            break;
        }
    }

    return equipo;
};

double* instancia::getVectorB(){
    return b;
}
