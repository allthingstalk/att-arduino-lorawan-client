/****
 *  AllThingsTalk Developer Cloud IoT experiment for LoRa
 *  Version 1.0 dd 09/11/2015
 *  Original author: Jan Bogaerts 2015
 *
 *  This sketch is part of the AllThingsTalk LoRa rapid development kit
 *  -> http://www.allthingstalk.com/lora-rapid-development-kit
 *
 *  This example sketch is based on the Proxilmus IoT network in Belgium
 *  The sketch and libs included support the
 *  - MicroChip RN2483 LoRa module
 *  - Embit LoRa modem EMB-LR1272
 *  
 *  For more information, please check our documentation
 *  -> http://docs.smartliving.io/kits/lora
 *  
 **/

//#include <Wire.h>
#include <ATT_IOT_LoRaWAN.h>
#include "keys.h"
#include <MicrochipLoRaModem.h>
#include "DataPacket.h"

#define SERIAL_BAUD 57600

int DigitalSensor = 20;                                        // digital sensor is connected to pin D20/21
MicrochipLoRaModem Modem(&Serial1, &SerialUSB);
ATTDevice Device(&Modem, &SerialUSB);
DataPacket payload;


void setup() 
{
	payload.SetId(LOUDNESS_SENSOR);
	
  //pinMode(DigitalSensor, INPUT);					            // initialize the digital pin as an input.          
  digitalWrite(ENABLE_PIN_IO, HIGH);
  delay(3000);
  
  SerialUSB.begin(SERIAL_BAUD);
  Serial1.begin(Modem.getDefaultBaudRate());					// init the baud rate of the serial connection so that it's ok for the modem
  while((!SerialUSB) && (millis()) < 30000){}            //wait until serial bus is available, so we get the correct logging on screen. If no serial, then blocks for 2 seconds before run
  
  while(!Device.Connect(DEV_ADDR, APPSKEY, NWKSKEY));
  SerialUSB.println("Ready to send data");
}

float value = 0;
unsigned long sendNextAt = 0;
int sendState = 1;

void loop() 
{
	if (sendNextAt < millis()){
		send();
   value++;
		sendNextAt = millis() + 15000;
  }
	sendState = Device.ProcessQueuePopFailed(sendState);
	delay(100);
}

void send()
{
	unsigned char buffer[220];
	payload.Reset();
	payload.Add(value);
	unsigned char length = payload.Write(buffer);
	bool sendSuccess = Device.Send(buffer, length);
	
	if(sendSuccess == true){
      sendState = 1;
   }
   
}

void serialEvent1()
{
  Device.Process();														// for future use of actuators
}


