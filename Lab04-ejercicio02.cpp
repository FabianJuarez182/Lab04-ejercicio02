/*
*------------------------------------------
* Laboratorio 4 parte02.cpp
* -----------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
* CC3086 - Programacion de Microprocesadores
* Fabian Juárez 21440
*------------------------------------------
* Descripción: 
* Opera una fabrica que se dedica a la produccion de cafe, crear 2 tostadoras por hilos paralelos
*ambas tostadoras trabajaran a 1 lb por segundo y tendremos un silo donde se ingresaran, al
*tener 5 se mandara a la empacadora y esta procesara 1 lb por segundo posterior a tener 5 lb en el silo.
* La empacadora la enviara a la bodega y cuando termine debera de tener 400 en bodega y 0 en el silo son tener negativos
-----------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;
int trabajador;
int totalPiezas;
int cantProd = 0;
int Silo = 0;
int Bodega = 0;
int order = 800;
pthread_mutex_t mutex;
struct Fworkers
{
    pthread_t thread;
    string nombre;
    int mpp;
    int order;
    int pieces;
};
 //workers[3];
void* production_order(void * args){
    Fworkers * ws;
    ws = (Fworkers *)args;
    while (ws->order != ws->pieces)
        {
            if(cantProd <= order-3){
                sleep(1);
                ws->pieces;
                pthread_mutex_lock(&mutex);
                Silo = Silo + ws->pieces;
                order = order - ws->pieces;
                cantProd = cantProd + ws->pieces;
                cout << ws->nombre << " produjo: " << ws->pieces <<" lb de café tostada"<< endl;
                pthread_mutex_unlock(&mutex);
            }
            else{
                cout << ws->nombre << " finished the order" <<endl;
                pthread_exit(NULL);
            }
        }
}

void* production_order2(void * args){
    Fworkers * ws;
    ws = (Fworkers *)args;
    while (ws->order != ws->pieces)
    {
        if(Silo>=5 || cantProd > 5 ){    
            if(Silo > 0){
                ws->pieces;
                pthread_mutex_lock(&mutex);
                Silo = Silo - ws->pieces;
                cout<< "Libras de café en Silo: " <<Silo << endl; 
                cout << ws->nombre << " produjo: " << ws->pieces <<" bolsa de 1 lb de café"<< endl;
                Bodega = Bodega + ws->pieces;
                cout << "Libras de café en bodega: "<< Bodega<<endl;
                pthread_mutex_unlock(&mutex);
                sleep(1);
            }
            else{
                printf("\n\n");
                cout << "Libras de café en Silo: " <<Silo << endl;
                cout << "Libras de café en bodega: "<< Bodega<<endl;
                pthread_exit(NULL);
            }
        }
    } 
}
int main()
{
    trabajador = 3;
    Fworkers workers[trabajador];
    for(int i=0; i<3; i++){
        //Para vaciar buffer y permitir digitar valores
        fflush(stdin);
        if(i==0){
            workers[i].nombre="Tostadora 1" ; 
        }
        if(i==1){
            workers[i].nombre="Tostadora 2";
        }
        if(i==2){
            workers[i].nombre="Empacadora";
        }
        workers[i].mpp = 1;
        cout << "\n";
        workers[i].pieces = 1;
    }
    fflush(stdin);
    cout << "\n";
    for (int j = 0; j < 3; j++){
        if(j<2){
            workers[j].order = 0;
        }
        else{
            workers[j].order = 0;
        }
    }
    for (int i = 0; i < 3; i++){
        cout << "\nNombre: " << workers[i].nombre << endl;
        cout << "libras por segundo: " << workers[i].mpp << "\n"<< endl;
        }
    for (int k = 0; k < 3; k++){
        if(k<2){
            pthread_create(&workers[k].thread, NULL, production_order, ( void *)&workers[k]);
        }
        else{
            pthread_create(&workers[k].thread, NULL, production_order2, ( void *)&workers[k]);
        }
    }
    for(int l = 0; l < 3; l++){
        pthread_join(workers[l].thread, NULL);
    }
    pthread_exit(NULL);
    return 0;
}