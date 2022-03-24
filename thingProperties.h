#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

#include "arduino_secrets.h"

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

void onMoistureChange();
void onHumidityChange();
void onTemperatureChange();


float moisture;
float humidity;
float temperature;

void initProperties(){
  ArduinoCloud.addProperty(moisture, READWRITE, 30 * SECONDS, onMoistureChange);
  ArduinoCloud.addProperty(humidity, READWRITE, 30 * SECONDS, onHumidityChange);
  ArduinoCloud.addProperty(temperature, READWRITE, 30 * SECONDS, onTemperatureChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
