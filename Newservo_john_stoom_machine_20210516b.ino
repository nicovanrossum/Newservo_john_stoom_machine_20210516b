
// Controlling a servo position using a potentiometer (variable resistor) 
//#include <Servo.h> 
#include <ServoTimer2.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
// Servo myservo;  // create servo object to control a servo 
int volume = 12;  // outputlevel for speaker 0 <--> 30;
int pushingTimeShort = 150; // short push button is 100 ms. (function next /pre)
int pushingTimeLong = 20000; // short push button is 500 ms. (function vol+ / vol-)
int swingEnd = 180; // servo goes to this position
int swingStart = 10; // servo starts from this position
int stapTijd = 22;    // hoe snel gaat de beweging 
int wachtTijdtop = 800;  // tijd voor het omschakelen
int wistleCounterX = 500;
int wistleCounter = 500; // every 500 strokes one wistle on the top of the stroke
const int IO1Pin = 2;
const int IO2Pin = 3;
const int busyPin = 4; // busy signal from PFplayer
// const int servo1 = 9; // the servo is connected to pin 9
#define servo1Pin 9
ServoTimer2 servoBalance;    // declare variables for up to eight servos

void setup() 
   {
    servoBalance.attach(servo1Pin); 
    pinMode(IO1Pin, OUTPUT);
    pinMode(IO2Pin, OUTPUT);
    pinMode(busyPin, INPUT);
    digitalWrite(IO1Pin, HIGH);
    digitalWrite(IO2Pin, HIGH);
    // myservo.attach(servo1);  // attaches the servo on pin 9 to the servo object
    mySoftwareSerial.begin(9600); // enable serial communication at 9600 baud
    Serial.begin(9600);
    Serial.println();
    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

    if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while(true);
      }
   Serial.println(F("DFPlayer Mini online."));
   myDFPlayer.volume(volume); 
    // start DFplayer
    if (busyPin == HIGH)
       {
       digitalWrite(IO1Pin, LOW);
       delay(pushingTimeShort);
       digitalWrite(IO1Pin, HIGH);
       delay(pushingTimeShort);
       } 
     }

// this function just increments a value until it reaches a maximum 
int incPulse(int val, int inc){
   if( val + inc  > 2000 )
      return 1000 ;
   else
       return val + inc;  
}

     
void loop() {
    Serial.println("loop");
    delay(pushingTimeLong);
    int val;
    val = incPulse( servoBalance.read(), 1);
    servoBalance.write(val);
    delay(10); 
}
 
void NGloop() 
    {
     // push button IO1 short for function 'next track'
     // second track (0002.mp3) is sound of steam engine
     digitalWrite(IO1Pin, LOW);
     delay(pushingTimeShort);
     digitalWrite(IO1Pin, HIGH);
     delay(pushingTimeShort);
     // delay(pushingTimeShort) 
     //  the servo (value between 0 and 180) 
     for (int i = swingStart ; i<= swingEnd; i+=1)
       {
       // myservo.write(i);     // sets the servo position according to the scaled value 
       delay(stapTijd); 
       }
      delay (wachtTijdtop);
     
     // push button IO1 short for function 'next track'
     digitalWrite(IO1Pin, LOW);
     delay(pushingTimeShort);
     digitalWrite(IO1Pin, HIGH);
     delay(pushingTimeShort);
     for (int i = swingEnd; i >= swingStart; i-=1) 
         {
         // myservo.write(i);     // sets the servo position according to the scaled value 
         delay(stapTijd);  
         }
     delay (wachtTijdtop);
     /*
     digitalWrite(IO2Pin, LOW);
     delay(pushingTimeShort);
     digitalWrite(IO2Pin, HIGH);
     delay(pushingTimeShort);
     digitalWrite(IO2Pin, LOW);
     delay(pushingTimeShort);
     digitalWrite(IO2Pin, HIGH);
     delay(pushingTimeShort);
     */
     // push button IO2 short twice for function 'previous track'
     // goes back to second track (0001.mp3)
     // digitalWrite(IO2Pin, LOW);
     // delay(pushingTimeShort);
     // digitalWrite(IO2Pin, HIGH);
     // delay (wachtTijdtop);
}
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
} 
