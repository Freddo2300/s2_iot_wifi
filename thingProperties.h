#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

#include "arduino_secrets.h"

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

void initProperties(){

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
