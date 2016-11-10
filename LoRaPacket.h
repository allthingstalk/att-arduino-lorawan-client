/*
AllThingsTalk - AllThingsTalk.io Arduino library 

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

#ifndef LoRaPacket_h
#define LoRaPacket_h


#define BINARY_SENSOR ((short)1)
#define BINARY_TILT_SENSOR ((short)2)
#define PUSH_BUTTON ((short)3)
#define DOOR_SENSOR ((short)4)
#define TEMPERATURE_SENSOR ((short)5)
#define LIGHT_SENSOR ((short)6)
#define PIR_SENSOR ((short)7)
#define ACCELEROMETER ((short)8)
#define GPS ((short)9)
#define PRESSURE_SENSOR ((short)10)
#define HUMIDITY_SENSOR ((short)11)
#define LOUDNESS_SENSOR ((short)12)
#define AIR_QUALITY_SENSOR ((short)13)
#define BATTERY_LEVEL ((short)14)
#define INTEGER_SENSOR ((short)15)
#define NUMBER_SENSOR ((short)16)

#include <Stream.h>

//this class represents the ATT cloud platform.
class LoRaPacket
{
	public:
		//create the object
		LoRaPacket();
		
		//writes the packet content to the specified byte array. This must be max 51 to 220 bytes long, depending on spreading factor.
		//returns: the nr of bytes actually written to the array.
		virtual unsigned char Write(unsigned char* result);
		
		//assigns the asset/container id to the packet
		void SetId(unsigned char id);
		
		
		//resets the content of the packet back to 0 ->> all data will be removed
		virtual void Reset() = 0;
		
		//get the data size of the packet
		virtual unsigned char GetDataSize() = 0;
	protected:
		//returns the frame type number for this lora packet. The default value is 0x40. Inheritors that render other packet types can overwrite this.
		virtual unsigned char getFrameType();
		//calculate the checksum of the packet and return it.
		unsigned char calculateCheckSum(unsigned char* toSend, short len);
		
	private:	
		unsigned char contId;
};

#endif
