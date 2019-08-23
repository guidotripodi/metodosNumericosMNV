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

    cout <<"Matriz Massey" << endl;
    Mass->printM();

    cout <<"vector y massey" << endl;
    for (i = 0; i < totalEquipos; ++i) {
        cout <<diferencias[i] <<endl;
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
    double nij,mij;
    int totalEquipos = this->getTotalEquipos();
    CMM = new Matriz(totalEquipos,totalEquipos);
    Mass = new Matriz(totalEquipos,totalEquipos);

    for (i = 0; i < totalEquipos; ++i) {
        for (j = 0; j < totalEquipos; ++j) {
            if(i==j){
                nij =this->getTotalJugados(i)+2.0;
                mij =this->getTotalJugados(i);
            }else{
                nij =(-1.0)* this->getTotalJugadosEntreEquipos(i,j);
                mij =(-1.0)* this->getTotalJugadosEntreEquipos(i,j);
            }
            CMM->setVal(i,j,nij);
            Mass->setVal(i,j,mij);
        }
    }
}

// setters
void instancia::setTotales(int* _totales){
    totales=_totales;
};
void instancia::setDiferencias(double* _diferencias){
    diferencias=_diferencias;
};
void instancia::setGanados(Matriz* _ganados){
    ganados = _ganados;
};

// getters
int* instancia::getTotales(){
    return totales;
};
double* instancia::getDiferencias(){
    return diferencias;
};

Matriz *instancia::getGanados(){
    return ganados;
};

Matriz *instancia::getMass(){
    return Mass;
}

Matriz *instancia::getCMM(){
    // int i, j;

    // Matriz * newCMM = new Matriz(this->getTotalEquipos(),this->getTotalEquipos());

    // for (i = 0; i < this->getTotalEquipos(); i++) {
    // for (j = 0; j < this->getTotalEquipos(); j++) {
    // newCMM->setVal(i,j,this->CMM->getVal(i,j));
    // }
    // }

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
// esta funcion lo que hace es darle un partido mas ganado al equipo1
void instancia::modificarPartido(int equipo1,int equipo2){
    // modifico el partido perdido en el otro
    int totalganadose2e1 = this->ganados->getVal(equipo2,equipo1);
    this->ganados->setVal(equipo2,equipo1,totalganadose2e1-1);

    // en la matriz de partidos ganados le sumo uno al primero
    int totalganadose1e2 = this->ganados->getVal(equipo1,equipo2);
    this->ganados->setVal(equipo1,equipo2,totalganadose1e2+1);

    // genero la nueva CMM
    this->generarCMM();
    // genero el nuevo vector B
    this->generarVectorB();
};

// esta funcion lo que hace es darle un partido mas ganado al equipo1
void instancia::ganaPartidoContra(int equipo1,int equipo2){
    int totalJugados1 = this->getTotalJugados(equipo1);
    int totalJugados2 = this->getTotalJugados(equipo2);
    // le sumo un partido mas en partidos totales
    this->totales[equipo1]=totalJugados1+1;
    this->totales[equipo2]=totalJugados2+1;
    // en la matriz de partidos ganados le sumo uno al primero
    int totalganadose1e2 = this->ganados->getVal(equipo1,equipo2);
    this->ganados->setVal(equipo1,equipo2,totalganadose1e2+1);
    // genero la nueva CMM
    this->generarCMM();
    // genero el nuevo vector B
    this->generarVectorB();
};

// Esta funcion lo que hace es cambiar un partido ganado por perdido y se lo suma al contrincante,
// el partido seleccionado es uno que se jugo y fue ganado
bool instancia::ganaPartido(int equipo1){
    bool encontroUno = false;
    int i;
    for (i = 0; i < this->getTotalEquipos(); i++) {
        if(this->ganados->getVal(equipo1,i)!=0 && i!=equipo1){
            // agarro el valor viejo
            int valor = this->ganados->getVal(equipo1,i);
            // le cambio el resultado al partido
            this->ganados->setVal(equipo1,i,valor-1);
            // le hago ganar al otro equipo
            int valor2 = this->ganados->getVal(i,equipo1);
            this->ganados->setVal(i,equipo1,valor2+1);
            encontroUno = true;
        }
    }
    return encontroUno;
}

double* instancia::getVectorB(){
    return b;
}
