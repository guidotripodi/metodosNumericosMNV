#ifndef __instancia_H_INCLUDED__   //   #define this so the compiler knows it has been included
#define __instancia_H_INCLUDED__   //   #define this so the compiler knows it has been included

#include "../matriz/matriz.h"
#include <iostream>
using namespace std;

class instancia{
    private:
        // esta matriz tiene en cada posicion cuantos partidos gano el equipo i contar el j
        // para obtener los que perdio ir a la posicion j , i
        Matriz * ganados;

        // esta es la matriz CMM
        Matriz * CMM;

        // este tiene para cada equipo los partidos totales
        int * totales ;
        int  totalPartidos;

        double * b;

    public:
        ~instancia();
        void print();
        // estas funciones son para la mariz ganados
        // esta funcion te da el total de equipos en este caso es
        // igual la cant de columnas de ganados
        int getTotalEquipos();

        int getTotalPartidos();
        // el primero siempre es el que gana el partido
        bool ganaPartido(int);
        void modificarPartido(int,int);
        void ganaPartidoContra(int,int);
        void setTotalPartidos(int);
        int getEquipoPerdido(int);
        // esta funcion te dice cuantos partidos GANADOS tiene el equipo q recibe por parametro
        double getTotalGanados(int);

        // esta funcion te dice cuantos partidos PERDIDOS tiene el equipo q recibe por parametro
        double getTotalPerdidos(int);

        // esta funcion te da la cantidad total de partidos jugaods para el equipo que recibe por parametro
        double getTotalJugados(int);

        // esta funcion te da el total de encuentros GANADOS o PERDIDOS entre dos equipos
        double getTotalJugadosEntreEquipos(int,int);

        // setters para privados
        void setGanados(Matriz*);
        void setTotales(int*);

        // getters para privados
        Matriz* getGanados();
        int* getTotales();
        Matriz* getCMM();

        // esta funciion genera la matriz CMM
        void generarCMM();
        void generarVectorB();
        double* getVectorB();
};

#endif
