#include <RTCZero.h> 
#include <ArduinoLowPower.h> 
#include <SigFox.h> 
#include "MQ135.h" 
#include <DHT.h>

 

#define SOIL_PIN A0

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define anInput  A0 
#define co2Zero  55 
 

//float moisture = 0;
int soil;
int temp;
int hum;


int Count; 
// Your threshold value 
 
int sensorThres = 1; //Cigarette smoke around 300, Butane 400, no measure around 180 
 
void setup() { 
  Serial.begin(9600); 
   while (!Serial); 
   if ( ! SigFox.begin() ) { 
     Serial.println("Error ... rebooting"); 
     NVIC_SystemReset(); 
     while(1); 
   } 
   SigFox.reset(); 
   delay(100); 
   SigFox.debug(); 
   SigFox.end(); 
  Count = 0; 
   // We need to have to time to program the Arduino after a reset 
   // Otherwise it does not respond when in low-power mode 
   Serial.println("Booting..."); 
   delay(5000); 
} 
 
 
void loop() { 
   // put your main code here, to run repeatedly: 
 
     dht.begin();

   int soilVal = readSoil();
   int tempVal = readTemp();
   int humVal = readHum();


/*   
    dht.begin();
    
  int tempVal = dht.readTemperature();  
  int humVal = dht.readHumidity();

*/
 
  Serial.print("DHT - Temp : "); 
  Serial.println(tempVal); 
  Serial.print("DHT - Hum : "); 
  Serial.println(humVal); 
  Serial.print("Pin A0 - soil : "); 
  Serial.println(soilVal); 
  Serial.print("Counter: "); 
  Count = Count+1; 
  Serial.println(Count); 
  // Checks if it has reached the threshold value 
  
  if (tempVal > sensorThres) 
  { 
     SigFox.begin(); 
     SigFox.status(); 
     SigFox.beginPacket(); 
        SigFox.write(soilVal); 
        SigFox.write(tempVal); 
        SigFox.write(humVal); 
     SigFox.endPacket(); 
     SigFox.end();     
    Serial.println("Packet sent"); 
    Serial.println(tempVal); 
     
  } 
  else 
  { 
    Serial.println("Threshold not exceeded"); 
  } 
  
  delay(30000); 
     
   // Wait for 10 minutes. Low Power version (does not wake up from sleep), use delay 
   //LowPower.sleep(1*60*1000); 
}



 int readSoil() {
   
   int succesfullRead = 0;
    
    for (int i = 0; i <= 100; i++) {
      int soilRead = analogRead(SOIL_PIN);
      
      if(readWithError(soilRead)){
        continue;
      }
      
    soil = soil + soilRead;
    succesfullRead = succesfullRead + 1;
    delay(1); 
  }
  
  soil = soil/succesfullRead;
  
  return soil;
}
   
   
int readTemp() {
   
   int succesfullRead = 0;
    
    for (int i = 0; i <= 100; i++) {
      int tempRead = dht.readTemperature();
      
      if(readWithError(tempRead)){
        continue;
      }
      
    temp = temp + tempRead;
    succesfullRead = succesfullRead + 1;
    delay(1); 
  }
  
  temp = temp/succesfullRead;
  
  return temp;
}

 int readHum() {
   
   int succesfullRead = 0;
    
    for (int i = 0; i <= 100; i++) {
      int humRead = dht.readHumidity();
      
      if(readWithError(humRead)){
        continue;
      }
      
    hum = hum + humRead;
    succesfullRead = succesfullRead + 1;
    delay(1); 
  }
  
  hum = hum/succesfullRead;
  
  return hum;
}

boolean readWithError(int value){
  return value >= 10000;
}

