/********************
  DHT 11
*********************/

//DHT Library
#include <dht.h>
#define dhtPin A1     // what pin we're connected to 
dht DHT;
int dhtHum = 0;
int dhtTemp = 0;

/********************
  LCD KeyPad
*********************/

//using LiquidCrystal library
#include <LiquidCrystal.h>
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// define some values used by the panel and buttons
#define LCD_BACKLIGHT_PIN 10
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
//define lcd text messages
//char text[];
//char airTemp[ ] = "Temperature";
//char airHum[ ] = "Humidity";
//define vars
int lcdOn = 0;
int menuPos = 1;
//int menuIndex[] = {0,1};

/********************
  Soil Moisture
*********************/

//Define the moisture sensor pins
const int moistPin1 = A2;  
//the value readed from each moisture sensor
int moistVal1 = 0;
//the sum of the 30 times sampling
long int moistureSum0 = 0;   //we need to sampling the moisture 30 times and get its average value

/********************
  LCD KeyPad
*********************/

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}

void toggleLight(){
  if(lcdOn == 1){
     lcd.noDisplay();
     // Turn off the LCD backlight.
      digitalWrite(LCD_BACKLIGHT_PIN, LOW);
     lcdOn = 0;
   }
   else{
     lcd.display();
     // Turn on the LCD backlight.
      digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
     lcdOn = 1;       
   }
}


void initMenu(){
  int menuPos[] = {2, 4, 8, 3, 6};
}

void initLcd(){
  // Set the LCD display backlight pin as an output.
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
   // Turn on the LCD backlight.
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
  lcd.begin(16, 2);              // start the library
  resetLcd();
}
void resetLcd(){
  lcd.clear();
  lcd.setCursor(0,0);
}
/*
void lcdShow(){
  resetLcd();
  lcd.print();
  delay(300);
  lcd.noDisplay();
  delay(300);
  lcd.display();
  delay(300);
  lcd.noDisplay();
  delay(300);
  lcd.display();
  delay(300);
}
*/
void nextView(){
  /*int i;
  for (i = 0; i < 2; i ++) {
    
  }*/
  resetLcd();
  if(menuPos == 0)
    menuPos = 1;
  else
    menuPos = 0;
    
  switch (menuPos)               
 {
   case 0:
     {
       lcd.print("IoG v1.0"); // print a simple message     
     break;
     }
   case 1:
     {
       readDht11();
     break;
     }
 /*  case 2:
     {
      
     break;
     }
   case 3:
     {
      
     break;
     }
   case 4:
     {
      
     break;
     }*/
   }
}

void timeCount(){
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print(millis()/1000);      // display seconds elapsed since power-up

 //lcd.setCursor(0,1);            // move to the begining of the second line
}


 
/********************
  DHT 11
*********************/
void readDht11(){
  
  DHT.read11(dhtPin);
  dhtHum = DHT.humidity;
  dhtTemp = DHT.temperature;
  
  Serial.print("Temperature : "); 
  Serial.print(dhtTemp); 
  Serial.print(" - Humidity : ");
  Serial.println(dhtHum);
  
  if(menuPos == 1){
        lcd.setCursor(0,0);            // move to the begining of the second line
        lcd.print("Air Temp: ");
        lcd.print(dhtTemp);
        lcd.print(" C");
        lcd.setCursor(0,1);
        lcd.print("Air Hum: ");
        lcd.print(dhtHum);
        lcd.print(" %");
  }
  
  delay(2000);//Wait 2 seconds before accessing sensor again.
  
}
/********************
  Soil Moisture
*********************/
void initSoilMoist(){

}

/********************
  Serial
*********************/
void initSerial(){
  Serial.begin(9600);
}
/********************
  Main
*********************/
void setup(){
  initLcd();
  initSerial();
  //delay(500);//Delay to let system boot
  //msg = "System Started OK...";
  //msg(msg);
  lcd.print("System OK...");
  delay(1000);//Wait before accessing Sensor
  nextView();
}

void loop()
{
  readDht11(); // read DHT11
  lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
       resetLcd();
//       lcd.setCursor(0,1);            // move to the begining of the second line
       lcd.print("Temp: ");
       lcd.print(dhtTemp);
       lcd.print(" C");
       lcd.setCursor(0,1);
       lcd.print("Hum: ");
       lcd.print(dhtHum);
       lcd.print(" %");
       menuPos = 1;
     break;
     }
   case btnLEFT:
     {
       timeCount();
     break;
     }
   case btnUP:
     { 
       
  /*     resetLcd();
       lcd.print(menuPos);
       
       if(menuPos < 2){
         menuPos ++;
       }
       else
         menuPos = 0;
       */
       nextView();
     break;
     }
   case btnDOWN:
     {
       menuPos = 0;
       lcd.setCursor(0,0);   
       lcd.print("IoG v1.0"); // print a simple message     
     //lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
       toggleLight();
     //lcd.print("SELECT");
     break;
     }
   case btnNONE:
     {
     //lcd.print("NONE  ");
     break;
     }
 }

}
