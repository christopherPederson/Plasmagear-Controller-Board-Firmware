// AirShade main controller PCB(V1.0) firmware
// Author: Christopher Pederson
// Date: 2024-04-02

// Version Control
// 2024-04-02 (V1.0) ~ Initial release

// Include libraries
#include <PWM.h>  // can't use built-in PWM functionality as we need to change the frequency

//Define constants
#define Fan_Pin_PWM         10
#define Auth_LED_Pin        13
#define Programing_Auth_Pin 14
#define PWM_Frequency       20000     //fan control PWM frequency to 20KHz (trust me bro)

// Utility function declarations
  void
blink_auth_indicator()
  {
  digitalWrite(Auth_LED_Pin, LOW);
  delay(150);
  digitalWrite(Auth_LED_Pin, HIGH);
  delay(150);
  }


// Runtime functions (setup and loop)
  void 
setup()
  {
  Serial.begin(9600);
  //
  // Set pin modes
  pinMode(Fan_Pin_PWM, OUTPUT);
  pinMode(Programing_Auth_Pin, INPUT);
  pinMode(Auth_LED_Pin, OUTPUT);
  //
  // Set PWM frequency
  InitTimersSafe();
  SetPinFrequencySafe(Fan_Pin_PWM, PWM_Frequency);
  pwmWrite(Fan_Pin_PWM, 255); 
  }

  void 
loop()
  {
    if (digitalRead(Programing_Auth_Pin) == HIGH)
        blink_auth_indicator();
  }