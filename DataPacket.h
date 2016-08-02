/*
AllThingsTalk - SmartLiving.io Arduino library 
Released into the public domain.

Original author: Jan Bogaerts (2015-2016)
*/

#ifndef DataPacket_h
#define DataPacket_h

#include <string.h>
#include <Stream.h>
#include <LoRaPacket.h>

//this class represents the ATT cloud platform.
class DataPacket: public LoRaPacket
{
	public:
		//create the object
		DataPacket();
		
		//writes the packet content to the specified byte array. This must be at least 51 bytes long.
		//returns: the nr of bytes actually written to the array.
		virtual unsigned char Write(unsigned char* result);
		
		//loads a bool data value into the data packet tha is being prepared to send to the
		//cloud server.
		//the packet is sent after calling Send(id_of_sensor)
		//returns true if successful, otherwise false.
		bool Add(bool value);
		
		//loads a bool data value into the data packet tha is being prepared to send to the
		//cloud server.
		//the packet is sent after calling Send(id_of_sensor)
		//returns true if successful, otherwise false.
		bool Add(short value);
		
		//loads a bool data value into the data packet tha is being prepared to send to the
		//cloud server.
		//the packet is sent after calling Send(id_of_sensor)
		//returns true if successful, otherwise false.
		bool Add(String value);
		
		//loads a bool data value into the data packet tha is being prepared to send to the
		//cloud server.
		//the packet is sent after calling Send(id_of_sensor)
		//returns true if successful, otherwise false.
		bool Add(float value);
		
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
};

#endif
