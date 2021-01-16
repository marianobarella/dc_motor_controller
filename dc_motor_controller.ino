/* 
  Remote controller with joystick.
  Two-axis joystick controls two DC motors using a L298N shield.
  A threshold is defined to avoid undesired movement due to read noise at analog inputs.
  This code controls one axis at a time. That is, active axis must return to home (below threshold) to allow the other axis to move.
  If none axis is active, the motors remain halted (unpowered).
  
  Author: Mariano Barella
  Contact: marianobarella@gmail.com
  Creation date: 25th August, 2020
  Update date: 11th December, 2020
  Buenos Aires, Argentina

  Arduino nano: use OLD bootloader.

  Cable color of my project (ignore):
  ENA gray D8
  IN1 violeta D7
  IN2 yellow D6
  IN3 green D5
  IN4 red D4
  ENB orange D3
  
 */

int joystickPinX = A0;    // select the input pin to sense joystick's X value 
int joystickPinY = A1;    // select the input pin to sense joystick's Y value
double joystickXValue = 505;  // variable to store the average value coming from the joystick
double joystickYValue = 498;  // variable to store the average value coming from the joystick
int joystickValue_aux = 0; // variable to store the value coming from the joystick
int i = 0;
int vel = 0; // proportional to focuser speed
int in3 = 5;    // Input3 of L298N, to digital pin D5 of the Arduino
int in4 = 4;    // Input4 of L298N, to digital pin D4 of the Arduino
int enaB = 3;    // ENB of L298N, to digital pin D3 of the Arduino
int in1 = 7;    // Input1 of L298N, to digital pin D7 of the Arduino
int in2 = 6;    // Input2 of L298N, to digital pin D6 of the Arduino
int enaA = 8;    // ENA of L298N, to digital pin D8 of the Arduino
int joystick_th_fw = 525; // define joystick threshold forward
int joystick_th_bw = 485; // define joystick threshold backward

// define joystick auxiliary function
double joystickX() {
  // reset auxiliary variable
  joystickValue_aux = 0;
  // read the value from the sensor, several times:  
  for (i = 0; i < 10; i++) {
    joystickValue_aux += analogRead(joystickPinX);
  }
  // calculate the average
  joystickXValue = joystickValue_aux/10;
 
  // print data on serial monitor
  Serial.print("Joystick X: "); 
  Serial.println(joystickXValue); 
  
  //delay(500);
  
  return joystickXValue;
}

// define joystick auxiliary function
double joystickY() {
  // reset auxiliary variable
  joystickValue_aux = 0;
  // read the value from the sensor, several times:  
  for (i = 0; i < 10; i++) {
    joystickValue_aux += analogRead(joystickPinY);
  }
  // calculate the average
  joystickYValue = joystickValue_aux/10;
 
  // print data on serial monitor
  Serial.print("Joystick Y: "); 
  Serial.println(joystickYValue); 
  
  //delay(500);
  
  return joystickYValue;
}

void haltMotor() {
  // halt motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enaB, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enaA, 0);
  Serial.println("Motor stopped");
  Serial.println(); 
}

void forwardX() {
  // X forward
  while (joystickXValue > joystick_th_fw) {
    // set rotation direction
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // set speed
    vel = (joystickXValue - joystick_th_fw)*255/(1023 - joystick_th_fw);
    analogWrite(enaB, vel);
    Serial.print("Speed X fw:");
    Serial.println(vel);

    // update joystick value
    joystickXValue = joystickX();
  }  
}

void backwardX() {
  // X backward
  while (joystickXValue < joystick_th_bw) {
    // change rotation direction
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // set speed
    vel = (joystick_th_bw - joystickXValue)*255/joystick_th_bw;
    analogWrite(enaB, vel);
    Serial.print("Speed X bw:");
    Serial.println(vel);
    
    // update joystick value
    joystickXValue = joystickX();
  }  
}

void forwardY() {
  // Y forward
  while (joystickYValue > joystick_th_fw) {
    // set rotation direction
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // set speed
    vel = (joystickYValue - joystick_th_fw)*255/(1023 - joystick_th_fw);
    analogWrite(enaA, vel);
    Serial.print("Speed Y fw:");
    Serial.println(vel);

    // update joystick value
    joystickYValue = joystickY();
  }  
}

void backwardY() {
  // Y backward
  while (joystickYValue < joystick_th_bw) {
    // change rotation direction
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // set speed
    vel = (joystick_th_bw - joystickYValue)*255/joystick_th_bw;
    analogWrite(enaA, vel);
    Serial.print("Speed Y bw:");
    Serial.println(vel);
    
    // update joystick value
    joystickYValue = joystickY();
  }  
}

//////////////////////////////////////////////////////////////////////

void setup()
{
 Serial.begin(9600);
 pinMode (enaB, OUTPUT); 
 pinMode (in3, OUTPUT);
 pinMode (in4, OUTPUT);
 pinMode (enaA, OUTPUT); 
 pinMode (in1, OUTPUT);
 pinMode (in2, OUTPUT);
}

void loop()
{ 
  haltMotor();

  // measure joystick's X value
  joystickXValue = joystickX();
  // measure joystick's Y value
  joystickYValue = joystickY();
  Serial.println(); 
  
  forwardX();
  backwardX();
  forwardY();
  backwardY();
}
