#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <IRremoteESP8266.h>

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


/*
 16 D0 5 on
 5 D1 4 on
 4 D2 3 on
 0 D3 2 on
 2 D4 2 off
 14 D5 3 off
 12 D6 4 off
 13 D7 5 off
 15 D8 1 off
 TV E0E040BF
 */
 
 const int pin_0 = 0;
// const int threeOn = 4;
 const int pin_5 = 5;
 const int pin_16 = 16;
 const int pin_2 = 2;
// const int threeOff = 14;
 const int pin_12 = 12;
 const int pin_13 = 13;

void setup()
{


  
  pinMode(pin_2, OUTPUT);
//  pinMode(threeOff, OUTPUT);
  pinMode(pin_12, OUTPUT);
  pinMode(pin_13, OUTPUT);
  pinMode(pin_0, OUTPUT);
//  pinMode(threeOn, OUTPUT);
  pinMode(pin_5, OUTPUT);
  pinMode(pin_16, OUTPUT);

  digitalWrite(pin_2, 0);
//  digitalWrite(threeOff, 0);
  digitalWrite(pin_12, 0);
  digitalWrite(pin_13, 0);
  digitalWrite(pin_0, 0);
//  digitalWrite(threeOn, 0);
  digitalWrite(pin_5, 0);
  digitalWrite(pin_16, 0);
  
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

  digitalWrite(pin_16, 1);
  delay(100);
  digitalWrite(pin_16, 0);
  
  
  isswitch_1_On = true;
  return isswitch_1_On;
}

bool switch_1_Off() {

  digitalWrite(pin_13, 1);
  delay(100);
  digitalWrite(pin_13, 0);
  
  //Serial.println("Switch 2 turn off ...");
  isswitch_1_On = false;
  return isswitch_1_On;
}

bool switch_2_On() {
  digitalWrite(pin_0, 1);
  delay(100);
  digitalWrite(pin_0, 0);

  
  //Serial.println("Switch 3 turn on ...");
  
  isswitch_2_On = true;
  return isswitch_2_On;
}

bool switch_2_Off() {
  digitalWrite(pin_2, 1);
  delay(100);
  digitalWrite(pin_2, 0);

  
  //Serial.println("Switch 3 turn off ...");
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
