#include "SNMPTrapSender.h"

SNMPTrapSender::SNMPTrapSender(const char* ip, int port) 
    : targetIP(ip), targetPort(port), community("public") {}

void SNMPTrapSender::setCommunity(const String& comm) {
    community = comm;
}

void SNMPTrapSender::setOID(const String& oidStr) {
    oid = oidStr;
}

// Основная реализация методов кодирования и отправки (аналогичная предыдущему коду)
// Полный код библиотеки доступен здесь: https://github.com/example/snmptrapsender 
