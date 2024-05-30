#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  int spreadingFactor = 7;     
  long bandwidth = 125E3;      
  int codingRate = 5; 
  int TxPower = 14;
  long frequency = 915E6;
  uint8_t syncword = 0x34;
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  // Set LoRa parameters
  LoRa.setTxPower(TxPower);            // Set the transmission power (range: 2 - 20 dBm)
  LoRa.setFrequency(frequency);       // Set the frequency (example: 915 MHz)
  LoRa.setSpreadingFactor(spreadingFactor);     // Set the spreading factor (range: 6 - 12)
  LoRa.setSignalBandwidth(bandwidth); // Set the signal bandwidth (range: 7.8E3 - 500E3 Hz)
  LoRa.setCodingRate4(codingRate);         // Set the coding rate (range: 5 - 8)
  LoRa.setSyncWord(syncword);         // Set the sync word (0x12 is the default value)

  Serial.println("LoRa configuration done.");
}

void loop() {
  
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
