#include "Arduino.h"
#include "Scanner.h"

using namespace std;


Scanner::Scanner()
{
  greenPin = 10;
  bluePin = 2;
  redPin = 6;
  pingPin = 4;
  echoPin = 5;

  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);

  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  analogWrite(redPin, 0);
  
    
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);
}


void Scanner::triggerLED(int dist)
{
  if(dist > 50)
  {
    analogWrite(bluePin, 10);
    analogWrite(redPin, 10);
    analogWrite(greenPin, 255);
  }
  else if(dist <= 50 && dist > 20)
  {
    analogWrite(greenPin, 29);
    analogWrite(redPin, 100);
    analogWrite(bluePin, 10);
  }
  else
  {
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    analogWrite(redPin, 255);
  }
  
  if(dist == 0)
  {
    analogWrite(bluePin, 25);
    analogWrite(redPin, 25);
    analogWrite(greenPin, 25);
  }
}



// convert amount of time the ping took to return to the sensor and devide 
// by 29, as sound will travel approximately 29 cm in one millisecond. then 
// divide by 2, as we only need to know the distance from the sensor to the object,
// and the ping traveled twoce that distance (there and back).
int Scanner::convertToCm(int t)
{
  return t / 29 / 2;
}



int Scanner::scan()
{
  int timer;

  //pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delay(2);
  digitalWrite(pingPin, HIGH);
  delay(5);
  digitalWrite(pingPin, LOW);

  //pinMode(echoPin, INPUT);
  timer = pulseIn(echoPin, HIGH);

  return convertToCm(timer);
}


