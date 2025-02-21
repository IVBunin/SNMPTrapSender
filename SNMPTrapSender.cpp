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

#include "SNMPTrapSender.h"

SNMPTrapSender::SNMPTrapSender(const char* ip, int port, bool useV2c)
    : targetIP(ip), targetPort(port), community("public"), enterpriseOID("1.3.6.1.4.1.4976"), snmpV2c(useV2c) {}

void SNMPTrapSender::setCommunity(const String& comm) {
    community = comm;
}

void SNMPTrapSender::setOID(const String& oidStr) {
    oid = oidStr;
}

void SNMPTrapSender::setEnterpriseOID(const String& oidStr) {
    enterpriseOID = oidStr;
}

bool SNMPTrapSender::sendTrap(int value) {
    return sendTrapData([value]() { return value; });
}

bool SNMPTrapSender::sendTrapData(std::function<int()> dataFunction) {
    int value = dataFunction();
    uint8_t buffer[128];
    int offset = 0;

    buffer[offset++] = 0x30;
    int seqStart = offset;
    buffer[offset++] = 0x00;

    buffer[offset++] = 0x02;
    buffer[offset++] = 0x01;
    buffer[offset++] = snmpV2c ? 0x01 : 0x00;

    buffer[offset++] = 0x04;
    buffer[offset++] = community.length();
    memcpy(&buffer[offset], community.c_str(), community.length());
    offset += community.length();

    buffer[offset++] = snmpV2c ? 0xA7 : 0xA4;
    int pduStart = offset;
    buffer[offset++] = 0x00;

    encodeOID(buffer, offset, oid.c_str());
    encodeInteger(buffer, offset, value);

    buffer[pduStart] = offset - pduStart - 1;
    buffer[seqStart] = offset - seqStart - 1;

    udp.beginPacket(targetIP, targetPort);
    udp.write(buffer, offset);
    return udp.endPacket();
}

void SNMPTrapSender::encodeOID(uint8_t* buf, int &offset, const char* oid) {
    buf[offset++] = 0x06;
    int oidStart = offset++;
    String oidStr(oid);
    int first, second;
    sscanf(oidStr.c_str(), "%d.%d", &first, &second);
    buf[offset++] = (first * 40) + second;
    oidStr = oidStr.substring(oidStr.indexOf('.') + 1);
    oidStr = oidStr.substring(oidStr.indexOf('.') + 1);
    while (oidStr.length() > 0) {
        int val;
        sscanf(oidStr.c_str(), "%d", &val);
        buf[offset++] = val;
        int dotIndex = oidStr.indexOf('.');
        if (dotIndex == -1) break;
        oidStr = oidStr.substring(dotIndex + 1);
    }
    buf[oidStart] = offset - oidStart - 1;
}

void SNMPTrapSender::encodeInteger(uint8_t* buf, int &offset, int value) {
    buf[offset++] = 0x02;
    buf[offset++] = 0x01;
    buf[offset++] = value;
}
