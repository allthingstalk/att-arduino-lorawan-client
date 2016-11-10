/*
AllThingsTalk - Abstract class for LoRa modems

   Copyright 2015-2016 AllThingsTalk

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

Original author: Jan Bogaerts (2015)
*/

#ifndef LoRaModem_h
#define LoRaModem_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include <instrumentationParamEnum.h>

#define SENDSTATE_TRANSMITCOMMAND 0				//3 states used to track async send status
#define SENDSTATE_EXPECTOK 1
#define SENDSTATE_GETRESPONSE 2
#define SENDSTATE_DONE 3

#define MAX_PAYLOAD_SIZE 222					//maximum allowed size for a payload
#define SMALLEST_PAYLOAD_SIZE 51			 	//the payload size for the biggest sf


//callback signature for functions that process data coming from the NSP that were send to the device
//first param: start of byte array that was found
//second param: the length of the package
#define ATT_CALLBACK_SIGNATURE void (*callback)(const uint8_t*,unsigned int)

//this class represents the ATT cloud platform.
class LoRaModem
{
	public:
		///create modem objects
		//MQTT_CALLBACK_SIGNATURE: assign a callback function that is called when incoming data (from nsp to device) needs to be processed
		LoRaModem(Stream *monitor, ATT_CALLBACK_SIGNATURE = NULL);
	
		// Returns the required baudrate for the device
		virtual unsigned int getDefaultBaudRate() = 0;
		//stop the modem.
		virtual bool Stop() = 0;
		//set the modem in LoRaWan mode (vs private networks)
		virtual bool SetLoRaWan(bool adr = true) = 0;
		//assign a device address to the modem
		//devAddress must be 4 bytes long
		virtual bool SetDevAddress(const unsigned char* devAddress) = 0;
		//set the app session key for the modem communication
		//app session key must be 16 bytes long
		virtual bool SetAppKey(const unsigned char* appKey) = 0;
		//set the network session key
		//network session key must be 16 bytes long
		virtual bool SetNWKSKey(const unsigned char*  nwksKey) = 0;
		//start the modem: returns true if successful
		virtual bool Start() = 0;
		//send a data packet to the server
		//ack = true -> request ack
		virtual bool Send(void* packet, unsigned char size, bool ack = true);
		
		//start the send process, but return before everything is done.
		//returns true if the packet was succesfully send, and the process of waiting for a resonse can begin. Otherwise, it returns false
		virtual bool SendAsync(void* packet, unsigned char size, bool ack = true) = 0;
		//checks the status of the current send operation (if there was any).
		//if there was none or the operation is done, then true is done. 
		//the result of the send operation is returned  in the param 'sendResult'
		virtual bool CheckSendState(bool& sendResult) = 0;
		
		//returns true if the modem can send a payload. If it can't at the moment (still processing another packet), then false is returned.
		inline bool IsFree(){ return sendState == SENDSTATE_DONE; };
		
		//process any incoming packets from the modem
		virtual void ProcessIncoming() = 0;
		//extract the specified instrumentation parameter from the modem and return the value
		virtual int GetParam(instrumentationParam param) = 0;
		//returns the id number of the modem type. See the container definition for the instrumentation container to see more details.
		virtual int GetModemId() = 0;
		
		//calcualte the max payload size, based on the current spreading factor of the modem. Used to check if the packet can be sent.
		int maxPayloadForSF(short spreading_factor = -1);
		
		float calculateTimeOnAir(unsigned char appPayloadSize, short spreading_factor = -1);
		
	protected:
		Stream *_monitor;
		void (*_callback)(const uint8_t*,unsigned int);
		//keeps track of the current async send position: are we waiting for 'ok' or response.
		char sendState;
		
		//stores the previous payload size. This is for in case that the payload size is requested while we are waiting on a response from
		//the modem. In this case, we can't request the sf from the modem, so we can't calculate the value. Instead we use this buffered val.
		int _prevPayloadForSF;
		
		void printHex(unsigned char hex);
		
		float calculateSymbolTime(short spreading_factor = -1, short bandwidth = -1) ;
		int calculateSymbolsInPayload(unsigned char appPayloadSize, short spreading_factor) ;
	
};

#endif