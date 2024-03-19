//AirClenz main controller PCB(V1.0) firmware
//Author: Christopher Pederson
//Date: 2024-03-18

//Version Control
// 2024-03-18 (V1.0) - Initial release

//Include libraries
#include <PWM.h>

//Define constants
const int Fan_Pin_Speed_Lo = 3;
const int Fan_Pin_Speed_Md = 4;
const int Fan_Pin_Speed_Hi = 5;

const int Fan_Pin_Time_90 = 6;
const int Fan_Pin_Time_60 = 7;
const int Fan_Pin_Time_30 = 8;

const int Fan_Pin_Speed_Btn = 2;
const int Fan_Pin_Time_Btn = 9;
const int Fan_Pin_PWR_Btn = 11;

const int Fan_Pin_PWM = 10;
const int32_t PWM_Frequency = 20000;// set fan control PWM frequency to 20KHz

bool Inputs_Permitted = true;// prevents users from being able to perform multiple inputs or hold down the input

bool PWR_On = false;// determines if inputs are unlocked

const long Thirty_Min_Millis = 1800000;
long Time_Duration = 0;
long Time_Duration_Start = 0; 

int Speed_Display_Value = 0;
int Time_Display_Value = 0;
int Speed_Value_Ary[] = {0,77,179,255};
int Speed_LED_Ary[] = {Fan_Pin_Speed_Lo,Fan_Pin_Speed_Md,Fan_Pin_Speed_Hi};

  void 
set_speed_leds() 
  {
  // Set all pins to HIGH
  for(int i = 0; i < sizeof(Speed_LED_Ary) / sizeof(Speed_LED_Ary[0]); i++) 
    {
    digitalWrite(Speed_LED_Ary[i], HIGH);
    }
  
  // Set pins corresponding to Speed_Display_Value to LOW
  for(int i = 0; i < Speed_Display_Value; i++) 
    {
    digitalWrite(Speed_LED_Ary[i], LOW);
    }
  }

  void
set_PWM()
  {
  pwmWrite(Fan_Pin_PWM, Speed_Value_Ary[3]);
  delay(100);
  pwmWrite(Fan_Pin_PWM, Speed_Value_Ary[Speed_Display_Value]);
  }

  void
set_speed()
  {
  if(Speed_Display_Value > 3)
    {
    Speed_Display_Value = 1;
    }
  Serial.println(Speed_Display_Value);
  set_speed_leds();
  set_PWM();
  }

  int
increment_time()
  {
  Serial.println("time");
  }

  int
increment_speed()
  {
    Speed_Display_Value++;
    set_speed();
  }

  void
handle_btn_press(void(*function)())
  {
  if (Inputs_Permitted && PWR_On)
    {
    Inputs_Permitted = false;
    function();
    delay(150);
    }
  }

  void
handle_PWR_press()
  {
  if (Inputs_Permitted)
    {
    Speed_Display_Value = PWR_On ? 0 : 1;
    set_speed();
    Inputs_Permitted = false;
    PWR_On = !PWR_On;
    delay(150);
    }
  }

  int
check_input()
  {
  if (digitalRead(Fan_Pin_PWR_Btn) == HIGH)
      {
      return Fan_Pin_PWR_Btn;
      }
  else if (digitalRead(Fan_Pin_Time_Btn) == HIGH)
      {
      return Fan_Pin_Time_Btn;
      }
  else if (digitalRead(Fan_Pin_Speed_Btn) == HIGH)
      {
      return Fan_Pin_Speed_Btn;
      }
  return 0;
  }
  void 
setup()
    {
    Serial.begin(9600);

    //set all LEDs to LOW (check schematic for more info)
    for (int i = 3; i < 9; i++)
    {
    digitalWrite(i, HIGH);
    }

    //Set pin modes
    pinMode(Fan_Pin_Speed_Lo, OUTPUT);
    pinMode(Fan_Pin_Speed_Md, OUTPUT);
    pinMode(Fan_Pin_Speed_Hi, OUTPUT);
    pinMode(Fan_Pin_Time_90, OUTPUT);
    pinMode(Fan_Pin_Time_60, OUTPUT);
    pinMode(Fan_Pin_Time_30, OUTPUT);
    pinMode(Fan_Pin_Speed_Btn, INPUT);
    pinMode(Fan_Pin_Time_Btn, INPUT);
    pinMode(Fan_Pin_PWR_Btn, INPUT);
    pinMode(Fan_Pin_PWM, OUTPUT);

    //Set PWM frequency
    InitTimersSafe();
    SetPinFrequencySafe(Fan_Pin_PWM, PWM_Frequency);
    }

    void 
loop()
    {
      switch(check_input())
        {
        case 0: //No Input
          {
          Inputs_Permitted = true;
          break;
          }
        case 11: //Power on
          {
          handle_PWR_press();
          }
        case 9: //Time
          {
          handle_btn_press(increment_time);
          break;
          }
        case 2: //Speed
          {
          handle_btn_press(increment_speed);
          break;
          }
        }
    }