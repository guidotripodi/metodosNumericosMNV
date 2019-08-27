#include "eliminaciongauss/elimgauss.h"
#include "matriz/matriz.h"
#include "instancia/instancia.h"
#include "wp/wp.h"

#include <math.h>       /* pow */
#include <algorithm>    // std::sort
#include <cmath>
#include <climits>
#include <vector>       // std::vector
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include <sys/time.h>

using namespace std;

string intToString(int pNumber);
instancia * generarInstanciaDesdeArchivo(ifstream &archivoDeEntrada,bool contarEmpates);
instancia * generarInstanciaVacia(ifstream &archivoDeEntrada);
void printVector(double * ,int );
bool pairCompare(const std::pair<int, double>& firstElem, const std::pair<int, double>& secondElem);

double getEloRaiting(double* raitings,int equipo1,int equipo2,int goles1,int goles2);

//El programa requiere 3 parametros, un archivo de entrada, uno de salida y el modo a ejecutar.
int main(int argc, char *argv[]) {
    timeval startGauss, endGauss;
    timeval startModificado, endModificado;
    timeval startWP, endWP;
    int totalEquipos;
    long elapsed_mtime; /* elapsed time in milliseconds */
    long elapsed_seconds; /* diff between seconds counter */
    long elapsed_useconds; /* diff between microseconds counter */

    int i;
    // argumentos
    // 0 - main
    // 1 archivo entrada
    // 2 archivo salida
    // 3 modo
    if (argc != 4) {
        cout << "Error, Faltan Argumentos" << endl;
        return 1;
    }

    // agregado para contar empates;
    int contarEmpates;
    if(argc ==4 ){
        contarEmpates  = 0;
    }else{
        contarEmpates  = atoi(argv[4]);
    }
    //leo archivo entrada
    ifstream archivoDeEntrada (argv[1]);

    //preparo archivo salida para escritura
    ofstream archivoDeSalida;
    ofstream archivoTiempos;
    archivoDeSalida.setf(ios::fixed, ios::floatfield); // tipo salida
    archivoTiempos.setf(ios::fixed, ios::floatfield); // tipo salida
    archivoTiempos.precision(15); // cant decimales
    archivoDeSalida.precision(15); // cant decimales
    archivoDeSalida.open(argv[2]);


    // genero una instancia Matriz de resultados Ganadores y vector de totales
    instancia *ins= generarInstanciaDesdeArchivo(archivoDeEntrada,contarEmpates);
    archivoDeEntrada.close();
    totalEquipos = ins->getTotalEquipos();
    // base para el resultado
    double* respuesta = new double[ins->getTotalEquipos()];
    for (i = 0; i < totalEquipos; ++i) {
        respuesta[i] = 0.0;
    }

    Matriz * CMM = ins->getCMM();
    // ins->print();
    string totales =  intToString(totalEquipos) + " " + intToString(ins->getTotalPartidos()) + " ";

    // metodo Metodo CMM Con Gauss
    if (strcmp(argv[3], "0") == 0) {
        cout << "Corriendo Metodo Gauss..." << endl;

        double timeGauss= 0.0;
        for (int iteraciones = 0; iteraciones<5; iteraciones++){
            gettimeofday(&startGauss, NULL);

            respuesta = gauss(CMM,ins->getVectorB());

            gettimeofday(&endGauss, NULL);

            elapsed_seconds = endGauss.tv_sec - startGauss.tv_sec;
            elapsed_useconds = endGauss.tv_usec - startGauss.tv_usec;

            timeGauss+= ((elapsed_seconds) * 1000 + elapsed_useconds / 1000.0) + 0.5;
        }
        timeGauss= timeGauss/5;

        archivoTiempos.open("tiempos/tiempos0.txt", std::ofstream::out | std::ofstream::app);
        archivoTiempos << totalEquipos << " "  << ins->getTotalPartidos() << " " <<timeGauss<< endl;
        archivoTiempos.close();
    }

    // metodo WP
    if (strcmp(argv[3], "1") == 0) {
        cout << "Corriendo Metodo WP..." << endl;

        gettimeofday(&startWP, NULL);
        respuesta = wp(ins);
        gettimeofday(&endWP, NULL);
        elapsed_seconds = endWP.tv_sec - startWP.tv_sec;
        elapsed_useconds = endWP.tv_usec - startWP.tv_usec;
        double timeWP =  ((elapsed_seconds) * 1000 + elapsed_useconds / 1000.0) + 0.5;
        archivoTiempos.open("tiempos/tiempos2.txt", std::ofstream::out | std::ofstream::app);
        archivoTiempos <<totalEquipos << " "  << ins->getTotalPartidos() << " " << timeWP<< endl;
        archivoTiempos.close();

    }

    // CALCULATE ELO
    if (strcmp(argv[3], "2") == 0) {
        respuesta = ins->getEloRaiting();
    }

    for (int w = 0; w < ins->getTotalEquipos(); w++) {
        archivoDeSalida << respuesta[w] << endl;
    }

    archivoDeSalida.close();
    return 0;
}

