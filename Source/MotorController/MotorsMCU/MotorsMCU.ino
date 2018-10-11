// 
//  Yellow Tank Robot v1.0
//  -- Motors controls through I2C by Raspberry pi --
//  Started: Oct 01, 2018
//  Author: Arnaud Thiercelin
//

#include <Wire.h>

// Left Motor
#define LEFTPIN1    4
#define LEFTPIN2    5
#define LEFTPWMPIN  6

// Right Motor
#define RIGHTPIN1   7
#define RIGHTPIN2   8
#define RIGHTPWMPIN 9

// Motor PWM settings
#define MOTOR_SLOWEST 100
#define MOTOR_FASTEST 255

// I2C

//
//  *** SETUP ***
//

void setup() {
  setupMotors();
  setupI2C();
  setupDebugging();
}

void setupMotors() {
  // Left Motor Pins
  pinMode(LEFTPIN1, OUTPUT);
  pinMode(LEFTPIN2, OUTPUT);
  pinMode(LEFTPWMPIN, OUTPUT);

  setLeftForward();

  // Right Motor Pins
  pinMode(RIGHTPIN1, OUTPUT);
  pinMode(RIGHTPIN2, OUTPUT);
  pinMode(RIGHTPWMPIN, OUTPUT);

  setRightForward();
}

void setupI2C() {
  Wire.begin(8);
  Wire.onReceive(receivedI2C);
  
}

void setupDebugging() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);  
}

//
//  *** LOOP ***
//



void loop() {
 
  delay(100);
}

void motorTestLoop() {
   for (int pwm = MOTOR_SLOWEST; pwm < MOTOR_FASTEST; pwm++) {
    analogWrite(LEFTPWMPIN, pwm);
    analogWrite(RIGHTPWMPIN, pwm);
    Serial.print(pwm);
    Serial.print("\n");
    delay(5);
  }

  for (int pwm = MOTOR_FASTEST; pwm >= MOTOR_SLOWEST - 30 ; pwm--) {
    analogWrite(LEFTPWMPIN, pwm);
    analogWrite(RIGHTPWMPIN, pwm);
    Serial.print(pwm);
    Serial.print("\n");
    delay(5);
  }



// After once, we stop
    analogWrite(LEFTPWMPIN, 0);
    analogWrite(RIGHTPWMPIN, 0);
    
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(500);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(500);
    } 
}

//
//  *** I2C ***
//

void receivedI2C() {
    Serial.println("---");
    while (Wire.available()) {
      // Read commands
      int data = Wire.read();

      Serial.print("I2C Received: ");
      Serial.println(data);
      handleMotorCommand(data);
    }
    Serial.println("Done Reading");
}

//
//  *** MOTORS FUNCTIONS ***
//

// LEFT MOTORS

void handleMotorCommand(int i2cValue) {
// Commands are defined as such.
// - Values sent are between [0, 200]
// - Values are then mapped to [-100, 100]
// - Positive values means forwards
// - Negative values means bacwards
// - Odd values means Left
// - Even values means Right

  int commandValue = i2cValue - 100;
  Serial.print("Command Receive: ");
  Serial.println(commandValue);

  if (commandValue == 0) {
    setLeftSpeed(0);
    setRightSpeed(0);
    Serial.println("Stopping");
  } else if (commandValue % 2) {
    Serial.print("Activating Left");
    if (commandValue > 0) {
      setLeftForward();
      setLeftSpeed(abs(commandValue));
      Serial.println(" Forward");
    } else {
      setLeftBackward();
      setLeftSpeed(abs(commandValue));
      Serial.println (" Backward");
    }
  } else {
    Serial.print("Activating Right");
    if (commandValue > 0) {
      setRightForward();
      setRightSpeed(abs(commandValue));
      Serial.println(" Forward");
    } else {
      setRightBackward();
      setRightSpeed(abs(commandValue));
      Serial.println (" Backward");
    }
  }
}

void setLeftSpeed(int speedValue) {
  static int currentSpeed = 0;
  int pwm = currentSpeed;
  
  if (speedValue == 0) {
    pwm = 0;    
  } else {
    pwm = map(speedValue, 0, 200, MOTOR_SLOWEST, MOTOR_FASTEST);
  }
//  for (int pwm = MOTOR_SLOWEST; pwm < MOTOR_FASTEST; pwm++) {
    analogWrite(LEFTPWMPIN, pwm);
    Serial.print("Left Speed: ");
    Serial.println(pwm);
//    delay(5);
//  }

  currentSpeed = pwm;
}

void setLeftForward() {
  digitalWrite(LEFTPIN1, HIGH);
  digitalWrite(LEFTPIN2, LOW);
}

void setLeftBackward() {
  digitalWrite(LEFTPIN1, LOW);
  digitalWrite(LEFTPIN2, HIGH);
}

// RIGHT MOTORS

void setRightSpeed(int speedValue) {
  static int currentSpeed = 0;
  int pwm = currentSpeed;
  
  if (speedValue == 0) {
    pwm = 0;    
  } else {
    pwm = map(speedValue, 0, 200, MOTOR_SLOWEST, MOTOR_FASTEST);
  }
//  for (int pwm = MOTOR_SLOWEST; pwm < MOTOR_FASTEST; pwm++) {
    analogWrite(RIGHTPWMPIN, pwm);
    Serial.print("Right Speed: ");
    Serial.println(pwm);
//    delay(5);
//  }

  currentSpeed = pwm;
}

void setRightForward() {
  digitalWrite(RIGHTPIN1, HIGH);
  digitalWrite(RIGHTPIN2, LOW);
}

void setRightBackward() {
  digitalWrite(RIGHTPIN1, LOW);
  digitalWrite(RIGHTPIN2, HIGH);
}



