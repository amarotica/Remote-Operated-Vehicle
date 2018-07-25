//-------Written by Taylor Amarotico-------//
// Remote Operated Vehicle Receiver //


char SoftwareVersion = "v1.01";



// transmissionArray Variable Inputs Declaration //
  byte transmissionArray[12];
  int Rmotorvalue = transmissionArray[1];
  int Lmotorvalue = transmissionArray[2];
  int Rdirectionvalue = transmissionArray[3];
  int Ldirectionvalue = transmissionArray[4];  
  int Ebrakevalue = transmissionArray[5];
  int Lightsvalue = transmissionArray[6];
  int Highbeamsvalue = transmissionArray[7];
  int JSAXvalue = transmissionArray[8];
  int JSAYvalue = transmissionArray[9];
  int JSBvalue = transmissionArray[10];
  int JSABvalue = transmissionArray[11];
  int VerificationValue = transmissionArray[12];
  
// Pin and Constant Declaration //
  const int Rmotor = 2;
  const int Lmotor = 3;
  const int Rdirection = 4;
  const int Ldirection = 5;
  const int Ebrake = 12;
  const int Lights = 6;
  const int Highbeams = 7;
  const int JSAX = 8;
  const int JSAY = 9;
  const int JSB = 10;
  const int JSAB = 11;
  
void setup() {

/* // Only used when debugging.
// Debugging //
  Serial.begin(9600);
  Serial.println("System Initializing");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.println(".");
  delay(500);
  Serial.println(SoftwareVersion);
  delay(250);
*/
// Output Setup //
  pinMode(Rmotor, OUTPUT);
  pinMode(Lmotor, OUTPUT);
  pinMode(Rdirection, OUTPUT);
  pinMode(Ldirection, OUTPUT);
  pinMode(Ebrake, OUTPUT);
  pinMode(Lights, OUTPUT);
  pinMode(Highbeams, OUTPUT);
  pinMode(JSAX, OUTPUT);
  pinMode(JSAY, OUTPUT);
  pinMode(JSB, OUTPUT);
  pinMode(JSAB, OUTPUT);
//  Serial.println("Outputs Initialized");
  delay(250);
// Safe Initialize //
  analogWrite(Rmotor, 0);
  analogWrite(Lmotor, 0);
  digitalWrite(Rdirection, 0);
  digitalWrite(Ldirection, 0);
  digitalWrite(Ebrake, 0);
  digitalWrite(Lights, 0);
  digitalWrite(Highbeams, 0);
  analogWrite(JSAX, 0);
  analogWrite(JSAY, 0);
  digitalWrite(JSB, 0);
  digitalWrite(JSAB, 0);
//  Serial.println("Safe Output Initialized");
  delay(250);
  
}

void loop() {

  receiverModule();

  enableModule();
  
}


void enableModule() {       // This module tests to see if emergency brake is tripped and if so, disables actions and writes motors to 0/255 duty cycle

  if (Ebrakevalue == 1){
    analogWrite(Rmotor, 0);
    analogWrite(Lmotor, 0);
    digitalWrite(Ebrake, HIGH);
    delay(1000);
  }
  else {
    actionsModule();
  }
  
//----------------------End of enableModule function----------------------//
}

void actionsModule() {        // This module writes motor and accesory values to output pins via PWM and digital on/off
  
  analogWrite(Rmotor, Rmotorvalue);
  analogWrite(Lmotor, Lmotorvalue);
  digitalWrite(Rdirection, Rdirectionvalue);
  digitalWrite(Ldirection, Ldirectionvalue);
  digitalWrite(Lights, Lightsvalue);
  digitalWrite(Highbeams, Highbeamsvalue);
  analogWrite(JSAX, JSAXvalue);
  analogWrite(JSAY, JSAYvalue);
  digitalWrite(JSB, JSBvalue);
  digitalWrite(JSAB, JSABvalue);
  
//----------------------End of actionsModule function----------------------//
}



void receiverModule() {       // This module takes in the data from the LoRa module and converts it into useable values for the enableModule and actionsModule

  byte state = lora.receive(transmissionArray, 12);
  
  if (state == ERR_NONE) {
    // packet was successfully received
    Serial.println("success!");

    // print data of the packet
    Serial.print("Data:\t\t");
//    Serial.println(transmissionArray); 
    
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
//----------------------End of receiverModule function----------------------//
}

