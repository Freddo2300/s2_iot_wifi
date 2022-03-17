#include <ThingerWiFiNINA.h>

#include <WiFi.h>
#include <WiFiNINA.h>

#include <DHT.h>

#include "thingProperties.h"

// create this file in your local repository and don't push to remote
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;        // your network SSID (name) 
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

#define THINGER_SERIAL_DEBUG
#define THINGER_USE_STATIC_MEMORY
#define THINGER_STATIC_MEMORY_SIZE 512

ThingerWiFiNINA thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // connect to thinger.io
    thing.add_wifi(SSID, pass);
    // start DHT sensor
    dht.begin();

    thing["HumTemp"] >> [](pson &out) {
      out["Humidity"] = dht.readHumidity();
      out["Temperature"] = dht.readTemperature();
    };
    // wait 10 seconds for connection:
    delay(10000);
  }
  
  // defined in thingProperties.h
  initProperties();

  // connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
	
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");

  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");
}

void loop() {
  // put your main code here, to run repeatedly:
  // check the network connection once every 10 seconds:
 delay(10000);

 ArduinoCloud.update();

 thing.handle();
 thing.write_bucket("first_bucket", "HumTemp");
}

void printData() {
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}

void getHumidityAndTemperature() {
 float humidity = dht.readHumidity();
 float temperature = dht.readTemperature();

 if(isnan(humidity) || isnan(temperature)) {
  Serial.println("Failed to read from DHT sensor");
  return;  
  }

 Serial.print("Humidity: ");
 Serial.print(humidity);
 Serial.print(" %\t");
 Serial.print("Temperature");
 Serial.print(temperature);
 Serial.print(" *C");
}
