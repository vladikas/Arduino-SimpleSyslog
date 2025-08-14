# Arduino SimpleSyslog

Easily add remote syslog capabilities to your ESP32/ESP8266 projects.

## Installation

Clone this repo to your Arduino libraries directory. On Linux this is `~/Arduino/libraries/`.
Alternately you can just drop `SimpleSyslog.h` in the same directory as your `.ino` script.

## Usage

Include the SimpleSyslog library and create a global `syslog` object.

### Using the default constructor + `begin()`

```cpp
#include <SimpleSyslog.h>

SimpleSyslog syslog;

void setup() {
  syslog.begin("ArduinoHostname", "ArduinoApp", "192.168.5.222");
}
```

### Using constructor with parameters

```cpp
SimpleSyslog syslog("ArduinoHostname", "ArduinoApp", "192.168.5.222");
```

Optional settings are port (default: 514) and maximum packet size (default: 128).

```cpp
SimpleSyslog syslog("ArduinoHostname", "ArduinoApp", "192.168.5.222", 5140, 400);
```

### Setters (change parameters on the fly)

```cpp
syslog.setHostname("ESP8266");
syslog.setApp("PumpCtrl");
syslog.setServer("192.168.1.200", 1514);
```

### Checking readiness

```cpp
if (syslog.isReady()) {
    syslog.printf(FAC_LOCAL0, PRI_INFO, "Ready to send syslog messages");
}
```

### Sending syslog messages

Supports both RAM strings and flash strings using `F()`:

```cpp
// RAM string
syslog.printf(FAC_LOCAL7, PRI_INFO, "This is a simple LOCAL7.INFO syslog packet");

// Flash string
syslog.printf(FAC_USER, PRI_DEBUG, F("Uptime: %0.1f minutes"), millis() / 60000.0f);
```

### Valid facilities:

* `FAC_USER`
* `FAC_LOCAL0`
* `FAC_LOCAL1`
* `FAC_LOCAL2`
* `FAC_LOCAL3`
* `FAC_LOCAL4`
* `FAC_LOCAL5`
* `FAC_LOCAL6`
* `FAC_LOCAL7`

### Valid severities:

* `PRI_EMERGENCY`
* `PRI_ALERT`
* `PRI_CRITICAL`
* `PRI_ERROR`
* `PRI_WARNING`
* `PRI_NOTICE`
* `PRI_INFO`
* `PRI_DEBUG`

## Configuring `rsyslog` to receive messages

You will need to configure your `/etc/rsyslog.conf` to accept incoming UDP syslog messages. Add these lines to your config and restart `rsyslog`:

```
$ModLoad imudp
$UDPServerRun 514
```

You may want to configure a specific rule to handle your messages:

```
# /etc/rsyslog.d/arduino.conf
template(name="arduino" type="string" string="/var/log/arduino.log")

if ($syslogfacility-text == "local7" or $syslogfacility-text == "user") then {
    action(type="omfile" DynaFile="arduino" FileCreateMode="0644")
    stop
}
```

## Based on

Borrowed from [jerryr/EspSyslog](https://github.com/jerryr/EspSyslog) and improved upon.

## New Features in v0.1.7

* **Default Constructor**: allows global object declaration without parameters.
* **`begin()` Method**: full initialization after object creation.
* **Setters**: change hostname, app, server, and port on the fly.
* **`isReady()` Method**: ensures hostname, app, server are set and Wi-Fi is connected before sending.
* **Safe `printf()` Methods**: supports both RAM and flash strings (`F()`), automatically checks Wi-Fi readiness.
* **Internal `sendPacket()` Function**: centralizes UDP packet sending and avoids code duplication.
