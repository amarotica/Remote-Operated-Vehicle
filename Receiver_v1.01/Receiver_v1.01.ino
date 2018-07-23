//-------Written by Taylor Amarotico-------//
// Remote Operated Vehicle Receiver //


char SoftwareVersion = "v1.1";
#include <LoRaLib.h>

SX1276 lora = new LoRa;





  byte transmissionArray{12};
// byte transmissionArray[] = {Rmotor, Lmotor, Rdirection, Ldirection, Ebrakeinput, Lightinput, Highbeaminput, JSAXtransmit, JSAYtransmit, JSBinput, JSABinput, 23};
  int Rmotor = transmissionArray{1};
  int Lmotor = transmissionArray{2};
  int Rdirection = transmissionArray{3};
  int Ldirection = transmissionArray{4};  
  int Ebrakeinput = transmissionArray{5};
  int Lightinput = transmissionArray{6};
  int Highbeaminput = transmissionArray{7};
  int JSAXtransmit = transmissionArray{8};
  int JSAYtransmit = transmissionArray{9};
  int JSBinput = transmissionArray{10};
  int JSABinput = transmissionArray{11};



void setup() {
  Serial.begin(9600);

  Serial.print(F("LoRa Initializing... "));
  byte state = lora.begin();
  if (state == ERR_NONE) {
    Serial.println(F("LoRa Initialized!"));
  } else {
    Serial.print(F("failed, code 0x"));
    Serial.println(state, HEX);
    while (true);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}

void receiverModule() {

  byte state = lora.receive(transmissionArray, 12);
  
  if (state == ERR_NONE) {
    // packet was successfully received
    Serial.println("success!");

    // print data of the packet
    Serial.print("Data:\t\t");
    Serial.println(transmissionArray);

    // print measured data rate
    Serial.print("Datarate:\t");
    Serial.print(lora.dataRate);
    Serial.println(" bps");

    // print RSSI (Received Signal Strength Indicator) 
    // of the last received packet
    Serial.print("RSSI:\t\t");
    Serial.print(lora.lastPacketRSSI);
    Serial.println(" dBm");

    // print SNR (Signal-to-Noise Ratio) 
    // of the last received packet
    Serial.print("SNR:\t\t");
    Serial.print(lora.lastPacketSNR);
    Serial.println(" dBm");

  } else if (state == ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println("timeout!");

  } else if (state == ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println("CRC error!");

  }
  
}

