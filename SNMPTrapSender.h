#ifndef SNMPTrapSender_h
#define SNMPTrapSender_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <functional>

class SNMPTrapSender {
public:
    SNMPTrapSender(const char* ip, int port, bool useV2c = true);
    void setCommunity(const String& comm);
    void setOID(const String& oidStr);
    void setEnterpriseOID(const String& oidStr);
    bool sendTrap(int value);
    bool sendTrapData(std::function<int()> dataFunction);

private:
    WiFiUDP udp;
    const char* targetIP;
    int targetPort;
    String community;
    String oid;
    String enterpriseOID;
    bool snmpV2c;

    void encodeOID(uint8_t* buf, int &offset, const char* oid);
    void encodeInteger(uint8_t* buf, int &offset, int value);
};

#endif