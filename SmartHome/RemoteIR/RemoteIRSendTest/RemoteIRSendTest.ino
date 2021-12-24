/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

IRsend irsend;

void setup()
{
  //Serial.begin(9600);
  irsend.sendRaw(0xE0E040BF, 32, 52);
}

void loop() {
irsend.sendRaw(0xE0E040BF, 32, 52);
delay(10000);
}
