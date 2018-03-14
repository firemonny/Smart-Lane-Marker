/*  HC12 smart lane marker psuedo car signal module 
    By Nung-Shun Chou
    
    Wire HC-12 "RXD" pin to Pin 4
    Wire HC-12 "TXD" pin to Pin 5
    Wire HC-12 "Set" pin to Pin 6
    
    Use  a 1 uF capacitor for decouping.
    Upload code to two Arduinos connected to two computers.
    
    Transceivers must be away for 1.5 meter
   
 */
 #include <SoftwareSerial.h>
const byte HC12RxdPin = 4;                  // Recieve Pin 4 on HC-12
const byte HC12TxdPin = 5;                  // Transmit Pin 5 on HC-12
const byte HC12SetPin = 6;                  // Set Pin 6 on HC-12 
unsigned long timer = millis();
int counter =0;

int datacounter;
const int numChars = 32;
char receivedChars[numChars];

boolean ReceiveInProgress = false;
int ndx = 0;
char startMarker = '<';
char endMarker = '>';
char rc;

boolean newData = false;


SoftwareSerial HC12(HC12TxdPin,HC12RxdPin);

void setup() {
   pinMode(HC12SetPin, OUTPUT);                  // Output High for Transparent / Low for Command
  digitalWrite(HC12SetPin, HIGH);               // Enter Transparent mode
  delay(2000);                                    // 80 ms delay before operation per datasheet
  Serial.begin(4800);                       // Open serial port to computer
  HC12.begin(4800);                         // Open serial port to HC12
  Serial.println("Car Module Start");
}

void loop() 
{
  counter++;
    if (counter>=10000){           
    HC12.print("<CAR>");
    counter =0;
    }// Send that data to serial

  while (HC12.available() > 0 && newData == false) {
    rc = HC12.read();

    if (ReceiveInProgress == true){
      if (rc != endMarker){
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars){
          ndx = numChars -1;
        }
      }
      else {
        receivedChars[ndx] = '\0';
        ReceiveInProgress = false;
        ndx = 0;
        newData = true;
        datacounter++;
      }
    }

    else if (rc == startMarker) {
      ReceiveInProgress = true;
    }
  }

  if (newData == true) {
    String temp = String(receivedChars);
    String message = "Received:" + temp;
    Serial.println(message);
    receivedChars[0]='\0';
    newData = false;
  } 
}
