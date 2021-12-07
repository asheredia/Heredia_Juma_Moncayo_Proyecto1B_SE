

//----------------------------------------------------------------------------------------------------------
//  Variables
#define tam_sig 100               //Tamaño de muestra de señal ECG de base de datos
extern int ecg_100Hz [tam_sig];   //Señal externa
//Parámetros para el cálculo de la Media Movil Exponencial
float S;                    //Variable asociada a la Media Movil Exponencial
float alpha=0.2;            //Factor de suavizado
int umbral = 550;           //Umbral de frecuencia cardiaca

 /*
  * PROYECTO 1ER BIMESTRE
  * INTEGRANTES:  HEREDIA ARIEL, JUMA KENETH, MONCAYO KARLA
  * TEMA:         SUAVIZADO DE SEÑALES - No aplicación filtro, Algoritmo de Media Móvil Exponencial
  */
void setup() {
   Serial.begin(9600);      
}


void loop() {
 
  for(int i=0;i<tam_sig;i++){
    //S[-1]=0;
    S=(alpha*ecg_100Hz[i])+((1-alpha)*S); //Aplicación del algoritmo
    Serial.println(ecg_100Hz[i]);         //Impresión señal original
    Serial.print(",");
    Serial.println(S);                    //Señal tratadasin Filtro FIR
    delay(10);
  
    if(ecg_100Hz[i] > umbral){             
      digitalWrite(13,HIGH);
    } else {
       digitalWrite(13,LOW);               
    }
  
    
  }
  
  
delay(10);

}
