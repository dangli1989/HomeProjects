#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "H2SO4";
const char* password = "meiyoumima";

ESP8266WebServer server(80);
/*
 16 D0 5 on
 5 D1 4 on
 4 D2 3 on
 0 D3 2 on
 2 D4 2 off
 14 D5 3 off
 12 D6 4 off
 13 D7 5 off
 15 D8 1 off*/
const int light1off = 15;
const int light3onoffoffoff = 13;
const int light3ononoff = 14;
const int light3onoffoff = 12;
const int light3onoff = 2;
const int light3onon = 0;  
const int light3on = 4;
const int light1 = 5;
const int light0 = 16;

void handleRoot() {
  /*digitalWrite(light1off, 1);
  delay(10);
  //digitalWrite(light1off, 0);
  digitalWrite(light3onoffoffoff, 1);
  delay(10);
  //digitalWrite(light3onoffoffoff, 0);
  digitalWrite(light3ononoff, 1);
  delay(10);
  //digitalWrite(light3ononoff, 0);
  digitalWrite(light3onoffoff, 1);
  delay(10);
  //digitalWrite(light3onoffoff, 0);
  digitalWrite(light3onoff, 1);
  delay(10);
  //digitalWrite(light3onoff, 0);
  digitalWrite(light3onon, 1);
  delay(10);
  //digitalWrite(light3onon, 0);
  digitalWrite(light3on, 1);
  delay(10);
  //digitalWrite(light3on, 0);
  digitalWrite(light1, 1);
  delay(10);
  //digitalWrite(light1, 0);
  digitalWrite(light0, 1);
  delay(100);
  digitalWrite(light0, 0);*/
  server.send(200, "text/plain", "hello from esp8266!");

}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  switch(server.uri()[1]){
    case 'a':
      digitalWrite(light0, 1);
      delay(100);
      digitalWrite(light0, 0);
      digitalWrite(light1, 1);
      delay(100);
      digitalWrite(light1, 0);
      digitalWrite(light3on, 1);
      delay(100);
      digitalWrite(light3on, 0);
      digitalWrite(light3onon, 1);
      delay(100);
      digitalWrite(light3onon, 0);
      break;
    case '1':
      Serial.println(1);
      digitalWrite(light0, 1);
      delay(100);
      digitalWrite(light0, 0);
      break;
    case '2':
      Serial.println(2);
      digitalWrite(light1, 1);
      delay(100);
      digitalWrite(light1, 0);
      break;
    case '3':
      Serial.println(3);
      digitalWrite(light3on, 1);
      delay(100);
      digitalWrite(light2, 0);
      break;
    case '4':
      Serial.println(4);
      digitalWrite(light2on, 1);
      delay(100);
      digitalWrite(light2on, 0);
      break;
    case '5':
      Serial.println(5);
      digitalWrite(light2off, 1);
      delay(100);
      digitalWrite(light2off, 0);
      break;
    case '6':
      Serial.println(6);
      digitalWrite(light2offoff, 1);
      delay(100);
      digitalWrite(light2offoff, 0);
      break;
    case '7':
      Serial.println(7);
      digitalWrite(light2onoff, 1);
      delay(100);
      digitalWrite(light2onoff, 0);
      break;
    case '8':
      Serial.println(8);
      digitalWrite(light2offoffoff, 1);
      delay(100);
      digitalWrite(light2offoffoff, 0);
      break;

    default:
      digitalWrite(light2off, 1);
      delay(100);
      digitalWrite(light2off, 0);
      digitalWrite(light2offoff, 1);
      delay(100);
      digitalWrite(light2offoff, 0);
      digitalWrite(light2onoff, 1);
      delay(100);
      digitalWrite(light2onoff, 0);
      digitalWrite(light2offoffoff, 1);
      delay(100);
      digitalWrite(light2offoffoff, 0);
      Serial.println("Wrong Input!");
/*    digitalWrite(light1off, 0);
    digitalWrite(light2offoffoff, 0);
    digitalWrite(light2onoff, 0);
    digitalWrite(light2offoff, 0);
    digitalWrite(light2off, 0);
    digitalWrite(light2on, 0);
    digitalWrite(light2, 0);
    digitalWrite(light1, 0);
    digitalWrite(light0, 0);*/
  }

}

void setup(void){
  pinMode(light1off, OUTPUT);
  pinMode(light2offoffoff, OUTPUT);
  pinMode(light2onoff, OUTPUT);
  pinMode(light2offoff, OUTPUT);
  pinMode(light2off, OUTPUT);
  pinMode(light2on, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light1, OUTPUT);
  pinMode(light0, OUTPUT);
  digitalWrite(light2offoffoff, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}