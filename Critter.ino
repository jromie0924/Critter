#include "Scanner.h"
#include <Servo.h>
#include "Closeness_determine.h"
#include "Pitches.h"

/*PINS*/
int PWM_A   = 3; //LEFT MOTOR
int PWM_B   = 11;// RIGHT MOTOR
int BRAKE_A = 9;
int BRAKE_B = 8;
int DIR_A   = 12;
int DIR_B   = 13;
int trigPin = 5;
int echoPin = 4;
int redPin = 6;
int greenPin = 2;
int bluePin = 10;
int servoPin = 7;
int speakerPin = A12;

Scanner scanner;
Servo servo;
Closeness_determine state;
int pos;

int melody[] = {
  NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_E7, NOTE_D7};

int on[] = {NOTE_C6, NOTE_D7};
int onDuration[] = {16, 4};

int melodyDuration = 1000 / 32;



void setup()
{
  Serial.begin(9600);

  // Turn-on melody
  for(int a = 0; a < sizeof(on); a++)
  {
    int duration = 1000 / onDuration[a];
    tone(speakerPin, on[a], duration);
    int pause = duration * 1.30;
    delay(pause);
    noTone(speakerPin);
  }

  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A

  // Configure the B output
  pinMode(BRAKE_B, OUTPUT);  // Brake pin on channel B
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel B

  servo.attach(servoPin);
  servo.write(90);
  delay(500);
}



/*----------------------------------------THE GOOD STUFF----------------------------------------*/



void loop()
{
  for(int a = 0; a < 2; a++)
  {
    tone(speakerPin, NOTE_C8, 1000 / 16);
    int pauseBetweenNotes = (1000 / 16) * 1.30;
    delay(pauseBetweenNotes);
    noTone(speakerPin);
  }
  
  int distance = 0;
  for(int a = 0; a < 5; a++)
  {
    int k = scanner.scan();
    if(k > distance)
      distance = k;
  }
  int closeness = state.determine(distance);
  scanner.triggerLED(distance);

  while(distance > 30 || distance == 0)
  {
    goStraight();
    if(closeness == 0)
    {
      analogWrite(PWM_A, 245);
      analogWrite(PWM_B, 245);
      delay(10);
    }
    distance = scanner.scan();
    closeness = state.determine(distance);
  }
  brake();
  delay(10);
  obstacleTone();
  delay(10);
  reverse();
  analogWrite(PWM_A, 200);
  analogWrite(PWM_B, 200);
  delay(500);
  brake();
  tone(speakerPin, NOTE_C8, 1000 / 16);
  delay((1000 / 16) * 1.30);
  noTone(speakerPin);
  int pos = pan();
  delay(1000);
  Serial.print(pos);
  Serial.println(" degrees");


  if(pos < 70)
  {
    turnRight();
    int delayTime;
    if(pos >= 50 && pos < 70)
      delayTime = 350;
    else if(pos >= 70)
      delayTime = 650;
    analogWrite(PWM_B, 200);
    analogWrite(PWM_A, 200);
    delay(delayTime);
    brake();
  }
  else if(pos > 110)
  {
    turnLeft();
    int delayTime;
    if(pos >= 110 && pos < 120)
      delayTime = 350;
    else if(pos >= 120)
      delayTime = 650;
    analogWrite(PWM_A, 200);
    analogWrite(PWM_B, 200);
    delay(delayTime);
    brake();
  }

  else
  {
    servo.write(90);
    delay(500);
    reverse();
    analogWrite(PWM_A, 245);
    analogWrite(PWM_B, 245);
    delay(300);
    brake();
  }

  servo.write(90);
  delay(500);

  delay(10);
}

void goStraight()
{
  digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
  digitalWrite(BRAKE_B, LOW);
  digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
  digitalWrite(DIR_B, LOW);
}

void reverse()
{
  digitalWrite(BRAKE_A, LOW);
  digitalWrite(BRAKE_B, LOW);
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, HIGH);
}

void brake()
{
  digitalWrite(BRAKE_A, HIGH);
  digitalWrite(BRAKE_B, HIGH);
  delay(2000);
}

void turnLeft()
{
  digitalWrite(BRAKE_A, LOW);
  digitalWrite(BRAKE_B, LOW);
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, LOW);
}


void turnRight()
{
  digitalWrite(BRAKE_A, LOW);
  digitalWrite(BRAKE_B, LOW);
  digitalWrite(DIR_A, HIGH);
  digitalWrite(DIR_B, HIGH);
}

int pan()
{
  int pos;
  int dist;
  int maxVal = 0;
  servo.write(10);
  delay(1000);
  Serial.println("\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  for(int a = 10; a < 170; a += 2)
  {
    Serial.print("Read distance: ");
    dist = scanner.scan();
    Serial.print(dist);
    Serial.println(" cm");
    delay(10);
    servo.write(a);
    delay(20);
    if(dist > maxVal)
    {
      maxVal = dist;
      Serial.print("Max distance: " + dist);
      Serial.println(" cm");
      pos = a;
      Serial.println(pos + " degrees\n\n");
    }
  }
  delay(500);
  servo.write(pos);
  delay(500);
  Serial.print(maxVal);
  Serial.println(" cm");
  return pos;
}

void obstacleTone()
{
  for(int a = 0; a < sizeof(melody); a++)
  {
    tone(speakerPin, melody[a], melodyDuration);
    int pause = melodyDuration * 1.30;
    delay(pause);

    noTone(speakerPin);
  }
}





