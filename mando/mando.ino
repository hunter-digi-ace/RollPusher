#include <VirtualWire.h>
 
const int dataPin = 9;
const int ledPin = 13;



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

uint8_t msg[4];



void setup() {
    vw_setup(2000);
    vw_set_tx_pin(dataPin);
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
  
  digitalWrite(ledPin, true);
  vw_send((uint8_t *)msg, sizeof(msg));
  vw_wait_tx();
  digitalWrite(ledPin, false);

  Serial.print("LF: ");
  Serial.print(pwmMLforwards);
  Serial.print(", ");
  Serial.print("LB: ");
  Serial.print(pwmMLbackwards);
  Serial.print(", ");
  Serial.print("RF: ");
  Serial.print(pwmMRforwards);
  Serial.print(", ");
  Serial.print("RB: ");
  Serial.print(pwmMRbackwards);
  Serial.print("\n");

  //Para no llenar tan rapido el log
  //delay(20);

}
