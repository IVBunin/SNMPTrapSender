#ifndef SNMPTrapSender_h
#define SNMPTrapSender_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

class SNMPTrapSender {
public:
    SNMPTrapSender(const char* ip, int port);
    
    void setCommunity(const String& comm);
    void setOID(const String& oidStr);
    bool sendTrap(int value);
    bool sendFloatTrap(float value);  // Для отправки float значений

private:
    WiFiUDP udp;
    const char* targetIP;
    int targetPort;
    String community;
    String oid;

    void encodeLength(uint8_t* buf, int &offset, int length);
    void encodeOID(uint8_t* buf, int &offset, const char* oid);
    void encodeInteger(uint8_t* buf, int &offset, int value);
    void encodeFloat(uint8_t* buf, int &offset, float value);
};

#endif
