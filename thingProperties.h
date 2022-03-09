#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

void onTrustChange();

bool trust;

void initProperties(){

  ArduinoCloud.addProperty(trust, READWRITE, ON_CHANGE, onTrustChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
