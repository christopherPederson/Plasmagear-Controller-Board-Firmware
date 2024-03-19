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

//Define utility constants
const int32_t PWM_Frequency = 20000;// set fan control PWM frequency to 20KHz
const int Speed_Value_Ary[] = {0,77,179,255};// used to set PWM duty cycle
const int Speed_LED_Ary[] = {Fan_Pin_Speed_Lo,Fan_Pin_Speed_Md,Fan_Pin_Speed_Hi};// pins for fan speed leds
const long Time_Value_Ary[] = {0,1800000,3600000,5400000};// utility values for timer settings
const int Time_LED_Ary[] = {Fan_Pin_Time_30,Fan_Pin_Time_60,Fan_Pin_Time_90};// pins for timer leds

//Define global variables 
bool Inputs_Permitted = true;// prevents users from being able to perform multiple inputs or hold down the input
bool PWR_On = false;// determines if inputs are unlocked
long Time_Duration_Start = 0; // this will be changed when timer is activated to be equal to the run time of the program. 
int Speed_Display_Value = 0; //tracks desired speed value
int Time_Display_Value = 0; //tracks desired timer value

// Utility function declerations
  void
set_time_leds()//manage time LEDs
  {
  // Set all pins to HIGH
  for(int i = 0; i < sizeof(Time_LED_Ary) / sizeof(Time_LED_Ary[0]); i++) 
    {
    digitalWrite(Time_LED_Ary[i], HIGH);
    }
  
  // Set pins corresponding to time_Display_Value to LOW
  for(int i = 0; i < Time_Display_Value; i++) 
    {
    digitalWrite(Time_LED_Ary[i], LOW);
    }
  }

  void 
set_speed_leds()//manage speed LEDs
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
manage_timer()
  {
    if(Time_Duration_Start + Time_Value_Ary[Time_Display_Value] < millis() && Time_Display_Value != 0)
      {
      // checks if timer is running and if the timer has exceeded set time 
      //powers off the device
      handle_PWR_press();
      }
  }

  void
set_timer()
  {
  Time_Duration_Start = millis(); //sets start time to current value
  if(Time_Display_Value > 3) // if the desired time value exceeds 3 it will be set back to 0 
    {
    Time_Display_Value = 0;
    }
  set_time_leds();
  manage_timer();
  }

  void
set_PWM()
  {
  pwmWrite(Fan_Pin_PWM, Speed_Value_Ary[3]);// the fans require an initial start jump
  delay(100);
  pwmWrite(Fan_Pin_PWM, Speed_Value_Ary[Speed_Display_Value]);// sets PWM to one of the preset values
  }

  void
set_speed()
  {
  if(Speed_Display_Value > 3)// loops desired value
    {
    Speed_Display_Value = 1;
    }
  set_speed_leds();
  set_PWM();
  }

  int
increment_time()
  {
  Time_Display_Value++;
  set_timer();
  }

  int
increment_speed()
  {
    Speed_Display_Value++;
    set_speed();
  }

  void
handle_btn_press(void(*function)())//pass desired function pointer 
  {
  if (Inputs_Permitted && PWR_On)
    {
    Inputs_Permitted = false;// prevents multiple or repeated inputs 
    function();
    delay(150);
    }
  }

  void
handle_PWR_press()
  {
  if (Inputs_Permitted)
    {
    Speed_Display_Value = PWR_On ? 0 : 1; //when powered on the device should start on the lowest speed setting
    Time_Display_Value = 0; // starts with no timer
    set_speed();
    set_timer();
    Inputs_Permitted = false; // prevents multiple or repeated inputs 
    PWR_On = !PWR_On; //toggle power state 
    delay(150);
    }
  }

  int
check_input()//looks for active pins
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
set_leds_high()
  {
  for (int i = 3; i < 9; i++)
      {
      digitalWrite(i, HIGH);
      }
  }

// Runtime functions (setup and loop)
  void 
setup()
    {
    set_leds_high(); //set all LEDs to LOW (check schematic for more info)
    Serial.begin(9600);

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
        default:
        case 0: //No Input
          {
          manage_timer();
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