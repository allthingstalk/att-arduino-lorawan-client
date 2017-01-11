/*
AllThingsTalk - LoRa Arduino library 
 
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

#ifndef ATTDevice_h
#define ATTDevice_h

//#include "Arduino.h"
#include <string.h>
#include <Stream.h>

#include <LoRaModem.h>
//#include <DataPacket.h>
//#include <instrumentationParamEnum.h>
//#include <InstrumentationPacket.h>

/////////////////////////////////////////////////////////////
//	Configuration
/////////////////////////////////////////////////////////////
#define SEND_MAX_RETRY 30			//the default max nr of times that 'send' functions will retry to send the same value.
#define MIN_TIME_BETWEEN_SEND 0 //the minimum time between 2 consecutive calls to Send data.

#define VERSION "2.0"

#define QUEUESIZE 5									


/////////////////////////////////////////////////////////////


//this class represents the ATT cloud platform.
class ATTDevice
{
	public:
		//create the object
		//modem: the object that respresents the modem that should be used.
		//monitor: the stream used to write log lines to.
		ATTDevice(LoRaModem* modem, Stream* monitor = NULL);
		
		/*connect with the base station (call first)
		returns: true when subscribe was successful, otherwise false.*/
		bool Connect(const uint8_t* devAddress, const uint8_t* appKey, const uint8_t*  nwksKey, bool adr = true);
		
		
		//sends the specified payload to the NSP. If required, the data is buffered until it can be sent
		//the buffer has a maximum size, upon overrun, newly added messages are not added and false is returned.
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		//returns true when the packet has been buffered, or the transmission has begun. Use processQueue to get the result.
		bool Send(void* data, unsigned char size, bool ack = true);
		
		//instructs the manager to try and send a message from it's queue (if there are any) if the modem is ready with the transmission 
		//of the previous message and there has been sufficient time between transmissions.
		//returns :
		//0: no more items on to process, all is done
		//1: still items to be processed, call this function again.
		//-1: the message currently on top failed transmission: if you want to disgard it, remove it manually with pop, otherwise the system will try to resend the payload.
		int ProcessQueue();
		
		//calls processQueue and removes the item from the queue if the send failed.
		//for return values, see ProcessQueue
		int ProcessQueuePopFailed();
		
		//remove the current front from the list, if there is still one
		void Pop();
		
	private:	
	    unsigned long _minTimeBetweenSend;
		unsigned long _lastTimeSent;							//the last time that a message was sent, so we can block sending if user calls send to quickly
		Stream *_monitor;
		LoRaModem* _modem;
		unsigned char _queue[QUEUESIZE][MAX_PAYLOAD_SIZE + 2];	//buffers the data 1 extra byte in the payload for the ack request flag (at end of array -> last byte) + 1 extra byte for the size, next to last byte
		char _front;
		char _back;
		
		void Push(void* data, unsigned char size, bool ack = true);
		
		//send data to modem for transmission
		void StartSend(void* data, unsigned char size, bool ack);
		inline bool IsQueueEmpty() { return _front == _back; };
		inline bool IsQueueFull() { return _front - 1 == _back  || (_front == 0 && _back == QUEUESIZE - 1); };
		//sends the payload at the front of the queue, if there is any and if it's within the allowed time frame.
		//returns true if there is still more work to be done. False if there was no more front to be sent
		bool trySendFront();
};

#endif
