//-------Written by Taylor Amarotico-------//
// Remote Operated Vehicle Receiver //

char SoftwareVersion = "v1.02";

// transmissionArray Variable Inputs Declaration //
int transmissionArray[12];

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
  delay(50);
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
  delay(50);
  
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

  String str = "";
  //String str ="1233,4146,7389,1511,21213,14315,16117,1819,2021,2223,2425,23";
  
  while (Serial.available() > 0) {
    str += (char)Serial.read();
    delay(5);
  }

  if (str != "") {
    int indexArray[12];
    indexArray[0]=str.indexOf(",");
    for(int i=1;i<=10;i++){
      indexArray[i]=str.indexOf(",",(indexArray[i-1] +1));
      }
      
  
transmissionArray[1]=str.substring(0,indexArray[0]).toInt();
for(int i=2;i<=11;i++){
  transmissionArray[i]=str.substring(indexArray[i-2]+1,indexArray[i-1]).toInt();
  }
transmissionArray[12]=str.substring(indexArray[10]+1).toInt();

for(int i=1;i<=12;i++){
  //Serial.println(transmissionArray[i]);
  }
  
   // while(1);
  }
  //----------------------End of receiverModule function----------------------//
}
