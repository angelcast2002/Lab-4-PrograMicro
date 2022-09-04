/*---------------------------------------
UNIVERSIDAD DEL VALLE DE GUATEMALA
CC3056 - Programacion de Microprocesadores
Realizado por: Angel Castellanos

LAB4 - Parte 2
Calculo de utilidad de la tienda &Café en UVG. Uso de variables MUTEX
---------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include<math.h>
using namespace std;

pthread_mutex_t candado;

//Constantes

//Hilos a trabajar
const int numeroHilos = 16;

//Precio por unidad (venta al publico)
const float pastelChocolate = 60;
const float whiteMocha = 32;
const float cafeAmericano = 22;
const float latte  = 24;
const float toffeCoffe = 28;
const float cappuccino = 24;
const float smoresLatte = 32;
const float cafeTostadoMolido = 60;

//Gastos varios por mes
const float costosVariablesJulio = 45640.00;
const float costosVariablesAgosto = 40590.00;

//Costos fijos por unidad (costo de produccion)
const float Unidad_pastelChocolate = 20.00;
const float Unidad_whiteMocha = 19.20;
const float Unidad_cafeAmericano = 13.20;
const float Unidad_latte = 17.20;
const float Unidad_toffeCoffe = 20.10;
const float Unidad_cappuccino = 17.20;
const float Unidad_smoresLatte = 23.00;
const float Unidad_cafeTostadoMolido = 20.00;

//Ventas por mes
//Julio
const float VentasJulio_pastelChocolate = 300;
const float VentasJulio_whiteMocha = 400;
const float VentasJulio_cafeAmericano = 1590;
const float VentasJulio_latte = 200;
const float VentasJulio_toffeCoffe = 390;
const float VentasJulio_cappuccino = 1455;
const float VentasJulio_smoresLatte = 800;
const float VentasJulio_cafeTostadoMolido = 60;
//Agosto
const float VentasAgosto_pastelChocolate = 250;
const float VentasAgosto_whiteMocha = 380;
const float VentasAgosto_cafeAmericano = 800;
const float VentasAgosto_latte = 250;
const float VentasAgosto_toffeCoffe = 600;
const float VentasAgosto_cappuccino = 1200;
const float VentasAgosto_smoresLatte = 1540;
const float VentasAgosto_cafeTostadoMolido = 15;


//Arrays con los valores a trabajar
//Nombres
string nombreProductos[8] = {"Pastel de Chocolate", "White Mocha", "Cafe Americano", "Latte", "Toffe Coffe", "Cappuccino", 
"S'mores Latte", "Cafe Tostado Molido"};
//Precios
float PrecioUnitario[16] = {pastelChocolate, whiteMocha, cafeAmericano, latte, toffeCoffe, cappuccino, smoresLatte, cafeTostadoMolido
,pastelChocolate, whiteMocha, cafeAmericano, latte, toffeCoffe, cappuccino, smoresLatte, cafeTostadoMolido};
float CostoUnitario[16] = {Unidad_pastelChocolate, Unidad_whiteMocha, Unidad_cafeAmericano, Unidad_latte, Unidad_toffeCoffe, Unidad_cappuccino, Unidad_smoresLatte, Unidad_cafeTostadoMolido
, Unidad_pastelChocolate, Unidad_whiteMocha, Unidad_cafeAmericano, Unidad_latte, Unidad_toffeCoffe, Unidad_cappuccino, Unidad_smoresLatte, Unidad_cafeTostadoMolido};
//Cantidad de ventas
float Ventas[32] = {VentasJulio_pastelChocolate, VentasJulio_whiteMocha, VentasJulio_cafeAmericano, VentasJulio_latte, VentasJulio_toffeCoffe, VentasJulio_cappuccino, VentasJulio_smoresLatte, VentasJulio_cafeTostadoMolido
, VentasAgosto_pastelChocolate, VentasAgosto_whiteMocha, VentasAgosto_cafeAmericano, VentasAgosto_latte, VentasAgosto_toffeCoffe, VentasAgosto_cappuccino, VentasAgosto_smoresLatte, VentasAgosto_cafeTostadoMolido};

//Resultados
//Arrays con los resultados
float MontoVentas[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float UtilidaVenta[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
//Ventas para cada mes
float VentasJulio = 0.0;
float VentasAgosto = 0.0;
//Utilidad por mes
float UtilidadJulio = 0.0;
float UtilidadAgosto = 0.0;
//Reportes
string reporteJulio = "--- REPORTE DEL MES DE JULIO ---\n";
string reporteAgosto = "--- REPORTE DEL MES DE AGOSTO ---\n";


void *CalculoHilo(void *argument) {

	int *input = (int *)argument;
    int idHilo = *input;

    cout << "Calculo del hilo " << idHilo << " iniciado" << endl;

    //Bloqueo MUTEX
    pthread_mutex_lock(&candado);

    //Calculo de monto de ventas por producto
    MontoVentas[idHilo] = PrecioUnitario[idHilo] * Ventas[idHilo];

    //Calculo de utilidad por producto
    UtilidaVenta[idHilo] = MontoVentas[idHilo] - (CostoUnitario[idHilo] * Ventas[idHilo]);
    
    //Desbloqueo MUTEX
    pthread_mutex_unlock(&candado);

    cout << "Calculo del hilo " << idHilo << " terminado" << endl;

    return NULL;
}

void *CalculoPerMes(void *argument) {

	int *input = (int *)argument;
    int idHilo = *input;
    float TotalVentasMes = 0.0;
    float TotalUtilidadMes = 0.0;

    //Trabajar julio
    if (idHilo == 1)
    {
        //Bloqueo MUTEX
        pthread_mutex_lock(&candado);
        for (int i = 0; i < 8; i++)
        {
            
            TotalVentasMes += MontoVentas[i];
            TotalUtilidadMes += UtilidaVenta[i];

        }
        
        VentasJulio += TotalVentasMes;
        UtilidadJulio = TotalUtilidadMes - costosVariablesJulio;
        //Desbloqueo MUTEX
        pthread_mutex_unlock(&candado);

    }
    //Trabajar agosto
    if (idHilo == 2)
    {
        //Bloqueo MUTEX
        pthread_mutex_lock(&candado);
        for (int i = 8; i < 16; i++)
        {
            
            TotalVentasMes += MontoVentas[i];
            TotalUtilidadMes += UtilidaVenta[i];

        }
        
        VentasAgosto += TotalVentasMes;
        UtilidadAgosto = TotalUtilidadMes - costosVariablesAgosto;
        //Desbloqueo MUTEX
        pthread_mutex_unlock(&candado);
    }

    return NULL;
}

void *Reportes(void *argument) {

	int *input = (int *)argument;
    int idHilo = *input;
    string reporte = "";
    //Trabajar julio
    if (idHilo == 1)
    {
        reporte = "- Ventas por producto - \n";
        for (int i = 0; i < 8; i++)
        {
            reporte += nombreProductos[i] + ": Q" + to_string((int)round(MontoVentas[i])) + " \n";
            
        }

        reporte += "\n- Utilidad por producto - \n";
        for (int i = 0; i < 8; i++)
        {
            reporte += nombreProductos[i] + ": Q" + to_string((int)round(UtilidaVenta[i])) + " \n";
            
        }
        
        reporte += "\nTotal ventas: Q" + to_string((int)VentasJulio) + " \n";
        reporte += "Costos variables: Q" + to_string((int)costosVariablesJulio) + " \n";
        reporte += "Utilidad del mes: Q" + to_string((int)UtilidadJulio) + " \n";

        reporteJulio += reporte;
    }
    //Trabajar agosto
    if (idHilo == 2)
    {
        int contador = 8;
        reporte = "- Ventas por producto - \n";
        for (int i = 0; i < 8; i++)
        {   
            contador = i + 8;
            reporte += nombreProductos[i] + ": Q" + to_string((int)round(MontoVentas[contador])) + " \n";

        }

        reporte += "\n- Utilidad por producto - \n";
        for (int i = 0; i < 8; i++)
        {
            contador = i + 8;
            reporte += nombreProductos[i] + ": Q" + to_string((int)round(UtilidaVenta[contador])) + " \n";

        }
        
        reporte += "\nTotal ventas: Q" + to_string((int)VentasAgosto) + " \n";
        reporte += "Costos variables: Q" + to_string((int)costosVariablesAgosto) + " \n";
        reporte += "Utilidad del mes: Q" + to_string((int)UtilidadAgosto) + " \n";

        reporteAgosto += reporte;
    }
    
    return NULL;
}

int main(void) {
    
    pthread_attr_t attr;
    pthread_t idThread[numeroHilos];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&candado, NULL);

    cout << "---Inicializando Hilos---" << endl;

    for(int i = 0; i < numeroHilos; i++){
        
        cout << "Hilo " << i << " creado" << endl;  //Esta linea le da tiempo suficiente al hilo para que se cree
        int id = i;
        pthread_create(&(idThread[i]), &attr, &CalculoHilo, ( void *)&id);
        //pthread_join(idThread[i], NULL);
        
    }

    for(int i = 0; i < numeroHilos; i++){

        int id = i ;
        pthread_join(idThread[id], NULL);
        
    }

    cout << endl;

    //Calculo de ventas por mes

    for(int i = 1; i < 3; i++){
        
        int id = i;
        pthread_create(&(idThread[i]), &attr, &CalculoPerMes, ( void *)&id);
        sleep(1);
        
    }
    

    for(int i = 1; i < 3; i++){

        int id = i ;
        pthread_join(idThread[id], NULL);
        
    }

    //Reportes
    for(int i = 1; i < 3; i++){
        
        int id = i;
        pthread_create(&(idThread[i]), &attr, &Reportes, ( void *)&id);
        sleep(1);
        
    }
    

    for(int i = 1; i < 3; i++){

        int id = i ;
        pthread_join(idThread[id], NULL);
        
    }

    cout << reporteJulio << endl;
    cout << reporteAgosto << endl;
    
    
    

    printf ("\n --- Fin --- \n");
    pthread_mutex_destroy(&candado);
    pthread_attr_destroy(&attr);
	pthread_exit(NULL);
    return 0;
}
