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

#define SERIAL_BAUD 57600

int DigitalSensor = 20;                                        // digital sensor is connected to pin D20/21
MicrochipLoRaModem Modem(&Serial1, &SerialUSB);
ATTDevice Device(&Modem, &SerialUSB);

struct DemoData
{
	short value1;
	unsigned char value2;
	unsigned char value3;
		
};


DemoData data;

void setup() 
{
  //pinMode(DigitalSensor, INPUT);					            // initialize the digital pin as an input.          
  digitalWrite(ENABLE_PIN_IO, HIGH);
  delay(3000);
  
  SerialUSB.begin(SERIAL_BAUD);
  Serial1.begin(Modem.getDefaultBaudRate());					// init the baud rate of the serial connection so that it's ok for the modem
  while((!SerialUSB) && (millis()) < 30000){}            //wait until serial bus is available, so we get the correct logging on screen. If no serial, then blocks for 2 seconds before run
  
  while(!Device.Connect(DEV_ADDR, APPSKEY, NWKSKEY));
  SerialUSB.println("Ready to send data");

  //Modem.PrintModemConfig();
}

int counter = 0;
unsigned long sendNextAt = 0;
int sendState = 1;

void loop() 
{
	data.value1 = counter;
	data.value2 = counter;
	data.value3 = counter;
	if (sendNextAt < millis()){
		//Modem.Send(&data, sizeof(data));					//blocking
		bool sendSuccess = Device.Send(&data, sizeof(data));				//non blocking
    if(sendSuccess == false){
      SerialUSB.println("discarding data");
    }
    else sendState = 1;                           //mke certain we try to process the queue again.
		counter++;
		sendNextAt = millis() + 5000;
	}
	sendState = Device.ProcessQueue();
	delay(100);
}



