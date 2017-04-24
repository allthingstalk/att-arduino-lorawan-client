/*
AllThingsTalk - Arduino library 

   Copyright 2015-2016 AllThingsTalk

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

Original author: Jan Bogaerts (2015-2017)
*/

#ifndef InstrumentationPacket_h
#define InstrumentationPacket_h

#include <string.h>
#include <Stream.h>

#include <instrumentationParamEnum.h>
#include <LoRaPacket.h>
#include <LoRaModem.h>

#define INST_DATA_SiZE 8

//this class represents the ATT cloud platform.
class InstrumentationPacket: public LoRaPacket
{
	public:
		/**
		Create the object.
		
		parameters:
		- device: the buffer object to use for transmitting data.
		- monitor: a stream object used to write monitoring output (text) towards.
				   Default is NULL.
		*/
		InstrumentationPacket(ATTDevice &device, Stream* monitor = NULL);
		
		/**
		Build the instrumentation and send it to the cloud.
		
		parameters:
		- ack: when true (default), acknowledgement is requested from the base station, otherwise no acknowledge is waited for.
		
		returns: true upon success.
		*/
		bool Send(bool ack = true);
		
		
		/**
		Get the data size of the packet (nr of bytes).
		*/
		unsigned char GetDataSize();
		
		/**
		Collects all the instrumentation data from the modem (RSSI, ADR, datarate,..) and store it in the object. Also print every value that was collected to the monitor (if any).
		
		parameters:
		- modem: the lora modem object to retreive the information from.
		
		returns: true upon success.
		*/
		bool BuildInstrumentation(LoRaModem& modem);
	protected:
		///returns the frame type number for this lora packet. The default value is 0x40. Inheritors that render other packet types can overwrite this.
		unsigned char getFrameType();
		
		///writes the packet content to the specified byte array. This must be at least 51 bytes long.
		///returns: the nr of bytes actually written to the array.
		virtual unsigned char Write(unsigned char* result);
		
		
		///resets the content of the packet back to 0 ->> all data will be removed
		void Reset();
		
	private:	
		unsigned char _data[INST_DATA_SiZE];
		Stream* _monitor;
		void SetInstrumentationParam(instrumentationParam param, char* name, int value);
		bool SetParam(instrumentationParam param, int value); 
};

#endif
