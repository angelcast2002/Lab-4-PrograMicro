/*---------------------------------------
UNIVERSIDAD DEL VALLE DE GUATEMALA
CC3056 - Programacion de Microprocesadores
Realizado por: Angel Castellanos

LAB3 - Parte 2
Pide una cantidad de threads y una rango de numeros para poder sumar todos los numeros primos dentro del rango.
---------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;
float SumaTotal = 0.0;

pthread_mutex_t candado;

void *Sumatoria(void *argument ) {

	float *input = (float *)argument;

    float numeroSumatoria = *input;
    
    float resultado = 0.0;

    
    resultado = (numeroSumatoria) * (numeroSumatoria + 1);

    pthread_mutex_lock(&candado);
    SumaTotal += (1/resultado);
    cout << "Cuando n = " << numeroSumatoria << " el subtotal es de  --- " << SumaTotal << " ---" << endl;

    pthread_mutex_unlock(&candado);

    //pthread_exit(NULL);
    //pthread_exit((void *) IntResultado);
    return NULL;
}

int main() {

    int n = 0;

    
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    cout << "Ingrese el numero n maximo: " << endl;
    cin >> n;

    pthread_t idThread[n];
    

    for(int i = 0; i <= n; i++){

        float NumeroATrabajar = (float) i;
        pthread_create(&idThread[i], &attr, Sumatoria, ( void *)&NumeroATrabajar);
        //pthread_join(idThread, NULL);
        
    }

    for(int i = 0; i <= n; i++){

        
        pthread_join(idThread[i], NULL);
        
    }

    cout << "La sumatoria de n = 1 hasta n = " << n << " es de: "  << SumaTotal << endl;

    printf ("\n --- Fin --- \n");
    pthread_mutex_destroy(&candado);
    pthread_attr_destroy(&attr);
	pthread_exit(NULL);
    return 0;
}