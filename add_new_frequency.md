# how to add support for new frequency regencies

- in stringLiterals.h, add the name of the modem (ex: #define STR_DEVICE_TYPE_EU "RN2483")
- in MicrochipLoRaModem::Stop(), add the new modem frequency to the line:
result = expectString(STR_DEVICE_TYPE_EU) || checkInputInstring(STR_DEVICE_TYPE_US)  