#include <nRF24L01.h>
#include "printf.h"
#include <RF24.h>
#include <RF24_config.h>
//Pines del joystick.
int potPinUp = A2;
int potPinSide = A3;
int potPinThrot = A1;
int potPinHeading = A0;

//limites de movimiento del Joystick
int upBase = 10;
int upMax = 1013;
int upRestMin = 490;
int upRestMax = 530;

int sideBase = 10;
int sideMax = 1013;
int sideRestMin = 496;
int sideRestMax = 536;


int upVal;
int sideVal;

int motorLeft;
int motorRight;
int forBack;

//datos a enviar (0-255)
int pwmMLforwards;
int pwmMLbackwards;
int pwmMRforwards;
int pwmMRbackwards;
int pwmWeapon;

//radio setups - iniciamos la radio
RF24 radio(9,10);
const uint64_t pipe = 0xE800F0EEE1LL;  //importante tener el mismo en los dos codigos

uint8_t msg[5];



void setup() {
  Serial.begin(9600);
 
  radio.begin();
  radio.openWritingPipe(pipe); 
  
}

void loop() {
  //miramos las posiciones por defecto
  upVal = analogRead(potPinUp);
  sideVal = analogRead(potPinSide);
  
  upVal=constrain(upVal,upBase,upMax);
  sideVal=constrain(sideVal,sideBase,sideMax);
  
  //limpiamos los valores antiguos
  motorLeft = 0;
  motorRight = 0;

  //pasamos la señal al movimiento de avanzar oir marcha atras
  //si lo ponemos en -1 ira hacia atras
  forBack=1;

  if(upVal<upRestMax&&upVal>upRestMin&&sideVal<sideRestMax&&sideVal>sideRestMin){
    //Condiciones de parada
    motorLeft = 0;
    motorRight = 0;
  }else{
    //miramos los movimientos
    if(upVal>upRestMax){
      //acia delante
      motorLeft = motorLeft + map(upVal,upRestMax,upMax,0,255);
      motorRight = motorRight + map(upVal,upRestMax,upMax,0,255);
      forBack=-1;
    }else if(upVal<upRestMin){
      //hacia attras
      motorLeft = motorLeft - map(upVal,upRestMin,upBase,0,255);
      motorRight = motorRight - map(upVal,upRestMin,upBase,0,255);
      forBack=1;
    }

    //comprobamos las posiciones
    if(sideVal>sideRestMax){
      //right
      motorLeft = motorLeft + (map(sideVal,sideRestMax,sideMax,0,255)*forBack);
      motorRight = motorRight - (map(sideVal,sideRestMax,sideMax,0,255)*forBack);
    }else if(sideVal<sideRestMin){
      //left
      motorLeft = motorLeft - (map(sideVal,sideRestMin,sideBase,0,255)*forBack);
      motorRight = motorRight + (map(sideVal,sideRestMin,sideBase,0,255)*forBack);
    }

    //ponemos la constrain de -255a 255
    motorLeft = constrain(motorLeft,-255,255);
    motorRight = constrain(motorRight,-255,255);
    
  }

  //pwm motor derecho
  if (motorLeft==0){
    pwmMLforwards = 0;
    pwmMLbackwards = 0;
  }else if(motorLeft<0){
    pwmMLforwards = 0;
    pwmMLbackwards = (motorLeft*-1);
  }else if(motorLeft>0){
    pwmMLforwards = motorLeft;
    pwmMLbackwards = 0;
  }

  //pwm motor izquierdo
  if (motorRight==0){
    pwmMRforwards = 0;
    pwmMRbackwards = 0;
  }else if(motorRight<0){
    pwmMRforwards = 0;
    pwmMRbackwards = (motorRight*-1);
  }else if(motorRight>0){
    pwmMRforwards = motorRight;
    pwmMRbackwards = 0;
  }

 //imprimimos los valores en el serial
  msg[0]=pwmMLforwards;
  msg[1]=pwmMLbackwards;
  msg[2]=pwmMRforwards;
  msg[3]=pwmMRbackwards;
  radio.write(msg,5);
  Serial.print(pwmMLforwards);
  Serial.print(", ");
  Serial.print(pwmMLbackwards);
  Serial.print(", ");
  Serial.print(pwmMRforwards);
  Serial.print(", ");
  Serial.print(msg[3]);

  //Para no llenar tan rapido el log
  delay(10);

}
