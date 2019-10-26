#include <VirtualWire.h>
 
const int dataPin = 9;
const int ledPin = 13;


uint8_t msg[4]; 


const int pinPWMA = 3;
const int pinAIN2 = 2;
const int pinAIN1 = 4;
const int pinBIN1 = 8;
const int pinBIN2 = 7;
const int pinPWMB = 6;
const int pinSTBY = 5;

const int waitTime = 2000;   //espera entre fases
const int speed = 200;      //velocidad de giro
 
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN1, pinBIN2 };

int dataALF;
int dataALB;
int dataBRF;
int dataBRB;

enum moveDirection {
   forward,
   backward
};
 
enum turnDirection {
   clockwise,
   counterClockwise
};

void setup() {
  Serial.begin(9600);
  vw_setup(2000);
  vw_set_rx_pin(dataPin);
  vw_rx_start();
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
}

void loop() {

  //SOlo movemos el robot cuando llega señal, sino esta parado
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    if (vw_get_message(buf, &buflen)){
    dataALF=buf[0];
    dataALB=buf[1];
    dataBRF=buf[2];
    dataBRB=buf[3];
    Serial.print("LF: ");
    Serial.print(dataALF);
    Serial.print(", ");
    Serial.print("LB: ");
    Serial.print(dataALB);
    Serial.print(", ");
    Serial.print("RF: ");
    Serial.print(dataBRF);
    Serial.print(", ");
    Serial.print("RB: ");
    Serial.print(dataBRB);
    Serial.print("\n");
    moveMotorForward(pinMotorA, dataALF);
    moveMotorForward(pinMotorB, dataBRF);

    if((dataALF>0&&dataALB>0)||(dataBRF>0&&dataBRB>0)){
      //evitamos que llegue una señal contradictoria (alante atras a la vez)
      Serial.println("BOTH ABOVE ERROR");
      fullStop();
      
    }else{
      enableMotors();
      if((dataALF>0&&dataALB==0)&&(dataBRF>0&&dataBRB==0)){
        //vamos adelante
        moveMotorForward(pinMotorA, dataALF);
        moveMotorForward(pinMotorB, dataBRF);
        
      }else if((dataALF==0&&dataALB>0)&&(dataBRF==0&&dataBRB>0)){
        //vamos atras
        moveMotorBackward(pinMotorA, dataALB);
        moveMotorBackward(pinMotorB, dataBRB);
        
      }else if((dataALF>0&&dataALB==0)&&(dataBRF==0&&dataBRB>0)){
        //vamos derecha
        moveMotorForward(pinMotorA, dataALF);
        moveMotorBackward(pinMotorB, dataBRB);
        
      }else if((dataALF==0&&dataALB>0)&&(dataBRF>0&&dataBRB==0)){
        //vamos izquierda
        moveMotorBackward(pinMotorA, dataALB);
        moveMotorForward(pinMotorB, dataBRF);
        
      }      
    }
    
  }else{
    //Si no hay señal se para
    //Podria ser tambien por poca vlocidad de transmision
    fullStop();
  }

  //pausa para sincronizar con el mando, ajustar
  delay(20);

}

void moveMotorForward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], speed);
}
 
void moveMotorBackward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], HIGH);
 
   analogWrite(pinMotor[0], speed);
}

void fullStop()
{
   disableMotors();
   stopMotor(pinMotorA);
   stopMotor(pinMotorB);
}

void stopMotor(const int pinMotor[3])
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], 0);
}
 
 
void enableMotors()
{
   digitalWrite(pinSTBY, HIGH);
}
 
void disableMotors()
{
   digitalWrite(pinSTBY, LOW);
}
