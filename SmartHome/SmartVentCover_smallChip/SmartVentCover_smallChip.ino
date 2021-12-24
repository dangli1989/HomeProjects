#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <IRremoteESP8266.h>
#include <Servo.h>

// prototypes
boolean connectWifi();

// Change this before you flash
const char* ssid = "H2SO4";
const char* password = "meiyoumima";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *switch_1= NULL;
Switch *switch_2= NULL;

bool isswitch_1_On = false;
bool isswitch_2_On = false;

//servo setup
Servo servo;
int angle = 0;

void setup()
{
  pinMode(0, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  servo.attach(0);
  servo.write(angle);
  
  Serial.begin(115200);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    switch_1= new Switch("Switch_1", 81, switch_1_On, switch_1_Off);
    switch_2= new Switch("Switch_2 ", 82, switch_2_On, switch_2_Off);



    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*switch_1);
    upnpBroadcastResponder.addDevice(*switch_2);

  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();

      switch_2->serverLoop();
      switch_1->serverLoop();
	 }
}



bool switch_1_On() {
  for(angle = 10; angle < 180; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 
  
  isswitch_1_On = true;
  return isswitch_1_On;
}

bool switch_1_Off() {
  for(angle = 180; angle > 10; angle--)    
  {                                
    servo.write(angle);           
    delay(15);       
  } 

  
  isswitch_1_On = false;
  return isswitch_1_On;
}

bool switch_2_On() {


  isswitch_2_On = true;
  return isswitch_2_On;
}

bool switch_2_Off() {


  isswitch_2_On = false;
  return isswitch_2_On;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
