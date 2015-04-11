#ifndef Scanner_h
#define Scanner_h

#include "Arduino.h"

using namespace std;

class Scanner
{
private:
  int greenPin; // 10
  int bluePin; // 2
  int redPin; // 6
  int pingPin; // 4
  int echoPin; //5

public:
  Scanner();
  int scan();
  int convertToCm(int);
  void triggerLED(int);
};



#endif


