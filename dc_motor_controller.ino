/* 
  Remote focuser
  
  Author: Mariano Barella
  Date: 25th August, 2020
  Buenos Aires, Argentina
 */

int joystickPin = A0;    // select the input pin to sense joystick's value
double joystickValue = 505;  // variable to store the average value coming from the joystick
int joystickValue_aux = 0; // variable to store the value coming from the joystick
int i = 0;
int vel = 0; // proportional to focuser speed
int in3 = 5;    // Input3 of L298N, to digital pin D5 of the Arduino
int in4 = 4;    // Input4 of L298N, to digital pin D4 of the Arduino
int enaB = 3;    // ENB of L298N, to digital pin D3 of the Arduino
int joystick_th_fw = 525; // define joystick threshold forward
int joystick_th_bw = 485; // define joystick threshold backward

// define joystick auxiliary function
double joystick() {
  // reset auxiliary variable
  joystickValue_aux = 0;
  // read the value from the sensor, several times:  
  for (i = 0; i < 10; i++) {
    joystickValue_aux += analogRead(joystickPin);
  }
  // calculate the average
  joystickValue = joystickValue_aux/10;
 
  // print data on serial monitor
  Serial.print("Joystick: "); 
  Serial.println(joystickValue); 
  
  delay(100);
  
  return joystickValue;
}

void setup()
{
 Serial.begin(9600);
 pinMode (enaB, OUTPUT); 
 pinMode (in3, OUTPUT);
 pinMode (in4, OUTPUT);
}

void loop()
{ 
  // halt motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enaB, 0);
  Serial.println("Motor stopped");
  
  // measure joystick's value
  joystickValue = joystick();
 
  // focuser forward
  while (joystickValue > joystick_th_fw) {
    // set rotation direction
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // set speed
    vel = (joystickValue - joystick_th_fw)*255/(1023 - joystick_th_fw);
    analogWrite(enaB, vel);
    Serial.print("Speed fw:");
    Serial.println(vel);

    // update joystick value
    joystickValue = joystick();
  }  
  
  // focuser backward
  while (joystickValue < joystick_th_bw) {
    // change rotation direction
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // set speed
    vel = (joystick_th_bw - joystickValue)*255/joystick_th_bw;
    analogWrite(enaB, vel);
    Serial.print("Speed dw:");
    Serial.println(vel);
    
    // update joystick value
    joystickValue = joystick();
  }  
  
}
