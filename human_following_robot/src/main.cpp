#include <Arduino.h>
#include <PPMReader.h>

int interruptPin = 3;
int channelAmount = 8;
PPMReader ppm(interruptPin, channelAmount);

//motor 1 pin 
int EN1_B = 4; 
int IN1_4 = 5;
int IN1_3 = 6;

//motor 2 pin
int IN2_2 = 7;
int IN2_1 = 8;
int EN2_A = 9; 

//motor 3 pin
int IN3_2 = 12;
int IN3_1 = 11;
int EN3_A = 10; 

int deadzone = 20;

//function to set motor 1 direction
void motor1_set_direction(char direction){
  switch (direction)
  {
  case 'f':
    digitalWrite(IN1_3, HIGH);
    digitalWrite(IN1_4, LOW);
    break;
  case 'b':
    digitalWrite(IN1_3, LOW);
    digitalWrite(IN1_4, HIGH);

  default:
    break;
  }
}

void motor2_set_direction(char direction){
  switch (direction)
  {
  case 'f':
    digitalWrite(IN2_1, LOW);
    digitalWrite(IN2_2, HIGH);
    break;
  case 'b':
    digitalWrite(IN2_1, HIGH);
    digitalWrite(IN2_2, LOW);
    
  default:
    break;
  }
}

void motor3_set_direction(char direction){
  switch (direction)
  {
  case 'f':
    digitalWrite(IN3_1, LOW);
    digitalWrite(IN3_2, HIGH);
    break;
  case 'b':
    digitalWrite(IN3_1, HIGH);
    digitalWrite(IN3_2, LOW);
    
  default:
    break;
  }
}

int pulseToPWM(int pulse){
  if (pulse >= 1000){
    pulse = map(pulse, 1000, 2000, -500, 500);
    pulse = constrain(pulse, -255, 255);
  }
  else {
    pulse = 0;
  }
  if (abs(pulse) <= deadzone){
    pulse = 0;
  }
  return pulse;
}

//setup
void setup() {
    Serial.begin(115200);
    pinMode(EN1_B, OUTPUT);
    pinMode(IN1_3, OUTPUT);
    pinMode(IN1_4, OUTPUT);
    pinMode(EN2_A, OUTPUT);
    pinMode(IN2_1, OUTPUT);
    pinMode(IN2_2, OUTPUT);
    pinMode(EN3_A, OUTPUT);
    pinMode(IN3_1, OUTPUT);
    pinMode(IN3_2, OUTPUT);
  
}

//main loop
void loop() {
  unsigned int x = ppm.latestValidChannelValue(1, 0);
  unsigned int y = ppm.latestValidChannelValue(2, 0);
  unsigned int z = ppm.latestValidChannelValue(4, 0);
  int y_lin = pulseToPWM(y);
  int x_lin = pulseToPWM(x);
  int z_rot = pulseToPWM(z);

  Serial.print("X linear speed : ");
  Serial.print(x_lin);
  Serial.print(" ");
  Serial.print("Y linear speed : ");
  Serial.print(y_lin);
  Serial.print(" ");
  Serial.print("z linear speed : ");
  Serial.println(z_rot);

  //forward
  if(y_lin > 0 && x_lin == 0){
    motor1_set_direction('b');
    motor2_set_direction('f');
    analogWrite(EN1_B, abs(y_lin));
    analogWrite(EN2_A, abs(y_lin));
    analogWrite(EN3_A, 0);
  }
  //backward
   else if(y_lin < 0 && x_lin == 0){
    motor1_set_direction('f');
    motor2_set_direction('b');
    analogWrite(EN1_B, abs(y_lin));
    analogWrite(EN2_A, abs(y_lin));
    analogWrite(EN3_A, 0);
  }
  //forward right
  else if(y_lin > 0 && x_lin > 0){
    motor1_set_direction('b');
    motor3_set_direction('f');
    analogWrite(EN1_B, abs(y_lin));
    analogWrite(EN2_A, 0);
    analogWrite(EN3_A, abs(x_lin));
  }
  //forward left
  else if(y_lin > 0 && x_lin < 0){
    motor2_set_direction('f');
    motor3_set_direction('b');
    analogWrite(EN1_B, 0);
    analogWrite(EN2_A, abs(y_lin));
    analogWrite(EN3_A, abs(x_lin));
  }
  //backward left
  else if(y_lin < 0 && x_lin < 0){
    motor1_set_direction('f');
    motor3_set_direction('b');
    analogWrite(EN1_B, abs(y_lin));
    analogWrite(EN2_A, 0);
    analogWrite(EN3_A, abs(x_lin));
  }
  //backward right
  else if(y_lin < 0 && x_lin > 0){
    motor1_set_direction('b');
    motor3_set_direction('f');
    analogWrite(EN1_B, 0);
    analogWrite(EN2_A, abs(y_lin));
    analogWrite(EN3_A, abs(x_lin));
  }
  //right
  else if(y_lin == 0 && x_lin > 0){
    motor1_set_direction('b');
    motor2_set_direction('b');
    motor3_set_direction('f');
    analogWrite(EN1_B, abs(x_lin));
    analogWrite(EN2_A, abs(x_lin));
    analogWrite(EN3_A, abs(x_lin));
  }
  //left
  else if(y_lin == 0 && x_lin < 0){
    motor1_set_direction('f');
    motor2_set_direction('f');
    motor3_set_direction('b');
    analogWrite(EN1_B, abs(x_lin));
    analogWrite(EN2_A, abs(x_lin));
    analogWrite(EN3_A, abs(x_lin));
  }
  else if(z_rot > 0 && y_lin == 0 && x_lin == 0){
    motor1_set_direction('b');
    motor2_set_direction('b');
    motor3_set_direction('b');
    analogWrite(EN1_B, abs(z_rot));
    analogWrite(EN2_A, abs(z_rot));
    analogWrite(EN3_A, abs(z_rot));
  }
    else if(z_rot < 0 && y_lin == 0 && x_lin == 0){
    motor1_set_direction('f');
    motor2_set_direction('f');
    motor3_set_direction('f');
    analogWrite(EN1_B, abs(z_rot));
    analogWrite(EN2_A, abs(z_rot));
    analogWrite(EN3_A, abs(z_rot));
  }
  else{
    analogWrite(EN1_B, 0);
    analogWrite(EN2_A, 0);
    analogWrite(EN3_A, 0);
  }
 
}