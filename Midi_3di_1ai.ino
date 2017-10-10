/*Midi_3di_1ai
 * 
 * Detecta flancos de subida en tres entradas digitales y envía 0 o 127
 * en los parámetros mídi definidos midiCH y baseCC.
 * Detecta cambios en una entrada analógica y envía su valor proporcional
 * al canal midi y parámetro definido.
 * 
 * Antonio Vásquez Briones.
 * Octubre 2017
 * 
 * Requiere flashear el dfu, con firmware Mocolufa-Dual y Atmel Flip
 * https://github.com/kuwatay/mocolufa
 */

#include <MIDI.h>

#define midiCH 4  //Puerto Midi a Utilizar
#define baseCC 20 //Parametro Base
#define in0 8     //Pin de entrada inicial
                  //los demás son consecutivos

MIDI_CREATE_DEFAULT_INSTANCE();

//variables utiles
int inputs[]={0,0,0,0};
int oldInputs[]={0,0,0,0};
boolean estados[3]={false,false,false};

void setup() {
  
  pinMode(in0,INPUT);
  pinMode(in0+1,INPUT);
  pinMode(in0+2,INPUT);
  Serial.begin(115200);
  MIDI.begin(midiCH);

}

void loop() {
  //Funcion que lee las entradas y las almacena
  //en un array
  lecturaEntradas();
  
  for(int i=0;i<=3;i++){

    //Se evalua si hay cambio
    if(oldInputs[i]!=inputs[i]){  

      //Se guarda el cambio
      oldInputs[i]=inputs[i];

      //Para los tres primeros datos digitales
      //Se busca un Flanco de Subida
      //Se descartan los flancos de bajada
      if((i<3)&&(inputs[i]!=0)){   
        
        estados[i]=!estados[i];   //cambia los estados
        MIDI.sendControlChange(baseCC+i,int(estados[i])*127,midiCH); //envía los Estados      
     
      }else if(i==3){
        //caso particular de la entrada analógica
        //si hay cambio entonces se envia
        
        //Se acondiciona a valores de 0 a 127
        int valor=map(inputs[i],0,1023,0,127);
        MIDI.sendControlChange(baseCC+i,valor,midiCH);
        
      }
      //imprimeValores(); //descomentar para debugging
    }
  }
}

void lecturaEntradas(){
  for(int i=0;i<=3;i++){
    inputs[i]=digitalRead(i+in0);
    if(i>2)
      inputs[i]=analogRead(0);
  } 
}

void imprimeValores(){
  for(int i=0;i<=3;i++){
    if(i<3){
      Serial.print(estados[i]);
      Serial.print(", ");
    }
    else{
      Serial.println(inputs[i]);
    }     
  }  
}


