 /*
  * PROYECTO 1ER BIMESTRE
  * INTEGRANTES:  HEREDIA ARIEL, JUMA KENETH, MONCAYO KARLA
  * TEMA:         SUAVIZADO DE SEÑALES - MEDIAN FILTER
  */

#define tam_sig 120 //tamaño de la señal orginal con 90 muestras
#define tam_fil 15 //orden del filtro


//----------------------SEÑALES GUARDADAS EN EL ARCHIVO SENIAL ADJUNTO-------------------------------------------
extern double ECG_AmpHJM[tam_sig];                  //Vector de muestras de señal
extern double Coef_FiltroFir[tam_fil];              //Vector de coeficientes del filtro                

double salida[tam_sig+tam_fil];                                       //Vector de salida
double mediana_signal[tam_sig];                                       //Vector para aplicación del filtro mediana

void setup() {
  Serial.begin(9600);                               //Habilitar cx serial
}

void loop() {
  convolucion((double *)&ECG_AmpHJM[0], (double *)&salida[0], (double *)&Coef_FiltroFir[0], (int) tam_sig, (int) tam_fil);  //Realizar convolución
  mediana ((double *)&ECG_AmpHJM[0], (double *)&mediana_signal[0], (int) tam_sig);                                          //Calculo de la mediana
  graficacion();                                                                                                  //Invoca la función de graficación
  SNR();                                                                                                          //Cálculo de SNR
  delay(100);                                                                                                     //Detiene al microc. en 100ms
  
}


//-----------------------------------------------Método de convolución para el filtro FIR-------------------------------------------------//
void convolucion(double x_n[],double y_n[], double h_n[], int x_tam, int h_tam){
  int i,j;
  /*-----------------------------------------------------------------Ciclos para la convolución------------------------------*/
  for(i=0;i<(x_tam+h_tam);i++){      
    y_n[i]=0;                         //Inicializa la salida en 0          
  }
  for(i=0;i<x_tam;i++){              
    for(j=0;j<h_tam;j++){                 
      y_n[i+j]+=x_n[i]*h_n[j];   //Cálculo de la convolución
    }
  }
}



//---------------------------------------SUAVIZADO DE SEÑAL MEDIANTE EL ALGORITMO DE LA MEDIANA-------------------------------------------------------

void mediana (double sig_in[], double mediana_signal[], int sig_tam){
  int i,j,k;
  float vector[5], sig_Temp;
  for(k=0;k<sig_tam;k++){   //Recorrido de la muestra de la señal
    vector[0]=sig_in[k+0];  //Cambio de la señal a la posición del vector 0
    vector[1]=sig_in[k+1];  //Cambio de la señal a la posición del vector 1
    vector[2]=sig_in[k+2];  //Cambio de la señal a la posición del vector 2
    vector[3]=sig_in[k+3];  //Cambio de la señal a la posición del vector 3
    vector[4]=sig_in[k+4];  //Cambio de la señal a la posición del vector 4
    for(i=0;i<(4);i++){     
      for(j=i+1;j<5;j++){
        if(vector[j]<vector[i]){
          //Intercambio de valores
          sig_Temp=vector[j];   
          vector[j]=vector[i];
          vector[i]=sig_Temp;  
        }
      }
    }
    mediana_signal[k]=vector[1];
  }
}
 //_-----------------------------------------------------Método de cálculo de la métrica SNR------------------------------------------------------------///
void SNR(){                                  
  //Media de la señal de pulsos
  float suma=0;
  for(int i=0;i<tam_sig;i++){                             //Recorrido del vector de muestras de señal de pulsos
      suma += ECG_AmpHJM[i];                              //Suma acumulativa de de todos los valores de muestras
  }
  float promedio=suma/tam_sig;                          //Valor medio de las muestras de la señal
  float v1=map(promedio,0,1023,0,5);
  Serial.println(String("Tension de señal: ")+String(v1)); //Voltaje de señal

  //Media de la señal filtrada por mediana
  float suma2=0;
  for(int i=0;i<tam_sig;i++){
    suma2 += mediana_signal[i];
  }
  float promedio2=suma2/tam_sig;                                          //divide par ael numero de datos de la señal
  float v2=map(promedio2,0,1023,0,5);
    
  Serial.println(String("Tension de señal filtrada: ")+String(v2)); //Imprime de forma serial el valor obtenido

//---------------------------------------Metrica de la relación SNR aplicada a tensiones---------------------------------------------------///
    double metricaSNR=20*log(v2/v1);                                        //
    Serial.println(String("SNR (dB): ")+String(metricaSNR));//Imprime la SNR en dB
    //Serial.print("");
    
}

//Grafica de la señal
void graficacion(){ 
  int i;
  for(i=0;i<tam_sig;i++){
    Serial.print(String("ECG orginal: ")+String(ECG_AmpHJM[i]));
    Serial.print(",");
    Serial.print(String("Filtrado con mediana: ")+String(mediana_signal[i]));
    Serial.print(",");
    Serial.println(String("ECG suavizada: ")+String(salida[i]));
    delay(5);
  }
}
