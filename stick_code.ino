#include "Wire.h" // This library allows you to communicate with I2C devices.
#include "TinyGPS++.h"
TinyGPSPlus gps;

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define CE_PIN 9 //rf data
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
int nrff[6];




int emer=24,sos;
int buzzer=11;
const int trigPin = 22;
const int echoPin = 23;
long duration;
int distance;
int times,dur;




const int trigPin2 = 4;
const int echoPin2 = 5;
long duration2;
int distance2;
int times2,dur2;



const int trigPin3 = 2;
const int echoPin3 = 3;
long duration3;
int distance3;
int times3,dur3;

int analogInput = A15;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; //30k
float R2 = 7500.0; //7500 ohm resistor, I tweaked this
int value = 0;

void setup() {
// put your setup code here, to run once;
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);

pinMode(trigPin3, OUTPUT);
pinMode(echoPin3, INPUT);
pinMode(emer, INPUT);
pinMode(buzzer, OUTPUT);
Serial3.begin(9600);
Serial2.begin(9600);
Serial.begin(9600);

radio.begin();
radio.openWritingPipe(pipe);
radio.stopListening();

times=millis();
}

void loop() {

dur=millis()-times;
if(dur>5000)
{
for(;;)
{
dur=millis()-times;
if (dur>=6000) break;
GPS();
Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
Serial.print(" LNG="); Serial.println(gps.location.lng(), 6);
}
}
if(dur>6500) times=millis();
//=======================================================================================================
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance=duration*0.034/2 ;


digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
distance2=duration2*0.034/2 ;


digitalWrite(trigPin3, LOW);
delayMicroseconds(2);
digitalWrite(trigPin3, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin3, LOW);
duration3 = pulseIn(echoPin3, HIGH);
distance3=duration3*0.034/2 ;


if((distance <100 && distance>50 && distance!=0)|| (distance2 <100 && distance2>50 && distance2!=0)|| (distance3 <100 && distance3>50 && distance3!=0))
{ Serial.println("somthing is near"); digitalWrite(buzzer,HIGH); delay(50);digitalWrite(buzzer,LOW); delay(50);}
if((distance <50 && distance!=0) || (distance2 <50 && distance2!=0) || (distance3 <50 && distance3!=0))
{nrff[0]=1;Serial.println("collesion may be happen");digitalWrite(buzzer,HIGH);}
else digitalWrite(buzzer,LOW);
//=======================================================================================================
sos=digitalRead(emer);
if(sos==1)
{call();
nrff[0]=1;
Serial.println("Sos is pressed");
for(int i=0;i<10;i++)
{digitalWrite(buzzer,HIGH); delay(100);digitalWrite(buzzer,LOW); delay(50);}

sms();
}
//=======================================================================================================
value = analogRead(analogInput);
vout = (value * 5.0) / 1024.0;
vin = vout / (R2/(R1+R2));
if(vin<10.0 && vin>2.0) {digitalWrite(buzzer,HIGH); delay(2000);digitalWrite(buzzer,LOW); }
//=======================================================================================================
if(sos==0 && distance>50) nrff[0]=0;
radio.write( nrff, sizeof(nrff) );

Serial.print("Battery V= ");
Serial.print(vin,2);
Serial.print(" , distance = ");
Serial.print(distance);
Serial.print(" , distance2 = ");
Serial.print(distance2);
Serial.print(" , distance3 = ");
Serial.print(distance3);
Serial.print(" nrf= ");
Serial.println(nrff[0]);
delay(100);

}


//-------------------------------------------

void GPS()//this function to locate the location
{
String altitude ;

while (Serial2.available() > 0)
gps.encode(Serial2.read());

if (gps.location.isUpdated())
{
Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
Serial.print(" LNG="); Serial.println(gps.location.lng(), 6);
}


}
//===================================================
void sms()
{
int t=0;
Serial3.println("AT+CMGF=1");
t=0;
while(t<100)
{
if(Serial3.available())
Serial.write(Serial3.read());
delay(10);
t++;
}
Serial3.println("AT+CMGS=\"0097450504440\"");
t=0;
while(t<100)
{
if(Serial3.available())
Serial.write(Serial3.read());
delay(10);
t++;
}
Serial3.println("Ineed help in location:");
Serial3.print("lat= ");
Serial3.println(gps.location.lat(), 6);
Serial3.print("long= ");
Serial3.println(gps.location.lng(), 6);
Serial3.print("https://maps.google.com/?q=");
Serial3.print(gps.location.lat(), 6); Serial3.print(',');Serial3.println(gps.location.lng(), 6);
t=0;
while(t<300)
{
if(Serial3.available())
Serial.write(Serial3.read());
delay(10);
t++;
}
Serial3.write(26); //end of message
Serial3.write(26); //end of message
Serial3.write(26); //end of message
Serial3.write(26); //end of message
Serial3.write(26); //end of message
Serial3.write(26); //end of message
t=0;
while(t<700)
{
if(Serial3.available())
Serial.write(Serial3.read());
delay(10);
t++;
}}
//===================================================
void call()
{
int t;
Serial3.println("ATD0097450504440;");
t=0;
while(t<500)
{
if(Serial3.available())
Serial.write(Serial3.read());
delay(10);
t++;
}
}
//===================================================
