#include <BTLE.h>
#include <SPI.h>
#include <RF24.h>
#include <DHT.h>

RF24 radio(9,10);
BTLE btle(&radio);


#define DHTPIN A0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("BTLE temperature sender");

  // 8 chars max
  btle.begin("SimoTemp");
  dht.begin();
}

void loop() {
 float t = dht.readTemperature();

  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = BTLE::to_nRF_Float(t);
  
  if(!btle.advertise(0x16, &buf, sizeof(buf))) {
    Serial.println("BTLE advertisement failure");
  }
  btle.hopChannel();
  
  delay(1000);

}
