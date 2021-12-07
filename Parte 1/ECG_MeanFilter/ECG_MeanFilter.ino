 /*
  * PROYECTO 1ER BIMESTRE
  * INTEGRANTES:  HEREDIA ARIEL, JUMA KENETH, MONCAYO KARLA
  * TEMA:         SUAVIZADO DE SEÑALES - MEAN FILTER
  */
 
 #define tam_sig 120 // declaramos el tamaño de la señal ecg
 #define tam_fil 15 // declaramos el tamaño de la señal del filtro realizado en Tfilter

 extern double ECG_AmpHJM[tam_sig];  //Vector de muestras de señal, externo
 extern double filtro_fir[tam_fil]; //Filtro FIR
 double output[tam_sig+tam_fil];  //Señal suavizada
 double output_sig[tam_sig];

void setup() {
  Serial.begin(9600);
}

void loop() {
 convolucion((double *)&ECG_AmpHJM, (double *)&output[0], (double *)&filtro_fir[0], (int) tam_sig, (int) tam_fil); //Llamado al método de Convolución
 filtro_media((double *)&ECG_AmpHJM, (double *)&output_sig[0], (int) tam_sig,5); //Llamado al algoritmo de media
 graficacion();  
 SNRmetrica();   
 delay(100);
}

//-----------------------------------------------Método de convolución para el filtro FIR-------------------------------------------------//
void convolucion (double x_n[],double y_n[], double h_n[], int x_tam, int h_tam){  // convulucion
  int i,j; // declaramos variables para hacer dos ciclos
  /*-----------------------------------------------------------------Ciclos para la convolución------------------------------*/
  for(i=0; i<(x_tam+h_tam); i++){
    y_n[i]=0;                       //Inicializa la salida en 0          
    }
  for(i=0; i<x_tam; i++){
    for(j=0; j<h_tam; j++){
      y_n[i+j] += x_n[i]*h_n[j]; //Cálculo de la convolución
    }
  }
}
//---------------------------------------SUAVIZADO DE SEÑAL MEDIANTE EL ALGORITMO DE LA MEDIA-------------------------------------------------------
void filtro_media(double sig_in[], double output_sig[], int sig_tam, int filter){
  int i,j; 
  i=floor(filter/2);  
  while(i<sig_tam-floor(filter/2)-1){ 
    i++;
    output_sig[i]=0;
    j=-floor(filter/2);
    while(j<floor(filter/2)){
      j++;
      output_sig[i]=output_sig[i]+sig_in[i+j];
      }
      output_sig[i]=output_sig[i]/((filter*2)+1);
    }
  }

//_-----------------------------------------------------Método de cálculo de la métrica SNR------------------------------------------------------------///
void SNRmetrica(){
  int i;
  //Media de la señal de pulsos
  float suma=0;
  for(i=0;i<tam_sig;i++){  //Recorrido del vector de muestras de señal
    suma += ECG_AmpHJM[i];  //suma acumulativa de las muestras de señal
  }
  float promedio=suma/tam_sig;
  float v1=map(promedio,0,1023,0,5);
  Serial.println(String("Tension de señal: ")+String(v1)); //Voltaje de señal
  
  //Media de la señal filtrada por media
  float suma2=0;
  for(i=0;i<tam_sig;i++){   //Recorrido del vector de señal filtrada
    suma2 += output[i];     //suma acumulativa de las muestras de señal filtrada
  }
  float promedio2=suma2/tam_sig;                          //Valor medio
  float v2=map(promedio2,0,1023,0,5);
  Serial.println(String("Tension de señal filtrada: ")+String(v2)); //Voltaje de señal filtrada

  //---------------------------------------Metrica de la relación SNR aplicada a tensiones---------------------------------------------------///
  double metricaSNR = 20*log(v2/v1);              //Obtencion de métrica según la relación de voltajes
  Serial.println(String("SNR (dB): ")+String(metricaSNR));
  }


//Grafica de la señal 
void graficacion(){   
  int i;
  for(i=0; i<tam_sig; i++){
    Serial.print(ECG_AmpHJM[i]);
    Serial.print(",");
    Serial.print(output_sig[i]);
    Serial.print(",");
    Serial.println(output[i]);
    delay(5);
    }
  }  
