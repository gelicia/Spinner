#include <Arduino.h>
#include "TimerOne.h"

const int PinCLK=7;                   // Used for generating interrupts using CLK signal
const int PinDT=8;                    // Used for reading DT signal
const int PinSW=9;                    // Used for the push button switch
bool bLastCLK=false;

const int PinSig=4;    //rpm input
const int PinZF=2;     //direction
const int PinVR=3;     //speed PWM
const int PinEL=5;     //enable
const int PinFoot=A1;  // Used for foot pedal

int prevFootRead = 0;
int maxFootRPM = 300; // the higher this is, the harder it is to do subtle footpedal speed control (range 0-1000)
int maxSpeed = 80; // range 0-255
bool lastDir = true; // save the direction it was spinning last when stopped with the knob, so the footpedal can use the same direction when restarting

void setup() {
    Serial.begin(9600);
    Serial.println("Spinner");

    pinMode(PinSig, INPUT);
    pinMode(PinZF, OUTPUT);
    pinMode(PinVR, OUTPUT);
    pinMode(PinEL, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(PinCLK,INPUT);
    pinMode(PinDT,INPUT);  
    pinMode(PinSW,INPUT_PULLUP);
    pinMode(PinFoot, INPUT);
    bLastCLK=digitalRead(PinCLK);
    prevFootRead=analogRead(PinFoot);    

    Timer1.initialize(100);  // 10 us = 100 kHz
    Timer1.attachInterrupt(stepperAdvance);
    Serial.println("init.done");
}

unsigned int cnt=0;
int iRPM=0;
bool footControl=false;

void loop() {
  cnt++;
  digitalWrite(LED_BUILTIN, ((cnt/10)&1)!=0);

  const int iMin=10;
  // iRPM is -1000 - 1000, speed is always positive
  int iR=iRPM>=0?iRPM:-iRPM;
  //speed is between 0-255
  int speed = iMin + (long(iR)*(255-iMin)/1000);
  speed = constrain(speed, 0, maxSpeed);

  Serial.print("speed write: ");
  Serial.print(iRPM!= 0 ? speed: 0);
  Serial.print(", off: ");
  Serial.println(iRPM!=0);
    
  analogWrite(PinVR, iRPM!= 0 ? speed: 0);
  digitalWrite(PinEL, iRPM!=0); //Stop if iRPM is 0 : HIGH = go, LOW = stop
  digitalWrite(PinZF, iRPM>0); // Direction based on iRPM +/- :  HIGH = forward, LOW = back

// footpedal range is 83-1020
// iRPM range is -1000-1000
// foot pedal doesnt change direction, just speed
  int footRead = analogRead(PinFoot);
  // if weve been using the knob to control but the footpedal speed changes, we go to foot to control
  // this has to fall outside a +-10 threshhold because the footpedal has some jitter
  if (!footControl && ((footRead > (prevFootRead + 10)) || (footRead < (prevFootRead - 10)))) {
    footControl = true;
  }
  
  if (footControl) { 
    prevFootRead = footRead;
    // if we last stopped it by pressing the button, use last direction
    bool posDir = iRPM == 0 ? lastDir : iRPM>=0;
    // intentionally let more values count as min or max
    int speed = map(footRead, 990, 75, 0, maxFootRPM);
    speed = constrain(speed, 0, maxFootRPM);   
    // footPedal will never change direction, so get prev direction 
    if (!posDir) {
      speed = speed * -1;
    }   
    Serial.print("foot iRPM: ");
    Serial.println(speed); 
    /*Serial.print(", foot val: "); 
    Serial.println(footRead); */     
    iRPM = speed;
  }

  delay(10);
}

unsigned int iClick=0;

void stepperAdvance() {
  if (digitalRead(PinSW) == LOW) {
    Serial.print("button press: ");
    Serial.println(iClick);
    iClick++;
    if(iClick==10) { // stop after 10 counts of this
      footControl = false; // ignore footpedal logic
      lastDir = iRPM > 0;
      iRPM = 0;
    }      
  } else {
    iClick=0;

    bool bCLK=digitalRead(PinCLK);
    if(bLastCLK!=bCLK) {
      Serial.println("knob turn");
      footControl = false; // ignore footpedal logic
      bLastCLK=bCLK;
      bool bDT=digitalRead(PinDT);
      if(bDT!=bCLK) {
        iRPM+=20;
        if(iRPM>1000) iRPM=1000;
      } else {
        iRPM-=20;
        if(iRPM<-1000) iRPM=-1000;
      }

      Serial.print("knob iRPM: ");
      Serial.println(iRPM);
    }
  }
}
