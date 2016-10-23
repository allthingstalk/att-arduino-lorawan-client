/*
AllThingsTalk - SmartLiving.io Communicate with Embit lora modems through binary AT commands
Released into the public domain.

Original author: Jan Bogaerts (2015)
*/

#ifndef EmbitLoRaModem_h
#define EmbitLoRaModem_h

#include <LoRaModem.h>
#include <LoRaPacket.h>
#include <instrumentationParamEnum.h>
#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#define DEFAULT_PAYLOAD_SIZE MAX_PAYLOAD_SIZE
#define PORT 1
#define DEFAULT_INPUT_BUFFER_SIZE 440				//needs to be big enough to store 220 hex byte values.
#define DEFAULT_RECEIVED_PAYLOAD_BUFFER_SIZE MAX_PAYLOAD_SIZE
#define DEFAULT_TIMEOUT 120
#define RECEIVE_TIMEOUT 60000
#define MAX_SEND_RETRIES 10


#if defined(ARDUINO_ARCH_AVR)
typedef HardwareSerial SerialType;
#elif defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
typedef Uart SerialType;
#else
typedef Stream SerialType;
#endif

#define ENABLE_SLEEP

enum MacTransmitErrorCodes
{
	NoError = 0,
	NoResponse = 1,
	Timeout = 2,
	TransmissionFailure = 3
};

//this class represents the ATT cloud platform.
class MicrochipLoRaModem: public LoRaModem
{
	public:
		//create the object
		MicrochipLoRaModem(SerialType* stream, Stream* monitor = NULL, ATT_CALLBACK_SIGNATURE = NULL);
		// Returns the required baudrate for the device
		unsigned int getDefaultBaudRate();
		//stop the modem.
		bool Stop();
		//set the modem in LoRaWan mode (vs private networks)
		//adr = adaptive data rate. true= use, false = none adaptive data rate
		bool SetLoRaWan(bool adr = true);
		//assign a device address to the modem
		//devAddress must be 4 bytes long
		bool SetDevAddress(const unsigned char* devAddress);
		//set the app session key for the modem communication
		//app session key must be 16 bytes long
		bool SetAppKey(const unsigned char* appKey);
		//set the network session key
		//network session key must be 16 bytes long
		bool SetNWKSKey(const unsigned char*  nwksKey);
		//start the modem , returns true if successful
		bool Start();
		//send a data packet to the server
		bool Send(void* packet, unsigned char size, bool ack = true);
		
		//send a data packet to the server
		//returns true if the packet was succesfully send, and the process of waiting for a resonse can begin. Otherwise, it returns false
		bool SendAsync(void* packet, unsigned char size, bool ack = true);
		
		//checks the status of the current send operation (if there was any).
		//if there was none or the operation is done, then true is done. 
		//the result of the send operation is returned  in the param 'sendResult'
		bool CheckSendState(bool& sendResult);
		
		//process any incoming packets from the modem
		void ProcessIncoming();
		//extract the specified instrumentation parameter from the modem and return the value
		int GetParam(instrumentationParam param);
		//returns the id number of the modem type. See the container definition for the instrumentation container to see more details.
		int GetModemId();
		//prints all configuration params (radio and mac) to the monitor
		void PrintModemConfig();
		#ifdef ENABLE_SLEEP
		//put the modem in sleep mode for 3 days (use WakeUp if you want to send something earlier)
		void Sleep();
		//wakes up the device after it has been put the sleep.
		void WakeUp();
		#endif
	private:
		Stream *_monitor;
		SerialType* _stream;					//the stream to communicate with the lora modem.
		char inputBuffer[DEFAULT_INPUT_BUFFER_SIZE + 1];
		//stores the starting time of the current async operation
		unsigned long asyncOperationStart;
		
		unsigned char lookupMacTransmitError(const char* error);
		unsigned char onMacRX();
		unsigned short readLn(char* buf, unsigned short bufferSize, unsigned short start = 0);
		unsigned short readLn() { return readLn(this->inputBuffer, DEFAULT_INPUT_BUFFER_SIZE); };
		bool expectOK();
		//block until the string is found on input or timeout occured.
		bool expectString(const char* str, unsigned short timeout = DEFAULT_TIMEOUT);
		//try to read the string on the input 1 time.
		char tryReadString(const char* str);
		//try to read the string from teh input until found or untill timeout, do this async, so call many times
		char expectStringAsync(const char* str, unsigned short timeout);
		
		bool setMacParam(const char* paramName, const unsigned char* paramValue, unsigned short size);
		bool setMacParam(const char* paramName, unsigned char paramValue);
		bool setMacParam(const char* paramName, const char* paramValue);
		//send and wait for response
		unsigned char macTransmit(const char* type, const unsigned char* payload, unsigned char size);
		//send command
		void macSendCommand(const char* type, const unsigned char* payload, unsigned char size);
		//convert the text value for spreading factor into a number between 0 and 6
		int sfToIndex(char* value);
		//retrieves the specified parameter from the radio
		char* getRadioParam(const char* paramName, unsigned short timeout = DEFAULT_TIMEOUT);
		//retrieves the specified parameter from the radio
		char* getMacParam(const char* paramName, unsigned short timeout = DEFAULT_TIMEOUT);
		
		unsigned char macTransmitGetResponse();
};

#endif