string intToString(int pNumber)
{
    ostringstream oOStrStream;
    oOStrStream << pNumber;
    return oOStrStream.str();
}

instancia *generarInstanciaDesdeArchivo(ifstream &archivoDeEntrada,bool contarEmpates){
    int n,k,i,fecha;
    int equipo1,equipo2,goles1,goles2;

    //leo cantidad de equipos
    archivoDeEntrada >> n;
    //leo cantidad de partidos
    archivoDeEntrada >> k;
    // creo la tabla de resultados ganadores
    Matriz * tablaResultados  =  new Matriz(n,n);
    // creo la tabla de partidos totales
    int* totales = new int[n];

    ///// ELO RAITING
    int cantEquipos = n;
    double* raitings = new double[cantEquipos];
    int K = 60;

    // raiting inicial 100 para todos los equipos
    for (int i = 0; i < cantEquipos; ++i) {
        raitings[i]=100.0;
    }
    //////////
    //
    for (i = 0; i < n; ++i) {
        totales[i]=0;
    }

    if (archivoDeEntrada.is_open())
    {
        for (i = 0; i < k; ++i) {
            //primer linea es fecha
            archivoDeEntrada >> fecha;
            // segunda linea es el numero del primer equipo
            archivoDeEntrada >> equipo1;
            // tercer linea es la cantidad de goles del primer equipo
            archivoDeEntrada >> goles1;
            // cuarta linea es el numero del segundo equipo
            archivoDeEntrada >> equipo2;
            // quinta linea es la cantidad de goles del segundo equipo
            archivoDeEntrada >> goles2;
            /// INIT Calculcate ELO RAIING ///
            double r1=getEloRaiting( raitings,equipo1,equipo2,goles1,goles2);
            double r2=getEloRaiting(raitings,equipo2,equipo1,goles2,goles1);

            raitings[equipo1]=r1;
            raitings[equipo2]=r2;
            /// END Calculate ELO RAIING ///
            if(goles1>goles2){
                totales[equipo1-1]++;
                totales[equipo2-1]++;
                int actual = tablaResultados->getVal(equipo1-1,equipo2-1);

                tablaResultados->setVal(equipo1-1,equipo2-1,actual+1);

            }else{
                if(goles2>goles1){
                    totales[equipo1-1]++;
                    totales[equipo2-1]++;
                    int actual = tablaResultados->getVal(equipo2-1,equipo1-1);

                    tablaResultados->setVal(equipo2-1,equipo1-1,actual+1);
                }else{
                    if(contarEmpates && goles1==goles2){
                        totales[equipo1-1]++;
                        totales[equipo2-1]++;
                        cout << "CONTE EMPATE" << endl;
                        cout << fecha << endl;
                        cout << goles1<< endl;
                        cout << goles2<< endl;
                        int actual = tablaResultados->getVal(equipo2-1,equipo1-1);

                        tablaResultados->setVal(equipo2-1,equipo1-1,actual+1);

                        actual = tablaResultados->getVal(equipo1-1,equipo2-1);

                        tablaResultados->setVal(equipo1-1,equipo2-1,actual+1);
                    }
                }
            }
        }
    }
    instancia *res =new instancia();
    res->setTotalPartidos(k);
    res->setEloRaiting(raitings);
    res->setGanados(tablaResultados);
    res->setTotales(totales);
    res->generarCMM();
    res->generarVectorB();
    return res;
}
double getEloRaiting(double* raitings,int equipo1,int equipo2,int goles1,int goles2){
    int K = 60;
    double ro1 = raitings[equipo1];
    double ro2 = raitings[equipo2];
    // if the game is a draw or is won by one goal
    // G = 1
    // If the game is won by two goals
    // G = 1,5
    // G = (11+N)/8
    int df1 = goles1-goles2;
    double G = 0;
    if (df1==0 || df1==1){
        G=1;
    } else if( df1==2){
        G = 1.5;
    }else if (df1>2){
        G  = (11+df1)/8;
    }

    // Result of match - Obtaining the W value
    double W=0;
    // W is the result of the game (1 for a win, 0.5 for a draw, and 0 for a loss).
    if(df1==0){
        W=0.5;
    }else if(df1>0){
        W=1;
    }

    // W_e = 1 / (10^(-dr/400) + 1)
    double dr = (ro1-ro2+100)*-1;
    double W_e = 1 / (pow(10,(dr/400)) + 1);
    return ro1+ K*G*(W-W_e);
}

void printVector(double * vec,int longitud){
    int i;
    for (i = 0; i < longitud; ++i) {
        cout << vec[i]<< endl;
    }

}

bool pairCompare(const std::pair<int, double>& firstElem, const std::pair<int, double>& secondElem) {
    return firstElem.second< secondElem.second;
};
