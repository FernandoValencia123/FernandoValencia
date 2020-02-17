/*
 * **********************************************************************************
 * ****************         UNIVERSIDAD TÉCNICA DEL NORTE               *************
 * ****************                 FICA-CIERCOM                        ************* 
 * ****************               SISTEMAS EMBEBIDOS                    *************
 * ****************   DATOS OBTENIDOS DESDE PCB-CALIDAD DEL AIRE        *************
 * **********************************************************************************                 
 * 
 * Para la evaluacion de datos de entrada se empleo una bse de datos de prueba en la que constan:
 * 1= Loma de guayabillas
 * 2= Centro de Ibarra
 * 3= Casa de Fernando Valencia 
 * 
 * Cada etiqueta contiene datos de %Humedad/*C Temperatura/CO pm10/ CO2 pm2.5/ uv concentracion 
 * LA EVALUACION DE DATOS DE ENTRADA SE REALIZO CON BAYESIANO DEBIDO A LA EFICIENCIA DE 98.02% DE PREDICCION 
 * QUE MUESRA DE ACUERCO CON EL SOFTWARE R
 */


/************************************************************************************/
#include "datosPrueba.h" //Incluir matriz de entrenamiento de 126 datos
int i,b,k,l,m; //contadores
float p0 = 0.8 ,p1 = 0.4,p2 = 0.4, p3=0.4,p4=0.2; //Pesos p0, p1, p2,p3, p4
float Mcu[126]; // Función de Entradas y Pesos
int iteracion = 0; //Contador Iteraciones
float pred[126]; //Vector Predición
float efficiency[126], cerouno[126]; //Matriz eficiancia y 0-1
int errTotal = 0; //Contador Error
int err = 0; //Puntero para error
float errores[126]; //Vector almacenar errores
float n = 0.3; // Aprendizaje del algoritmo


void setup() {

  Serial.begin(9600); //Inicio Comunicación Serial
}


void loop() {

  while (iteracion < 5) { //Validación Iteraciones
    errTotal = 0; //Contador
    //Ecuación Fwx = Xo*po + X1*p1 + X2*p2..
    ecuacion();
    //Método para predicción
    prediction();
    //Cuenta error Out-Pred
    out_pred();
    //método para buscar 1 error
    search_one();
    //Calculo de pesos
    pesos();
    //conteo Errores
    error_count();
    //Salida Datos
    data_out();
  }
}

void ecuacion(){

for (i = 0; i < 126; i++) {
      Mcu[i] = (matriz[i][0] * p0) + (matriz[i][1] * p1) + (matriz[i][2] * p2 + (matriz[i][3] * p3) + (matriz[i][4] * p4)); //Calculo Ecuación
      if (Mcu[i]>matriz[i]){
      cerouno[i]=1;
    } else {
      cerouno[i]=1;
    }
    ////////Eficiencia Sistema
    if (pred[i]==cerouno[i]){
      efficiency[i]=1;
    } else {
      efficiency[i]=0;
    }
       
  
}
}

void prediction(){
    for (b = 0; b < 126; b++) {
      if (Mcu[b] >= 0) { // validación de predición
        pred[b] = 1;
      } else {
        pred[b] = 0;
      }
    }
}

void out_pred(){
for (k = 0; k < 63; k++) { 
      errores[k] = 1 - pred[k]; //Etiqueta 1
      errores[k + 126] = 0 - pred[k + 126]; //Etiqueta 0
    }
}

void search_one(){
for (l = 0; l < 126; l++) { //método para buscar 1 error
      if (errores[l] == -1 || errores[l] == 1) { //Diferente de 0
        err = l; //Asignar valor
        break; //Obligar a terminar método
      }
    }
}

void error_count(){
    for (m = 0; m < 126; m++) { //Método para conteo Errores
      if (errores[m] != 0) { //Validación Errores
        errTotal = errTotal + 1; //Conteo errores
      }
    }
}

void pesos (){
    p0 = p0 + (n * (errores[err]) * 1);  //Peso 1
    p1 = p1 + (n * (errores[err]) * (matriz[err][1])); //Peso 2
    p2 = p2 + (n * (errores[err]) * (matriz[err][2])); //Peso 3
    p3 = p3 + (n * (errores[err]) * (matriz[err][3])); //Peso 4
    p4 = p4 + (n * (errores[err]) * (matriz[err][4])); //Peso 5
    iteracion = iteracion + 1; //Conteo Iteraciones
}

void data_out(){
    Serial.print("Iter: ");
    Serial.println(iteracion);
    Serial.print("Pesos: ");
    Serial.print(p0);
    Serial.print(" ");
    Serial.print(p1);
    Serial.print(" ");
    Serial.println(p2);
    Serial.print(" ");
    Serial.println(p3);
    Serial.print(" ");
    Serial.println(p4);
    Serial.print("Errores: ");
    Serial.println(errTotal);
    ////////Eficiencia Salida Pantalla Eficiencia
    Serial.print("Eficiencia:  ");
    Serial.println(efficiency[i]); 
}
