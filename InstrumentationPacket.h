/*
AllThingsTalk - SmartLiving.io Arduino library 
Released into the public domain.

Original author: Jan Bogaerts (2015-2016)
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
		//create the object
		InstrumentationPacket();
		
		//writes the packet content to the specified byte array. This must be at least 51 bytes long.
		//returns: the nr of bytes actually written to the array.
		virtual unsigned char Write(unsigned char* result);
		
		
		//resets the content of the packet back to 0 ->> all data will be removed
		void Reset();
		
		bool SetParam(instrumentationParam param, int value); 
		
		//get the data size of the packet
		unsigned char GetDataSize();
		
		//collects all the instrumentation data from the modem (RSSI, ADR, datarate,..) and store
		//it in the object. Also print every value that was collected to the monitor (if any.)
		bool BuildInstrumentation(LoRaModem& modem, Stream* monitor = NULL);
	protected:
		//returns the frame type number for this lora packet. The default value is 0x40. Inheritors that render other packet types can overwrite this.
		unsigned char getFrameType();
	private:	
		unsigned char _data[INST_DATA_SiZE];
		void SetInstrumentationParam(Stream* monitor, instrumentationParam param, char* name, int value);
};

#endif