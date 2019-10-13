int potPinUp = A2;
int potPinSide = A3;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("UP: ");
  Serial.print(analogRead(potPinUp));
  Serial.print(" Side: ");
  Serial.print(analogRead(potPinSide));
  //Serial.print(" Throttle: ");
  //Serial.print(analogRead(potPinThrot));
  //Serial.print(" Heading: ");
  //Serial.println(analogRead(potPinHeading));
  delay(100);
}
