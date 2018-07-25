//-------Written by Taylor Amarotico-------//
// Remote Operated Vehicle Transmitter //

char SoftwareVersion = "v1.01";

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin and Constant Declaration //
  const int JSX = A3; //Joystick pin
  const int JSY = A2; //Joystick pin
  const int JSB = 2; //Joystick button pin
  const int EbrakeSwitch = 3; //Emergency brake switch pin
  const int LightSwitch = 4; //Lighting switch pin
  const int HighbeamSwitch = 5; //Highbeam switch pin
  const int JSAX = A1; //Accesory joystick pin
  const int JSAY = A0; //Accesory joystick pin
  const int JSAB = 6; //Accesory joystick button pin
  const int ActionEnable = 7; //Action enable switch pin
  const int SpeedLimitSwitch = 8; //Speed limiter switch pin
  const int BrakeStatusLED = 9;
  const int LightStatusLED = 10;
  const int HighbeamStatusLED = 11;
  const int SpeedLED = 12;
  const int StatusLED = 13;
  const int SpeedLimitHigh = 100; //Changeable top speed value
  const int SpeedLimitLow = 25; //Changeable low speed value
  
// ------Variable Declaration------ //
// "loop" //
  int ControllerStatus = 0;
// "inputsModule" //
  int JSXinputvalue = 0;
  int JSYinputvalue = 0;
  int JSAXinputvalue = 0;
  int JSAYinputvalue = 0;
  int JSBinput = 0;
  int JSABinput = 0;
  int SpeedLimitinput = 0;
  int Ebrakeinput = 0;
  int Lightinput = 0;
  int Highbeaminput = 0;
// "motor speed calculation"//
  int JSXmapped = 0;
  int JSYmapped = 0;
  int SpeedLimit = 0;
  int V = 0;
  int W = 0;
  int R = 0;
  int L = 0;
// "indicatorsModule"//
  int Counter = 0;
// "transmissionModule" //
int JSAXtransmit = 0;
int JSAYtransmit = 0;
// global global  variables //
  int Rmotor = 0;
  int Lmotor = 0;
  int Rdirection = 0;
  int Ldirection = 0;


void setup()
{
  Serial.begin(9600);
/*  For debugging only
// Debugging //

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
// Input Setup //
  pinMode(JSX, INPUT); //Analog
  pinMode(JSY, INPUT); //Analog
  pinMode(JSB, INPUT); //Digital
  pinMode(EbrakeSwitch, INPUT); //Digital
  pinMode(LightSwitch, INPUT); //Digital
  pinMode(HighbeamSwitch, INPUT); //Digital
  pinMode(JSAX, INPUT); //Analog
  pinMode(JSAY, INPUT); //Analog
  pinMode(JSAB, INPUT); //Digital
  pinMode(ActionEnable, INPUT); //Digital
  pinMode(SpeedLimitSwitch, INPUT); //Digital

//  Serial.println("Inputs Initialized...");
  delay(250);
  
// Output Setup //
  pinMode(BrakeStatusLED, OUTPUT);
  pinMode(LightStatusLED, OUTPUT);
  pinMode(HighbeamStatusLED, OUTPUT);
  pinMode(SpeedLED, OUTPUT);
  pinMode(StatusLED, OUTPUT);

//  Serial.println("Outputs Initialized...");
   delay(250);

}

void loop()
{
  inputsModule(); //All the analog and digital inputs are handled through this function

  indicatorsModule(); //All the LED indicators are handled through this function

  transmissionModule(); //Packs all the data into an array and transmits through Serial LoRa module

  displaysModule(); //Takes stored data from inputs module and system info, and prints important data to four I2C OLED displays via an I2C multiplexer

  ControllerStatus = 1;
}










void transmissionModule()
{
  byte transmissionArray[] = {Rmotor, Lmotor, Rdirection, Ldirection, Ebrakeinput, Lightinput, Highbeaminput, JSAXtransmit, JSAYtransmit, JSBinput, JSABinput, 23}; // I need to send these values to the receiver many times per second. Motor control is the most important to have sent very often but ideally everything sends many times per second
  // Maybe if it can't be sent as an array, it could be sent individually and each thing have an identifier and on the receiver it can run individual functions based on type of value was received
  
  byte state = lora.transmit(transmissionArray, 12);
//create transmission here//
  //----------------------End of transmissionModule function----------------------//
}





void displaysModule()
{
// This function utilizes two 128x32 I2C OLED displays and two 128x64 I2C OLED displays
// The working library is at: https://github.com/adafruit/Adafruit_SSD1306/tree/master/ and The examples work. Even deleting unnecessary stuff from the examples and putting what I need in the code works for me.
// It would be nice for all the displays to startup with the software version from the top of my code right after they initialize. 
// One of the 128x32 will then have static text on the screen
// One of the 128x32 will then have:
// Left---Motor Speed---Right
// Lmotorvalue   Rmotorvalue
// The other two displays (the 128x64 displays) will just have filler text for now. Lets say they should say ("Hello World") in medium font size
//----------------------End of displaysModule function----------------------//
}





void motorcalculationModule()
{
// map values to usable values //
JSXmapped = map(JSXinputvalue,0,1023,-255,255);
JSYmapped = map(JSYinputvalue,0,1023,-255,255);
SpeedLimit = map(SpeedLimitinput,0,1,SpeedLimitLow,SpeedLimitHigh);

// motor speed calculation //
  V = (255-abs(JSXmapped))*(JSYmapped/255)+JSYmapped;
  W = (255-abs(JSYmapped))*(JSXmapped/255)+JSYmapped;
  R = ((V+W)/2)*(100/SpeedLimit);
  L = ((V-W)/2)*(100/SpeedLimit);

// direction control //
  if (R > 0){
    Rdirection = 1;
  }
  if (L > 0){
    Ldirection = 1;
  }
  if (R < 0){
    Rdirection = 0;
  }
  if (L < 0){
    Ldirection = 0;
  }

// motor output conversion //
  Rmotor = abs(R);
  Lmotor = abs(L);

/*
// Debugging //
  Serial.print("Right Motor Speed: ");
  Serial.println(Rmotor);
  Serial.print("Direction: ");
  Serial.println(Rdirection);

  Serial.print("Left Motor Speed: ");
  Serial.println(Lmotor);
  Serial.print("Direction: ");
  Serial.println(Ldirection);  
*/
  
//----------------------End of motorcalculationModule function----------------------//
}


