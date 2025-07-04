#ifndef SIMPLE_SYSLOG_H
#define SIMPLE_SYSLOG_H

#define SIMPLE_SYSLOG_VERSION "0.1.3"

#if defined (ESP8266)
  #include <ESP8266WiFi.h>
  #include <pgmspace.h>
#elif defined (ESP32)
  #include <WiFi.h>
  #include <pgmspace.h>
#else
  #error SimpleSyslog requires an ESP board. Disabling for non-supported board
#endif

#include <Arduino.h>
#include <WiFiUdp.h>

#define PRI_EMERGENCY 0
#define PRI_ALERT     1
#define PRI_CRITICAL  2
#define PRI_ERROR     3
#define PRI_WARNING   4
#define PRI_NOTICE    5
#define PRI_INFO      6
#define PRI_DEBUG     7

#define FAC_USER      1
#define FAC_LOCAL0    16
#define FAC_LOCAL1    17
#define FAC_LOCAL2    18
#define FAC_LOCAL3    19
#define FAC_LOCAL4    20
#define FAC_LOCAL5    21
#define FAC_LOCAL6    22
#define FAC_LOCAL7    23

class SimpleSyslog {
  WiFiUDP SimpleSyslog_udp;

public:
  SimpleSyslog(const char* hostname, const char* app, const char* server, uint16_t port = 514, uint16_t max_packet_size = 128) {
	this->_hostname        = hostname;
	this->_app             = app;
	this->_server          = server;
	this->_port            = port;
	this->_max_packet_size = max_packet_size;

  }

  // strings from RAM
  void printf(uint8_t facility, uint8_t severity, const char* format, ...) {
    uint8_t priority = (8 * facility) + severity;

    va_list args;
    va_start(args, format);
    char buf[_max_packet_size];
    vsnprintf(buf, _max_packet_size, format, args);
    va_end(args);

    uint8_t buffer[_max_packet_size];
    int len = snprintf((char*)buffer, _max_packet_size, "<%d>%s %s: %s", priority, _hostname, _app, buf);

    SimpleSyslog_udp.beginPacket(_server, _port);
    SimpleSyslog_udp.write(buffer, len);
    SimpleSyslog_udp.endPacket();
  }

  // strings from F()
  void printf(uint8_t facility, uint8_t severity, const __FlashStringHelper* format, ...) {
    uint8_t priority = (8 * facility) + severity;

    va_list args;
    va_start(args, format);
    char buf[_max_packet_size];
    vsnprintf_P(buf, _max_packet_size, (PGM_P)format, args);
    va_end(args);

    uint8_t buffer[_max_packet_size];
    int len = snprintf((char*)buffer, _max_packet_size, "<%d>%s %s: %s", priority, _hostname, _app, buf);

    SimpleSyslog_udp.beginPacket(_server, _port);
    SimpleSyslog_udp.write(buffer, len);
    SimpleSyslog_udp.endPacket();
  }

private:
  const char* _hostname;
  const char* _app;
  const char* _server;
  uint16_t _port;
  uint16_t _max_packet_size;
};

#endif
