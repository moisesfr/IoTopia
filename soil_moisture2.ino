int sensorPin0 = 0; 
int sensorPin1 = 1; // select the input pin for the potentiometer
int sensorValue0 = 0;  // variable to store the value coming from the sensor
int sensorValue1 = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
   Serial.begin(9600);  
}
 
void loop() {
  // read the value from the sensor:
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);  
  
  delay(1000);          
  
  Serial.print("Soil Moisture 1 = " );                       
  Serial.print(sensorValue0);
  Serial.print("\t Soil Moisture 2 = " );                       
  Serial.print(sensorValue1);  
  Serial.println();
}

