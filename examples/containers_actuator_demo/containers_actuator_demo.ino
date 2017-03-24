/*
   Copyright 2015-2017 AllThingsTalk

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

/****
 *  AllThingsTalk Developer Cloud IoT experiment for LoRa
 *  Version 1.0 dd 22/3/2017
 *  Original author: Jan Bogaerts 2015
 *
 *  This sketch is part of the AllThingsTalk LoRa rapid development kit
 *  -> http://www.allthingstalk.com/lora-rapid-development-kit
 *
 *  This example sketch is based on the Proxilmus IoT network in Belgium
 *  The sketch and libs included support the
 *  - MicroChip RN2483 LoRa module
 * 
 *  THIS SKETCH IS FOR THE SODAQ ONE BOARD
 *  
 *  For more information, please check our documentation
 *  -> http://allthingstalk.com/docs/tutorials/lora/setup
 *  
 **/

//#include <Wire.h>
#include <ATT_IOT_LoRaWAN.h>
#include "keys.h"
#include <MicrochipLoRaModem.h>
#include <Container.h>

#define SERIAL_BAUD 57600

void callback(const uint8_t* data,unsigned int length);

MicrochipLoRaModem Modem(&Serial1, &SerialUSB, callback);
ATTDevice Device(&Modem, &SerialUSB);
Container payload(Device);


void setup() 
{
  //pinMode(DigitalSensor, INPUT);					            // initialize the digital pin as an input.          
  digitalWrite(ENABLE_PIN_IO, HIGH);
  delay(3000);
  
  SerialUSB.begin(SERIAL_BAUD);
  Serial1.begin(Modem.getDefaultBaudRate());					// init the baud rate of the serial connection so that it's ok for the modem
  while((!SerialUSB) && (millis()) < 30000){}            //wait until serial bus is available, so we get the correct logging on screen. If no serial, then blocks for 2 seconds before run
  
  while(!Device.Connect(DEV_ADDR, APPSKEY, NWKSKEY))
	  Modem.WakeUp();
  SerialUSB.println("Ready to receive data");

}


void loop() 
{
	Device.ProcessQueuePopFailed();
}

void callback(const uint8_t* data,unsigned int length)
{
	SerialUSB.print("found:");
	for(int i = 0; i < length; i++){
		SerialUSB.print("");
		SerialUSB.print(data[i], HEX);
	}
	SerialUSB.println();
  
  short id;
  void* value = (void*)payload.Parse(data, id);
  if(value){
    if(id == BUZZER)
      SerialUSB.print("value for buzzer: ");
    else if(id == RELAY)
      SerialUSB.print("value for relay: ");
    else if(id == LED)
      SerialUSB.print("value for led: "); 
    SerialUSB.println(*(bool*)value);
  }
}

