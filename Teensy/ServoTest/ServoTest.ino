#include <Servo.h>

#define SERVO_PIN 9

// Servo variables
Servo theServo;
int servoStart = 72;
int servoEnd = 155;

void setup() {
  Serial.begin(115200);
  while(!Serial.available()) {}
  
  // Initialize the servo
  theServo.attach(SERVO_PIN);
  delay(500);
  theServo.write(servoStart);
  delay(500); // DON'T DELETE THIS DELAY OTHERWISE IT WON'T WORK
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'R') {
      theServo.write(servoStart);
      delay(500); // DON'T DELETE THIS DELAY OTHERWISE IT WON'T WORK
    } else if (c == 'P') {
      theServo.write(servoEnd);
      delay(500); // DON'T DELETE THIS DELAY OTHERWISE IT WON'T WORK
    }
  }
}
