#include <DS1302RTC.h>
#include <Streaming.h>        //http://arduiniana.org/libraries/streaming/
#include <Time.h>             //http://playground.arduino.cc/Code/Time
// Set pins:  CE, IO,CLK
DS1302RTC RTC(27, 29, 31);
// Optional connection for RTC module
#define DS1302_GND_PIN 33
#define DS1302_VCC_PIN 35
#define ramFirstAddress 192
#define FF 255
time_t t;
tmElements_t tm;
char time[20];
int i;
char buf [8];
unsigned long delayTime;
char ramArray[31];
void setup(void)
{
  Serial.begin(9600);
  delay(100);
  // Activate RTC module
  digitalWrite(DS1302_GND_PIN, LOW);
  pinMode(DS1302_GND_PIN, OUTPUT);
  digitalWrite(DS1302_VCC_PIN, HIGH);
  pinMode(DS1302_VCC_PIN, OUTPUT);
  if (RTC.haltRTC()) {
    Serial << "The DS1302 is stopped." << endl << "Please, set the time by typing \"SET TIME\".";
    Serial << endl << endl;
  }    
  //setSyncProvider() causes the Time library to synchronize with the
  //external RTC by calling RTC.get() every five minutes by default.
  setSyncProvider(RTC.get); 
  printMenu();
}
void loop(){  
  if (Serial.available()){
    char buf = '\0';
    buf = Serial.read();
    if (buf == 's' || buf == 'S'){
       while(Serial.available())
         buf = Serial.read();
       Serial << "Set the date and time by entering the current date/time in" << endl;
       Serial << "the IDE serial monitor:" << endl << "  - must be in the 24 hour format, including commas" 
         << endl << endl;
       Serial << "YYYY,MM,DD,HH,DD,SS," << endl << endl
         << "(example: 2000,12,31,23,59,59, would be DEC 31st, 2000  11:59:59 pm)" << endl;
       Serial << "(must be typed in within 30 seconds or SET function times out)" << endl << endl;
       while(Serial.available())
         buf = Serial.read();
       setTimeFunction();
       printMenu();
    }
    else if (buf == 't' || buf == 'T'){
      t = now();
      printDateTime(t);
      Serial << endl << endl;
    }
    else if (buf == 'w' || buf == 'W'){
      Serial.println();
      Serial.println("writing data");
      delay(200);
      Serial.println("Done");
      RTC.writeEN(true);
      int j = 0;
      for(int i = 0; i < 62; i+=2){
        RTC.writeRTC((ramFirstAddress+i), 100+j);
        j++;
      }
      Serial.println();
    }
    else if (buf == 'r' || buf == 'R'){
      for(int i = 0; i < 62; i+=2){
        Serial.print("address :  ");
        Serial.print(ramFirstAddress+i, HEX);
        Serial.print("  contains :  ");
        Serial.print(RTC.readRTC(ramFirstAddress+i));
        Serial.println();
      }
      Serial.println();
      Serial.println("end of RAM read");
    }
    else if (buf == 'h' || buf == 'H'){
      RTC.writeRTC(DS1302_TRICKLE, B10100111);
      Serial << "Trickle Charger ENABLED (HIGH): 1 diodes, 8kohm" << endl << endl;
    }
    else if (buf == 'l' || buf == 'L'){
      RTC.writeRTC(DS1302_TRICKLE, B10101011);
      Serial << "Trickle Charger ENABLED (LOW): 2 diodes, 8kohm" << endl << endl;
    }
    else if (buf == 'o' || buf == 'O'){
      RTC.writeRTC(DS1302_TRICKLE, B01011100);
      Serial << "Trickle Charger DISABLED (OFF)" << endl << endl;
    }    
    while(Serial.available())
      buf = Serial.read();
  }     
}
void printMenu(){
  Serial.println("  *************************************************");
  Serial.println("****  DS1302 CLOCK, RAM, TRICKLE CHARGE PROGRAM  ****");
  Serial.println("  *************************************************");
  Serial.println();
  Serial.println("type  \"TIME\"    to view current time");  
  Serial.println("type  \"SET\"     to set the time");
  Serial.println("type  \"WRITE\"   to write to RAM");
  Serial.println("type  \"READ\"    to read from RAM");
  Serial.println("type  \"HIGH\"    to turn ON high current trickle charger");
  Serial.println("type  \"LOW\"     to turn ON low current trickle charger");
  Serial.println("type  \"OFF\"     to turn OFF low current trickle charger");
  Serial << endl << endl << endl;
}
void setTimeFunction(){
  delayTime = millis() + 45000UL;
  while (delayTime >= millis() && !Serial.available()) {
    delay(10);
  }
  if (Serial.available()) {
        //note that the tmElements_t Year member is an offset from 1970,
        //but the RTC wants the last two digits of the calendar year.
        //use the convenience macros from Time.h to do the conversions.
            int y = Serial.parseInt();
            tm.Year = CalendarYrToTm(y);
            tm.Month = Serial.parseInt();
            tm.Day = Serial.parseInt();
            tm.Hour = Serial.parseInt();
            tm.Minute = Serial.parseInt();
            tm.Second = Serial.parseInt();
            t = makeTime(tm);
     //use the time_t value to ensure correct weekday is set
            if(RTC.set(t) == 0) { // Success
              setTime(t);
              Serial << "RTC set to: ";
              Serial << endl << endl;
              printDateTime(t);
              Serial << endl << endl;
              Serial << endl;
     }
     else
       Serial << "RTC set failed!" << endl;
            //dump any extraneous input
            while (Serial.available() > 0) Serial.read();
  }
  else 
    Serial << "timed out, please try again" << endl << endl;
}
void printDateTime(time_t t)
{
    printDate(t);
    Serial << ' ';
    printTime(t);
}
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}
void printDate(time_t t)
{
    Serial << "DATE/TIME: ";    
    Serial << monthShortStr(month(t)) << ", ";
    printI00(day(t), 0);
    Serial << ", " << _DEC(year(t)) << " ";
}
void printI00(int val, char delim)
{
    if (val < 10) Serial << '0';
    Serial << _DEC(val);
    if (delim > 0) Serial << delim;
    return;
}
