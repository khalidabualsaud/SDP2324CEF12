
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE 9
#define CSN 10
const uint64_t Virtual_wire = 0xE8E8F0F0E1LL; // Define the transmit Virtual_wire

RF24 radio(CE, CSN); // Create a Radio
int coming[6];  // 6 element coming holding coming readings
int  valueA;




void setup()
{
  Serial.begin(9600);
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  radio.begin();
  radio.openReadingPipe(1,Virtual_wire);
  radio.startListening();
}

void loop()
{

  if ( radio.available() )
  {
      radio.read( coming, sizeof(coming) );
      valueA = coming[0];
   

   Serial.print(coming[0]);  Serial.print("   "); Serial.println(coming[1]);
  

      if (valueA==1) //
      
     {
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7,LOW);
       }

     else if (valueA==0) 
      
     {digitalWrite(6, LOW);
      digitalWrite(7,HIGH);
      digitalWrite(5,LOW);
        }
   
  }
  }
