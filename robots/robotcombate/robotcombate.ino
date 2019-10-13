#include <SoftPWM.h>
#include <SoftPWM_timer.h>

#include <nRF24L01.h>
#include "printf.h"
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL; 
uint8_t msg[5]; 


int pinPWMA = 3;
int pinPWMB = 6;
int pinSTBY = 5;
int leftFor = 2;
int leftBack=4;
int rightFor = 8;
int rightBack=7;

int data;
int dataLF;
int dataLB;
int dataRF;
int dataRB;

void setup() {
  
  radio.begin();
  radio.openReadingPipe(1,pipe); 
  radio.startListening();
  SoftPWMBegin();
  digitalWrite(pinSTBY, HIGH);
  analogWrite(pinPWMA, 200);
  analogWrite(pinPWMB, 200);
  SoftPWMSet(leftFor, 0);
  SoftPWMSet(leftBack, 0);
  SoftPWMSet(rightFor, 0);
  SoftPWMSet(rightBack, 0);


}

void loop() {

  //SOlo movemos el robot cuando llega señal, sino esta parado
  if (radio.available()){
    radio.read(msg,5);
    dataLF=msg[0];
    dataLB=msg[1];
    dataRF=msg[2];
    dataRB=msg[3];
    data=msg[4];

    //analogWrite(motor,data);
    if((dataLF>0&&dataLB>0)||(dataRF>0&&dataRB>0)){
      //evitamos que llegue una señal contradictoria (alante atras a la vez)
      Serial.println("BOTH ABOVE ERROR");
      SoftPWMSet(leftFor, 0);
      SoftPWMSet(leftBack, 0);
      SoftPWMSet(rightFor, 0);
      SoftPWMSet(rightBack, 0);
      
    }else{
      SoftPWMSet(leftFor, dataLF);
      SoftPWMSet(leftBack, dataLB);
      SoftPWMSet(rightFor, dataRF);
      SoftPWMSet(rightBack, dataRB);
    }
    
  }else{
    //Si no hay señal se para
    //Podria ser tambien por poca vlocidad de transmision
    SoftPWMSet(leftFor, 0);
    SoftPWMSet(leftBack, 0);
    SoftPWMSet(rightFor, 0);
    SoftPWMSet(rightBack, 0);
  }

  //pausa para sincronizar con el mando, ajustar
  delay(20);

}
