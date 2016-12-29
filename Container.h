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

Original author: Jan Bogaerts (2015-2016)
*/

#ifndef Container_h
#define Container_h

#include <string.h>
#include <Stream.h>
#include <LoRaPacket.h>

//this class represents the ATT cloud platform.
class Container: public LoRaPacket
{
	public:
		//create the object
		Container(ATTDevice &device);
		
		
		//send a bool data value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknolodge, otherwise no acknowledge is waited for.
		bool Send(bool value, short id, bool ack = true);
		
		//send an integer value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		bool Send(short value, short id, bool ack = true);
		
		//send a string data value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		bool Send(String value, short id, bool ack = true);
		
		//send a gloat data value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		bool Send(float value, short id, bool ack = true);
		
		//send data value to the cloud server for the sensor with the specified id. (x, y, z values)
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		bool Send(float x, float y, float z, short id, bool ack = true);
	
		
	protected:
	
		//writes the packet content to the specified byte array. This must be at least 51 bytes long.
		//returns: the nr of bytes actually written to the array.
		virtual unsigned char Write(unsigned char* result);
		
		//resets the content of the packet back to 0 ->> all data will be removed
		void Reset();
		
		//get the data size of the packet
		unsigned char GetDataSize();
	
	private:	
		//define the stores for all the values for this packet
		char stringValues[48];
		short intValues[16];
		float floatValues[16];
		unsigned char boolValues;
		unsigned char stringPos;
		unsigned char nrInts;
		unsigned char nrFloats;
		unsigned char nrBools;
		
		//send a bool data value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknolodge, otherwise no acknowledge is waited for.
		bool Add(bool value);
		
		//send an integer value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		bool Add(short value);
		
		//send a string data value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		bool Add(String value);
		
		//send a gloat data value to the cloud server for the sensor with the specified id.
		//if ack = true -> request acknowledge, otherwise no acknowledge is waited for.
		bool Add(float value);
};

#endif
