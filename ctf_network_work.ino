/********************
  Relays oooooooooooooooooooooyeeeeeeeeeeeeeeeeee
*********************/
#define RELAY1  22                        
#define RELAY2  24    

void initRelays(){
  pinMode(RELAY1, OUTPUT);       
  pinMode(RELAY2, OUTPUT);
}

void onRelay1(){
   digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
}
void onRelay2(){
   digitalWrite(RELAY2,LOW);           // Turns ON Relays 1
}
void offRelay1(){                                    
   digitalWrite(RELAY1,HIGH);          // Turns Relay Off
}
void offRelay2(){                                    
   digitalWrite(RELAY2,HIGH);          // Turns Relay Off
}
/********************
  DHT 11
*********************/

//DHT Library
#include <dht.h>
#define dhtPin A8     // what pin we're connected to 
dht DHT;
int dhtHum = 0;
int dhtTemp = 0;

/********************
  Soil Moisture
*********************/

//Define the moisture sensor pins
const int moistPin1 = A2;  
//the value readed from each moisture sensor
int moistVal1 = 0;
//the sum of the 30 times sampling
long int moistSum1 = 0;   //we need to sampling the moisture 30 times and get its average value

/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client0;
//IPAddress ip(192,168,1,86);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void initNetwork(){
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
  }
  else{
     // start the Ethernet connection and the server:
    //Ethernet.begin(mac, ip);
    Ethernet.begin(mac, Ethernet.localIP());
    server.begin();
    Serial.print("Server @ ");
    Serial.println(Ethernet.localIP());
  }
}

/********************
  Soil Moisture
*********************/
void readSoilMoisture(){
  int i;
  for(i = 0; i < 30; i++){//sampling 30 time within 3 seconds
    moistSum1 = moistSum1 + analogRead(moistPin1);  
    delay(100);
  }
  moistVal1 = moistSum1 / 30;//get the average value
 // for(int i = 0; i < 3; i ++){
    Serial.print("Soil "); 
    Serial.print("1"); 
    Serial.print(" : ");
    Serial.print(moistVal1);
    Serial.println(" % ");
  //}
  
    
  
  if(moistVal1 > 650){
    onRelay1();
  }
  else{
    offRelay1();
  
  moistSum1 = 0;  
}

void checkInternetClients(){
 
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  initNetwork();
  
 
}


void loop() {
   checkEthernetClients();
}

