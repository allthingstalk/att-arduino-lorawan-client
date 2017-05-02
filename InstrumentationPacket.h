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

/**
  a data object that contains all the instrumentation data of the modem.
  Use this to get information about the following modem parameters:
  
  - Modem (number): the id of the modem. 
    values: 
      - 0: unknown
      - 1: multitech mdot
      - 2: embit-EMB-LR1272(E)
      - 3: microchip RN2483
  - data rate (number): The speed by which the modem sends and receives data. The following data rates are available: 7.8 kHz; 10.4 kHz; 15.6 kHz; 20.8 kHz; 31.2 kHz; 41.7 kHz; 62.5 kHz; 125 kHz; 250 kHz; 500 kHz. The required bandwidth can be selected according to the data requirements as well as the link conditions.
  - power index (integer): or output power: the power that the modem uses for radio communication, expressed in dBm
  - adr: Indicates if adaptive data rate is used or not (the gateway determines the speed of the modem)
  - duty cycle (integer): Duty cycle is the ratio between transmission time versus wait or receive time
  - nr of gateways (integer): The number of gateways that successfully received the last link check request frame command.
  - SNR (integer, between -128 and 127):  The signal to noise ration of the last received packet
  - retransmission count (integer):  nr of times that the modem needed to retry the last transmission.
  - bandwitdh (integer): the bandwith being used by the modem.
    values: 
      - 0: unknown
      - 1: 125
      - 2: 250
      - 3: 500
  - coding rate (integer): In telecommunication and information theory, the code rate (or information rate[1]) of a forward error correction code is the proportion of the data-stream that is useful (non-redundant). That is, if the code rate is k/n, for every k bits of useful information, the coder generates totally n bits of data, of which n-k are redundant.
    values:
      - 0: 4/5
      - 1: 4/6
      - 2: 4/7
      - 3: 4/8
  - frequency band (integer): The frequency band represents the module’s operation types (region)
    values:
      - 0: 433
      - 1: 868
  - spreading factor (integer) 
    values:
      - 0: unknown
      - 1: sf 7
      - 2: sf 8
      - 3: sf 9
      - 4: sf 10
      - 5: sf 11
      - 6: sf 12
   
    Note: for the exact bit layout of the instrumentation packet, see: instrumentation_packet_layout.PNG
*/
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
