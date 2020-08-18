#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int trigpin=10;
const int echopin=9;
const int LEDG=2;
const int sw=8;
const int buzzer=11;
int i=0;
int Time=0;
bool alarm=false;
long Duration;
int distance;
unsigned long HoldT;
unsigned long PressT;
unsigned long HeldT;
unsigned long DisarmStart;
unsigned long DisarmEnd;
unsigned long DisarmTime;
int val=0;
bool disarmed=false;
bool Sequence1=false;
bool Sequence3=false;
bool Sequence2=false;
int time1=0;
int time2=0;
void setup() {
 pinMode(trigpin,OUTPUT);
pinMode(echopin,INPUT);
pinMode(LEDG,OUTPUT);
pinMode(sw,INPUT);
pinMode(buzzer,OUTPUT);
Serial.begin(9600);
lcd.init();   
}
void loop() {
Serial.println(alarm);
digitalWrite(trigpin,LOW);
delay(2);
digitalWrite(trigpin,HIGH);
delay(10);
digitalWrite(trigpin,LOW);
Duration=pulseIn(echopin,HIGH);
distance=Duration*0.0343/2;
Serial.println(alarm);
Serial.println(distance);
lcd.backlight();
lcd.setCursor(0,0);
lcd.print(distance);
lcd.clear();
alarm=false;
HoldT=0;
PressT=0;
HeldT=0;
DisarmStart=0;
DisarmEnd=0;
DisarmTime=0;
 if(distance<=10)
  {
  alarm=true;
  DisarmStart=millis();
  while(DisarmTime<=20000)
  {
   disarmed=Disarming(DisarmStart);
      if(disarmed==true)
   {
    alarm=false;
    disarmed=false;
    break; 
   }
   DisarmEnd=millis();
   DisarmTime=DisarmEnd-DisarmStart;
   lcd.backlight();
   DisarmEnd=millis();
   DisarmTime=DisarmEnd-DisarmStart;
   time1=(DisarmTime/1000)+1;
   lcd.setCursor(0,0);
   lcd.print(time1);
   Serial.println(DisarmTime);
  }

    Serial.println(distance);
    Serial.println(alarm);
    if(alarm==false)
    {
      Alarmdisarmed();
    }
    else if(alarm==true)
    {
      while(alarm==true)
      {

        Alarmtriggered();
        if(digitalRead(sw)==HIGH)
        {
          alarm=Alarmreset();
          if(alarm==false)
         {  
          break;
         }
        }
      }
     }
  }   
}
bool Alarmreset()
{
    unsigned long HoldT=0;
    unsigned long PressT=0;
    unsigned long HeldT=0;
     bool alarm=true;
     int time2=0;
     PressT=millis();
     while(digitalRead(sw)==HIGH)
     {
        HoldT=millis();
        HeldT=HoldT-PressT;
        Serial.println(HeldT); 
        if(HeldT>=10000)
        {
          alarm=false;
          Serial.print("alarmReset");
          lcd.backlight();
          lcd.setCursor(0,0);
          lcd.print("Alarm Reset");
          digitalWrite(LEDG,LOW);
          digitalWrite(buzzer,LOW);
          delay(1000);
          lcd.clear();
          break;
        }
      time2=(HeldT/1000)+1;
      Serial.println(HeldT);
      lcd.backlight();
      lcd.setCursor(14,1);
      lcd.print(time2);
      }
      lcd.clear();
      return alarm;
}
void Alarmtriggered()
{
   digitalWrite(LEDG,HIGH);
  // digitalWrite(buzzer,HIGH);
   Serial.println(alarm);
   lcd.backlight();
   lcd.setCursor(0,0);
   lcd.print("Intruder!");
   Serial.print("Intruder!");
}
bool Alarmdisarmed()
{
  bool alarm;
    alarm=false;
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Disarmed");
    Serial.print("Disarmed");
    delay(1000);
    lcd.clear();
    return alarm;
}
bool Disarming(unsigned long DisarmStart)
{
  bool disarmed=false;
  int time1=0;
  int time2=0;
  unsigned long HoldT;
  unsigned long PressT;
  unsigned long HeldT;
  unsigned long DisarmTime;
  unsigned long DisarmEnd;
    if(digitalRead(sw)==HIGH)
   {
    PressT=millis();
    while(digitalRead(sw)==HIGH)
    {
      HoldT=millis();
      HeldT=HoldT-PressT;
      Serial.println(HeldT);
      if(HeldT>=5000)
      {
        disarmed=true;
        break;
      }
      DisarmEnd=millis();
      DisarmTime=DisarmEnd-DisarmStart;
      time1=(DisarmTime/1000)+1;
      time2=(HeldT/1000)+1;
      Serial.println(DisarmTime);
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print(time1);
      lcd.setCursor(14,1);
      lcd.print(time2);
      if(DisarmTime>=20000)
      {
        break;
      }
    }
    lcd.clear();
   }
   return disarmed;  
}