void indicatorsModule()
{
  if (Ebrakeinput == 1) digitalWrite(BrakeStatusLED, HIGH);
  if (Ebrakeinput == 0) digitalWrite(BrakeStatusLED, LOW);
  
  if (Lightinput == 1) digitalWrite(LightStatusLED, HIGH);
  if (Lightinput == 0) digitalWrite(LightStatusLED, LOW);
  
  if (SpeedLimitinput == 1) digitalWrite(SpeedLED, HIGH);
  if (SpeedLimitinput == 0) digitalWrite(SpeedLED, LOW);

  if (Highbeaminput == 1) digitalWrite(HighbeamStatusLED, HIGH);
  if (Highbeaminput == 0) digitalWrite(HighbeamStatusLED, LOW);


  if ((ControllerStatus == 1) && (Counter == 0)){
    digitalWrite(StatusLED, HIGH);
//    Serial.println("Controller Status: OK!");
  }
  if ((ControllerStatus == 0)  && (Counter == 0)){
    digitalWrite(StatusLED, LOW);
//    Serial.println("Controller Status: ERROR");
  }
  if (Counter == 20){
    Counter = 0;
    
  }
  Counter = Counter + 1;
  
//----------------------End of indicatorsModule function----------------------//
}




void inputsModule()
{
JSXinputvalue = analogRead(JSX); //Joystick 1 input value
JSYinputvalue = analogRead(JSY); //Joystick 1 input value
JSAXinputvalue = analogRead(JSAX); //Joystick 2 input value
JSAYinputvalue = analogRead(JSAY); //Joystyick 2 input value
JSBinput = digitalRead(JSB); //Joystick 1 button
JSABinput = digitalRead(JSAB); //Joystick 2 button
SpeedLimitinput = digitalRead(SpeedLimitSwitch); //Speed limit switch
Ebrakeinput = digitalRead(EbrakeSwitch); //Emergency brake switch
Lightinput = digitalRead(LightSwitch); //Auxillary lights switch
Highbeaminput = digitalRead(HighbeamSwitch); //Highbeams light switch

  JSAXtransmit = map(JSAXinputvalue, 0, 1023, 0, 255);
  JSAYtransmit = map(JSAYinputvalue, 0, 1023, 0, 255);
  

motorcalculationModule(); //Runs the motor calculations to feed to next functions

//----------------------End of inputsModule function----------------------//
}
