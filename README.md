# Документация библиотеки SNMPTrapSender

## Обзор
SNMPTrapSender — это легковесная библиотека для отправки SNMP-трапов, разработанная для микроконтроллеров ESP32. Она поддерживает SNMP v1 и v2c, позволяя передавать сообщения ловушек SNMP-менеджеру.

## Возможности
- Поддержка SNMP v1 и v2c.
- Возможность задания пользовательских строк сообщества и OID.
- Отправка SNMP-трапов с целочисленными значениями.
- Настраиваемый OID предприятия.
- Оптимизировано для Arduino-плат ESP32.

## Установка
Добавьте файлы `SNMPTrapSender.h` и `SNMPTrapSender.cpp` в ваш Arduino-проект.

## API

### Конструктор
```cpp
SNMPTrapSender(const char* ip, int port, bool useV2c = true);
```
- `ip`: IP-адрес SNMP-менеджера.
- `port`: Порт SNMP-менеджера (по умолчанию 162 для ловушек).
- `useV2c`: Флаг версии SNMP (по умолчанию `true` для v2c).

### Методы

#### `void setCommunity(const String& comm);`
Устанавливает строку сообщества SNMP.
- `comm`: Строка сообщества, используемая для аутентификации.

#### `void setOID(const String& oidStr);`
Задает OID (Object Identifier) для SNMP-трапа.
- `oidStr`: OID в строковом формате, например, "1.3.6.1.4.1.4976.1.1".

#### `void setEnterpriseOID(const String& oidStr);`
Задает OID предприятия (используется в SNMP v1).
- `oidStr`: OID предприятия в строковом формате.

#### `bool sendTrap(int value);`
Отправляет SNMP-трап с целочисленным значением.
- `value`: Целочисленное значение, включаемое в ловушку.
- Возвращает `true`, если передача успешна, иначе `false`.

#### `bool sendTrapData(std::function<int()> dataFunction);`
Отправляет SNMP-трап с динамически вычисляемым значением.
- `dataFunction`: Функция, возвращающая целочисленное значение для отправки.
- Возвращает `true`, если передача успешна, иначе `false`.

#### `void encodeOID(uint8_t* buf, int &offset, const char* oid);`
Кодирует OID в SNMP-пакет.
- `buf`: Буфер для хранения закодированного OID.
- `offset`: Текущий сдвиг в буфере.
- `oid`: OID для кодирования.

#### `void encodeInteger(uint8_t* buf, int &offset, int value);`
Кодирует целое число в SNMP-пакет.
- `buf`: Буфер для хранения закодированного числа.
- `offset`: Текущий сдвиг в буфере.
- `value`: Целое число для кодирования.

## Пример использования

```cpp
#include <WiFi.h>
#include "SNMPTrapSender.h"

const char* ssid = "Ваш_SSID";
const char* password = "Ваш_пароль";
SNMPTrapSender snmpSender("192.168.1.100", 162);

// Функция чтения напряжения с аналогового пина ESP32
int readVoltage() {
    return analogRead(34); // Читаем с ADC пина 34
}

void setup() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    
    snmpSender.setCommunity("public");
    snmpSender.setOID("1.3.6.1.4.1.4976.1.1");
    
    // Отправляем данные в сыром виде
    snmpSender.sendTrap(analogRead(34));
    
    // Отправляем данные через обработчик
    snmpSender.sendTrapData(readVoltage);
}

void loop() {
    delay(5000);
}
```

## Примечания
- Убедитесь, что ваш ESP32 подключен к той же сети, что и SNMP-менеджер.
- IP-адрес цели должен быть адресом SNMP-менеджера.
- Используйте SNMP-отладочные инструменты для проверки приема ловушек.

## Лицензия
Эта библиотека выпущена под лицензией MIT.

