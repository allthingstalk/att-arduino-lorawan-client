/*
   Copyright 2015-2016 AllThingsTalk

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#include <InstrumentationPacket.h>
#define PRINTLN(...) {if(_monitor) _monitor->println(__VA_ARGS__); }
#define PRINT(...) {if(_monitor) _monitor->print(__VA_ARGS__); }

//create the object
InstrumentationPacket::InstrumentationPacket(ATTDevice &device, Stream* monitor): LoRaPacket(device), _monitor(monitor)
{
	SetId(0x11);
}

unsigned char InstrumentationPacket::Write(unsigned char* result)
{
	unsigned char curPos =  LoRaPacket::Write(result);
	
	memcpy(result + curPos, _data, INST_DATA_SiZE);
	curPos += INST_DATA_SiZE;
	
	short packetLen = curPos - 3;												//write the packet length
	memcpy(result + 1, static_cast<const char*>(static_cast<const void*>(&packetLen)), sizeof(short));
	
	result[curPos] = calculateCheckSum(result + 3, curPos - 3);					//add the checksum
	curPos++;
	return curPos;
}

//resets the content of the packet back to 0 ->> all data will be removed
void InstrumentationPacket::Reset()
{
	memset(_data, NULL, INST_DATA_SiZE);
	SetId(0x11);
}

unsigned char InstrumentationPacket::getFrameType()
{
	return 0x60;									//the default packet type
}

bool InstrumentationPacket::SetParam(instrumentationParam param, int value)
{
	switch(param){
		case MODEM:
			_data[0] = (value << 4) | (_data[0] & 0x0F);     //make certain that any prev value is removed with 'and', add bits with or 
			return true;
		case FREQUENCYBAND:
			_data[0] = (value << 3) | (_data[0] & 0xF7);
			return true;
		case SP_FACTOR:
			_data[0] = value | (_data[0] & 0xF8);
			return true;
		case ADR:
			_data[1] = (value << 7) | (_data[1] & 0x7F);
			return true;
		case POWER_INDEX:
			_data[1] = (value << 4) | (_data[1] & 0x8F);
			return true;
		case BANDWIDTH:
			_data[1] = (value << 2) | (_data[1] & 0xF3);
			return true;
		case CODING_RATE:
			_data[1] = value | (_data[1] & 0xFC);
			return true;
		case DUTY_CYCLE:
			*((unsigned short*)(_data + 2)) = (unsigned short)value;
			return true;
		case SNR:
			_data[4] = value;
			return true;
		case GATEWAY_COUNT:
			_data[5] = value;
			return true;
		case RETRANSMISSION_COUNT:
			_data[6] = value;
			return true;
		case DATA_RATE:
			_data[7] = value;
			return true;
		default: return false;
	}	
}

unsigned char InstrumentationPacket::GetDataSize() {
	unsigned char cnt = 6;	// LoRaPacket header size;
	return cnt + INST_DATA_SiZE;
}


//collects all the instrumentation data from the modem (RSSI, ADR, datarate,..) and store
//it in the object. Also print every value that was collected to the monitor (if any.)
bool InstrumentationPacket::BuildInstrumentation(LoRaModem& modem)
{
	PRINTLN("instrumentation values:");
	SetInstrumentationParam(MODEM, "modem", modem.GetModemId());
	SetInstrumentationParam(DATA_RATE, "data rate", modem.GetParam(DATA_RATE));
	SetInstrumentationParam(FREQUENCYBAND, "frequency band", modem.GetParam(FREQUENCYBAND));
	SetInstrumentationParam(POWER_INDEX, "power index", modem.GetParam(POWER_INDEX));
	SetInstrumentationParam(ADR, "ADR", modem.GetParam(ADR));
	SetInstrumentationParam(DUTY_CYCLE, "duty cycle", modem.GetParam(DUTY_CYCLE));
	SetInstrumentationParam(GATEWAY_COUNT, "nr of gateways", modem.GetParam(GATEWAY_COUNT));
	SetInstrumentationParam(SNR, "SNR", modem.GetParam(SNR));
	SetInstrumentationParam(SP_FACTOR, "spreading factor", modem.GetParam(SP_FACTOR));
	SetInstrumentationParam(BANDWIDTH, "bandwidth", modem.GetParam(BANDWIDTH));
	SetInstrumentationParam(CODING_RATE, "coding rate", modem.GetParam(CODING_RATE));
	SetInstrumentationParam(RETRANSMISSION_COUNT, "retransmission count", modem.GetParam(RETRANSMISSION_COUNT));
}


//store the param in the  data packet, and print to serial.
void InstrumentationPacket::SetInstrumentationParam(instrumentationParam param, char* name, int value)
{
	SetParam(param, value);
	PRINT(name);
	PRINT(": ");
	switch(param){
		case MODEM:
			if(value == 0){ PRINTLN("unknown");}
			else if(value == 1){ PRINTLN("multitech mdot");}
			else if(value == 2){ PRINTLN("embit-EMB-LR1272(E)");}
			else if(value == 3){ PRINTLN("microchip RN2483");}
			else {PRINT("unknown value: "); PRINTLN(value);}
			break;
		case BANDWIDTH:
			if(value == 0){ PRINTLN("unknown");}
			else if(value == 1){ PRINTLN("125");}
			else if(value == 2){ PRINTLN("250");}
			else if(value == 3){ PRINTLN("500");}
			else {PRINT("unknown value: "); PRINTLN(value);}
			break;
		case CODING_RATE:
			if(value == 0){ PRINTLN("4/5");}
			else if(value == 1){ PRINTLN("4/6");}
			else if(value == 2){ PRINTLN("4/7");}
			else if(value == 3){ PRINTLN("4/8");}
			else {PRINT("unknown value: "); PRINTLN(value);}
			break;
		case FREQUENCYBAND:
			if(value == 0){ PRINTLN("433");}
			else if(value == 1){ PRINTLN("868");}
			else {PRINT("unknown value: "); PRINTLN(value);}
			break;
		case SP_FACTOR:
			if(value == 0){ PRINTLN("unknown");}
			else if(value == 1){ PRINTLN("sf7");}
			else if(value == 2){ PRINTLN("sf8");}
			else if(value == 3){ PRINTLN("sf9");}
			else if(value == 4){ PRINTLN("sf10");}
			else if(value == 5){ PRINTLN("sf11");}
			else if(value == 6){ PRINTLN("sf12");}
			else {PRINT("unknown value: "); PRINTLN(value);}
			break;
		default: 
			PRINTLN(value);
			break;
	}
}
