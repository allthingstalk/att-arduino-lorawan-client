
//#include <Wire.h>
#include <ATT_IOT_LoRaWAN.h>
#include <MicrochipLoRaModem.h>
#include <Container.h>
#include "keys.h"

#define SERIAL_BAUD 57600
#define SEND_EVERY 30 * 1000
#define MONITOR SERIAL_PORT_MONITOR

MicrochipLoRaModem Modem(&Serial2, &MONITOR);
ATTDevice Device(&Modem, &MONITOR, true, 7000);
Container payload(Device);


unsigned long sendNextAt = 0;

void setup() 
{
    
    MONITOR.begin(SERIAL_BAUD);                             // set baud rate of the default MONITOR debug connection
  
    while((!MONITOR) && (millis()) < 4000){}                //wait until MONITOR bus is available, so we get the correct logging on screen. If no MONITOR, then blocks for 2 seconds before run
    MONITOR.println("start");  
    Serial2.begin(Modem.getDefaultBaudRate());              // set baud rate of the MONITOR connection between Mbili and LoRa modem

    // RESET LORA_MODULE
    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, LOW);
    delay(100);
    digitalWrite(LORA_RESET, HIGH);
    delay(1000);
    Serial2.println("sys reset");
    delay(100);
    Serial2.println("sys get ver");
    delay(100);

    while(!Device.Connect(DEV_ADDR, APPSKEY, NWKSKEY))
        MONITOR.println("retrying...");                     // initialize connection with the AllThingsTalk Developer Cloud
    MONITOR.println("Ready to send data");
    Device.ProcessQueue();
}

short count = 0;

void loop() 
{
    if (sendNextAt < millis()){
        MONITOR.println("sending data");
        Senddata(count++);
        MONITOR.print("sleep for: ");
        MONITOR.println(SEND_EVERY);
        MONITOR.println();
        sendNextAt = millis() + SEND_EVERY;
    }
    Device.ProcessQueue();
    
}

bool Senddata(short val)
{
  MONITOR.print("Data: ");MONITOR.println(val);
  bool res = payload.Send((short)val, INTEGER_SENSOR);
  if(res == false)
    MONITOR.println("maybe the last couple of packages were sent too quickly after each other? (min of 15 seconds recommended)");
  return res;
}


