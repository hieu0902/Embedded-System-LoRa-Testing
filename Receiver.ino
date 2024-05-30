#include <SPI.h>
#include <LoRa.h>

float calculateBitRate(long bandwidth, int spreadingFactor, int codingRate) {
  return (float)bandwidth / (1 << spreadingFactor) * (codingRate - 4) / codingRate;
}
int countBitErrors(String sentMessage, String receivedMessage) {
  int bitErrors = 0;
  for (int i = 0; i < sentMessage.length(); i++) {
    char sentChar = sentMessage.charAt(i);
    char receivedChar = receivedMessage.charAt(i);
    for (int j = 0; j < 8; j++) {
      if (((sentChar >> j) & 0x01) != ((receivedChar >> j) & 0x01)) {
        bitErrors++;
      }
    }
  }
  return bitErrors;
}

void setup() {
  int spreadingFactor = 7;     
  long bandwidth = 125E3;      
  int codingRate = 5; 

  long frequency = 915E6;
  uint8_t syncword = 0x34;
  Serial.begin(9600);
  
  while (!Serial);
  
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  // Set LoRa parameters

  LoRa.setFrequency(frequency);       // Set the frequency (example: 915 MHz)
  LoRa.setSpreadingFactor(spreadingFactor);     // Set the spreading factor (range: 6 - 12)
  LoRa.setSignalBandwidth(bandwidth); // Set the signal bandwidth (range: 7.8E3 - 500E3 Hz)
  LoRa.setCodingRate4(codingRate);         // Set the coding rate (range: 5 - 8)
  LoRa.setSyncWord(syncword);         // Set the sync word (0x12 is the default value)

  Serial.println("LoRa configuration done.");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
//    Serial.print("Received packet '");
//
//    // read packet
//    while (LoRa.available()) {
//      Serial.print((char)LoRa.read());
//    }
    String receivedMessage = "";
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }
    Serial.println("Received: " + receivedMessage);

    // Bit rate
    float bitRate = calculateBitRate(bandwidth, spreadingFactor, codingRate);
    Serial.print("Bit Rate: ");
    Serial.print(bitRate);
    Serial.println(" bps");
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print("' with SNR ");
    Serial.println(LoRa.packetSnr());
    Serial.print("' with Frequency Error ");
    Serial.println(LoRa.packetFrequencyError());
    
    int bitErrors = countBitErrors(sentMessage, receivedMessage);
    int totalBits = sentMessage.length() * 8;
    float ber = (float)bitErrors / totalBits;

    Serial.print("Bit Errors: ");
    Serial.println(bitErrors);
    Serial.print("Total Bits: ");
    Serial.println(totalBits);
    Serial.print("BER: ");
    Serial.println(ber, 6);
  }
}
