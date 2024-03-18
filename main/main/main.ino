//AirClenz main controller PCB(V1.0) firmware
//Author: Christopher Pederson
//Date: 2024-03-18

//Version Control
// 2024-03-18 (V1.0) - Initial release

//Include libraries
#include <PWM.h>

//Define constants
int Fan_Pin_Speed_Lo = 3;
int Fan_Pin_Speed_Md = 4;
int Fan_Pin_Speed_Hi = 5;

int Fan_Pin_Time_90 = 6;
int Fan_Pin_Time_60 = 7;
int Fan_Pin_Time_30 = 8;

int Fan_Pin_Speed_Btn = 2;
int Fan_Pin_Time_Btn = 9;
int Fan_Pin_Pwr_Btn = 11;

bool inputs_permitted = true;// prevents users from being able to perform multiple inputs or hold down the input

int Fan_Pin_PWM = 10;
int32_t PWM_Frequency = 20000;
bool PWR_On = false;// determines if inputs are unlocked

  bool
check_if_high(pin)
  {
  
  }
    void 
setup()
    {
    //Set pin modes
    pinMode(Fan_Pin_Speed_Lo, OUTPUT);
    pinMode(Fan_Pin_Speed_Md, OUTPUT);
    pinMode(Fan_Pin_Speed_Hi, OUTPUT);
    pinMode(Fan_Pin_Time_90, OUTPUT);
    pinMode(Fan_Pin_Time_60, OUTPUT);
    pinMode(Fan_Pin_Time_30, OUTPUT);
    pinMode(Fan_Pin_Speed_Btn, INPUT);
    pinMode(Fan_Pin_Time_Btn, INPUT);
    pinMode(Fan_Pin_Pwr_Btn, INPUT);
    pinMode(Fan_Pin_PWM, OUTPUT);

    //Set PWM frequency
    InitTimersSafe();
    SetPinFrequencySafe(Fan_Pin_PWM, PWM_Frequency);
    }

    void 
loop()
    {
      

      if(PWR_On)
        {
        //main body of code
        }
    }