/*  HC12 Smartlane marker module 
    By Nung-Shun Chou
    
    Wire HC-12 "RXD" pin to Pin 4
    Wire HC-12 "TXD" pin to Pin 5
    Wire HC-12 "Set" pin to Pin 6
    
    Use  a 1 uF capacitor for decouping.
    Upload code to two Arduinos connected to two computers.
    
    Transceivers must be away for 1.5 meter

   Update version V1.0.4 Add the Timer feature.
   TimerOne used the online free source timer.
   Please include Timer-r11 libray 
   https://www.arduinolibraries.info/libraries/timer-one
 */
 
#include <SoftwareSerial.h>
#include <TimerOne.h>
enum MicorcontrollerSM {NON_DETECTING, DETECTING,POSTDETECTING};

// Global varaiable declaration
const byte HC12RxdPin = 4;                  // Recieve Pin 4 on HC-12
const byte HC12TxdPin = 5;                  // Transmit Pin 5 on HC-12
const byte HC12SetPin = 6;                  // Set Pin 6 on HC-12 
char HC12moduleinformation[] = "0001";
unsigned long interval = 500;
int counter = 0;
int countersignal = 0;
char HC12ByteIn; 
boolean HC12End = false;
String HC12ReadBuffer = "";                     // Read/Write Buffer 1 for HC12
String SerialReadBuffer = "";                   // Read/Write Buffer 2 for Serial
char startMarker = '<';
char endMarker = '>';
const int numChars = 32;
char receivedChars[numChars];
boolean newData = false;
boolean ReceiveDataInProgress = false;
int ndx = 0;
char rc;
String CAR = "CAR\0";
bool TimerStart=false;



MicorcontrollerSM CurrentMicrocontroller_SM = NON_DETECTING;


bool Detect_Vehicle = false;
SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port

void setup() {
  HC12ReadBuffer.reserve(64);                   // Reserve 64 bytes for Serial message input
  SerialReadBuffer.reserve(64);                 // Reserve 64 bytes for HC12 message input
  Timer1.initialize(100000);                    //Initialize Timer
  Timer1.setPeriod(4000000);                    //Set period to 4000000 microsecond
  pinMode(HC12SetPin, OUTPUT);                  // cofigure the HC-12 Set ping 6 to be output mode Command
  digitalWrite(HC12SetPin, HIGH);
  delay(2000);
  Serial.begin(4800);                       // Open serial port to computer
  HC12.begin(4800);// Open serial port to HC12
  Serial.println("Module Start");
}

void loop() 
{
  switch (CurrentMicrocontroller_SM)
  {
    case NON_DETECTING:
    if(HC12.available()>0 && newData == false)
    {
      rc = HC12.read(); 
      if (ReceiveDataInProgress == true)
      {
        if  (rc != endMarker)
        {
          receivedChars[ndx] = rc;
          ndx++;
          if (ndx >= numChars)
          {
            ndx = numChars-1;  
          }
        }
        else
        {
          receivedChars[ndx] = '\0';
          ReceiveDataInProgress = false;
          HC12End = true;
          ndx = 0;
          newData = true;
        }
      }
      else if (rc == startMarker)
      {
        ReceiveDataInProgress = true;
      }
    }
    if(HC12End){
      String verify(receivedChars);
      Detect_Vehicle = (!verify.compareTo(CAR));
      //Serial.print(verify);
      if(Detect_Vehicle == true) CurrentMicrocontroller_SM = DETECTING;   
      // Serial.println("Detecting");
      HC12End = false;   
    }
    break;
    
    case DETECTING:
    counter++;
    if (counter >= 3)
    {
      HC12.print("<C0002>");
      counter = 0;
      countersignal++;
    }
      if (countersignal >=8)
            { 
              countersignal=0;
              Detect_Vehicle = false;
            CurrentMicrocontroller_SM = POSTDETECTING;
           // Serial.println("Go to non-detecting");
            
            }
     break;
     case POSTDETECTING:
      if(TimerStart == false)
      {
        TimerStart = true;
        Timer1.start();
        }
      else{
        unsigned long currenttime = Timer1.read();
         if(currenttime>=800000)
             {
              Timer1.stop();
              Timer1.setPeriod(2000000);
              TimerStart=false;
              CurrentMicrocontroller_SM = NON_DETECTING;
              }
        }
        break;
     default:
     Serial.println("Should not get to here");
     break;
  }
   if(newData == true)
    {                     // If Arduino's HC12 rx buffer has data
      String message = String(receivedChars);
      if(!message.compareTo(CAR)){
      Serial.println(receivedChars);}// Send the data to the computer
      receivedChars[0] = "\0";
      newData = false;
    }
    
}

