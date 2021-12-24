#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"


// prototypes
boolean connectWifi();

//on/off callbacks 
bool officeLightsOn();
bool officeLightsOff();
bool kitchenLightsOn();
bool kitchenLightsOff();
bool studioLightsOn();
bool studioLightsOff();
bool bedRoomLightsOn();
bool bedRoomLightsOff();

// Change this before you flash
const char* ssid = "H2SO4";
const char* password = "meiyoumima";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *allLights = NULL;
Switch *livingRoom= NULL;
Switch *studio= NULL;
Switch *bedRoom= NULL;


bool isAllLightsOn = false;
bool isLivingRoomLightsOn = false;
bool isStudioLightsOn = false;
bool isBedRoomLightsOn = false;


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
 
 const int twoOn = 0;
 const int threeOn = 4;
 const int fourOn = 5;
 const int fiveOn = 16;
 const int twoOff = 2;
 const int threeOff = 14;
 const int fourOff = 12;
 const int fiveOff = 13;

void setup()
{

  pinMode(twoOff, OUTPUT);
  pinMode(threeOff, OUTPUT);
  pinMode(fourOff, OUTPUT);
  pinMode(fiveOff, OUTPUT);
  pinMode(twoOn, OUTPUT);
  pinMode(threeOn, OUTPUT);
  pinMode(fourOn, OUTPUT);
  pinMode(fiveOn, OUTPUT);

  digitalWrite(twoOff, 0);
  digitalWrite(threeOff, 0);
  digitalWrite(fourOff, 0);
  digitalWrite(fiveOff, 0);
  digitalWrite(twoOn, 0);
  digitalWrite(threeOn, 0);
  digitalWrite(fourOn, 0);
  digitalWrite(fiveOn, 0);
  
  Serial.begin(9600);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    allLights = new Switch("all lights", 80, allLightsOn, allLightsOff);
    livingRoom= new Switch("living room lights", 81, livingRoomLightsOn, livingRoomLightsOff);
    studio= new Switch("studio lights", 82, studioLightsOn, studioLightsOff);
    bedRoom= new Switch("bed room lights", 83, bedRoomLightsOn, bedRoomLightsOff);


    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*allLights);
    upnpBroadcastResponder.addDevice(*livingRoom);
    upnpBroadcastResponder.addDevice(*studio);
    upnpBroadcastResponder.addDevice(*bedRoom);

  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      allLights->serverLoop();
      studio->serverLoop();
      livingRoom->serverLoop();
      bedRoom->serverLoop();
	 }
}

bool allLightsOn() {
  digitalWrite(twoOn, 1);
  delay(100);
  digitalWrite(twoOn, 0);
  digitalWrite(threeOn, 1);
  delay(100);
  digitalWrite(threeOn, 0);
  digitalWrite(fourOn, 1);
  delay(100);
  digitalWrite(fourOn, 0);
  digitalWrite(fiveOn, 1);
  delay(100);
  digitalWrite(fiveOn, 0);
  
  Serial.println("Switch 1 turn on ...");
  
  isAllLightsOn = true;
  return isAllLightsOn;
}

bool allLightsOff() {
  digitalWrite(twoOff, 1);
  delay(100);
  digitalWrite(twoOff, 0);
  digitalWrite(threeOff, 1);
  delay(100);
  digitalWrite(threeOff, 0);
  digitalWrite(fourOff, 1);
  delay(100);
  digitalWrite(fourOff, 0);
  digitalWrite(fiveOff, 1);
  delay(100);
  digitalWrite(fiveOff, 0);
  
  Serial.println("Switch 1 turn off ...");
  isAllLightsOn = false;
  return isAllLightsOn;
}

bool livingRoomLightsOn() {

  digitalWrite(fiveOn, 1);
  delay(100);
  digitalWrite(fiveOn, 0);
  
  Serial.println("Switch 2 turn on ...");
  
  isLivingRoomLightsOn = true;
  return isLivingRoomLightsOn;
}

bool livingRoomLightsOff() {

  digitalWrite(fiveOff, 1);
  delay(100);
  digitalWrite(fiveOff, 0);
  
  Serial.println("Switch 2 turn off ...");
  isLivingRoomLightsOn = false;
  return isLivingRoomLightsOn;
}

bool studioLightsOn() {
  digitalWrite(twoOn, 1);
  delay(100);
  digitalWrite(twoOn, 0);

  
  Serial.println("Switch 3 turn on ...");
  
  isStudioLightsOn = true;
  return isStudioLightsOn;
}

bool studioLightsOff() {
  digitalWrite(twoOff, 1);
  delay(100);
  digitalWrite(twoOff, 0);

  
  Serial.println("Switch 3 turn off ...");
  isStudioLightsOn = false;
  return isStudioLightsOn;
}

bool bedRoomLightsOn() {
  digitalWrite(threeOn, 1);
  delay(100);
  digitalWrite(threeOn, 0);

  
  Serial.println("Switch 4 turn on ...");
  
  isBedRoomLightsOn = true;
  return isBedRoomLightsOn;
}

bool bedRoomLightsOff() {
  digitalWrite(threeOff, 1);
  delay(100);
  digitalWrite(threeOff, 0);

  
  Serial.println("Switch 4 turn off ...");
  isBedRoomLightsOn = false;
  return isBedRoomLightsOn;
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
    if (i > 10){
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
