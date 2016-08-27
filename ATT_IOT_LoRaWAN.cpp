/*
	ATT_IOT.cpp - SmartLiving.io LoRa Arduino library 
*/

#include <ATT_IOT_LoRaWAN.h>
#include "Utils.h"
#include <arduino.h>			//still required for the 'delay' function. use #ifdef for other platforms.


//create the object
ATTDevice::ATTDevice(LoRaModem* modem, Stream* monitor):  _minTimeBetweenSend(MIN_TIME_BETWEEN_SEND)
{
	_modem = modem;
	 _monitor = monitor;
	_back = _front = _lastTimeSent = 0;
}

//connect with the to the lora gateway
bool ATTDevice::Connect(const uint8_t* devAddress, const uint8_t* appKey, const uint8_t* nwksKey, bool adr)
{
	PRINT("ATT lib version: "); PRINTLN(VERSION);
	if(!_modem->Stop()){								//stop any previously running modems
		PRINTLN("can't communicate with modem: possible hardware issues");
		return false;
	}
	
	if (!_modem->SetLoRaWan(adr)){						//switch to LoRaWan mode instead of peer to peer				
		PRINTLN("can't set adr: possible hardware issues?");
		return false;
	}
	if(!_modem->SetDevAddress(devAddress)){
		PRINTLN("can't assign device address to modem: possible hardware issues?");
		return false;
	}
	if(!_modem->SetAppKey(appKey)){
		PRINTLN("can't assign app session key to modem: possible hardware issues?");
		return false;
	}
	if(!_modem->SetNWKSKey(nwksKey)){
		PRINTLN("can't assign network session key to modem: possible hardware issues?");
		return false;
	}
	bool result = _modem->Start();								//start the modem up 
	if(result == true){
		PRINTLN("modem initialized");
	}
	else{
		PRINTLN("Parameters loaded, but modem won't start: initialization failed");
	}
	return result;									//we have created a connection successfully.
}


bool ATTDevice::trySendFront()
{
	if(IsQueueEmpty() == false){
		unsigned long curTime = millis();
		unsigned long timeoutAt = _lastTimeSent + _minTimeBetweenSend;
		if(timeoutAt < curTime){
			StartSend(_queue[_front], _queue[_front][MAX_PAYLOAD_SIZE - 2], _queue[_front][MAX_PAYLOAD_SIZE - 1]);
		}
		return true;
	}
	return false;														//nothing left to be sent.
}

//instructs the manager to try and send a message from it's queue (if there are any).
int ATTDevice::ProcessQueue()
{
	//PRINTLN(_modem->IsFree())
	bool sendResult = false;
	if(_modem->IsFree() == true)
		return (int)trySendFront();
	else if(_modem->CheckSendState(sendResult) == true){
		if(sendResult == true){													// modem succesfully sent a packet, so remove from queue.
			PRINTLN("modem reported successfull send")
			Pop();
			return (int)trySendFront();
		}
		else{
			PRINTLN("modem reported failed send")
			return -1;
		}
	}
	return 1;																	//there is still work to be done: modem is not free and still working on something.
}

int ATTDevice::ProcessQueuePopFailed(int sendState)
{
	if(sendState != 0){
		sendState = ProcessQueue();
		if(sendState == -1){
			PRINTLN("send failed")
			Pop();                        //remove problem message.
		}
	}
	return sendState;
}

int ATTDevice::ProcessQueueRetryFailed(int sendState)
{
	if(sendState != 0){
		sendState = ProcessQueue();
		if(sendState == -1)
			PRINTLN("send failed, retrying next time")
	}
	return sendState;
}

bool ATTDevice::Send(void* packet, unsigned char size, bool ack)
{
	//check if the packet is not too big
	if(size > _modem->maxPayloadForSF()){
		PRINTLN("Data size exceeds limitations for current spreading factor.")
		return false;
	}
	short nrRetries = 0;
	unsigned long curTime = millis();
	if(IsQueueEmpty() == false || 												// there could be previous payloads waiting to be sent first
	   _modem->IsFree() == false || 											// or the modem could still be processing a packet.
	   (_lastTimeSent != 0 && _lastTimeSent + _minTimeBetweenSend > curTime))	// or it's not yet time to send a new packet
	{
		if(IsQueueFull() == true){
			PRINTLN("buffer is full, can't transmit packet");
			return false;
		}
		else{
			Push(packet, size, ack);
			return true;
		}
	}
	else{
		StartSend(packet, size, ack);
		return true;
	}
	
}

void ATTDevice::StartSend(void* packet, unsigned char size, bool ack)
{
	// calculate BEFORE or AFTER send ??  (-> sf might change ... before would be the actual value used in send)
	float toa = _modem->calculateTimeOnAir(size); // calculate for current settings, so BEFORE send !!
	PRINT("TOA: ") PRINTLN(toa)
	_modem->SendAsync(packet, size, ack);
	_lastTimeSent = millis();
	toa = toa * 100;
	_minTimeBetweenSend = ceil(toa);			//dynamically adjust
	PRINT("min time between send: ") PRINTLN(_minTimeBetweenSend)
}


void ATTDevice::Pop()
{
	if(_front != _back){						//if both are the same there is nothing to pop
		_front++;
		if(_front >= QUEUESIZE)
			_front = 0;
	}
}

void ATTDevice::Push(void* data, unsigned char size, bool ack)
{
	PRINTLN("buffering payload:")
	for (unsigned char i = 0; i < size; i++) {
		PRINT(((unsigned char*)data)[i], HEX) PRINT(" ")
		//_modem->printHex(((unsigned char*)data)[i]);
	}
	PRINTLN();
	
	memcpy(_queue[_back], data, size);
	_queue[_back][MAX_PAYLOAD_SIZE - 2] = size;
	_queue[_back][MAX_PAYLOAD_SIZE - 1] = ack;
	_back++;
	if(_back >= QUEUESIZE)
		_back = 0;
}
