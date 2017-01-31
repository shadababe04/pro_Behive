/*
 * HumidTemp_20jan.ino
 *
 * Created: 1/20/2017 4:09:48 PM
 * Author: shadab
 */ 

 #include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"


#include "dht.h"

RF24 radio(7,8);

const uint64_t pipes[2] = { 0xF0F0F0F0EELL, 0xF0F0F0F0DDLL };

const int max_payload_size = 32;

char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char
char nodeAddr[10],nodeStatus[11];


dht DHT;

int dustPin=A0;
int dustVal=0;
 
int ledPower=2;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
int temp,hum;
char txbuf[19],rxbuf[19];

//#define DHT11_PIN 4
#define DHT21_PIN 5
//#define DHT22_PIN 6

void setup()
{
  Serial.begin(57600);
  pinMode(ledPower,OUTPUT);
//  Serial.println("DHT TEST PROGRAM ");
  //Serial.print("LIBRARY VERSION: ");
  //Serial.println(DHT_LIB_VERSION);
//  Serial.println();
  //Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

  printf_begin();
  
  radio.begin();

  // enable dynamic payloads
  radio.enableDynamicPayloads();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setDataRate(RF24_1MBPS);

    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);

  radio.startListening();

  radio.printDetails();
}


int aqi()
{
// ledPower is any digital pin on the arduino connected to Pin 3 on the sensor
digitalWrite(ledPower,LOW);// power on the LED
delayMicroseconds(delayTime);
dustVal=analogRead(dustPin);// read the dust value via pin 5 on the sensor
delayMicroseconds(delayTime2);
digitalWrite(ledPower,HIGH);// turn the LED off
delayMicroseconds(offTime);

 return dustVal;
//delay(2000);
//Serial.println(dustVal);
  
}
void dht()
{
//Serial.print("DHT21, \t");
  int chk = DHT.read21(DHT21_PIN);
//  switch (chk)
//  {
//    case DHTLIB_OK:
//    Serial.print("OK,\t");
//    break;
//    case DHTLIB_ERROR_CHECKSUM:
//    Serial.print("Checksum error,\t");
//    break;
//    case DHTLIB_ERROR_TIMEOUT:
//    Serial.print("Time out error,\t");
//    break;
//    default:
//    Serial.print("Unknown error,\t");
//    break;
//  }
  // DISPLAY DATA
//  Serial.print(DHT.humidity, 1);
//  Serial.print(",\t");
//  Serial.println(DHT.temperature, 1);

  
}

void loop()
{
//  int j;
//  if ( radio.available() )
//    {
//      // Dump the payloads until we've gotten everything
//      uint8_t len;
//      bool done = false;
//      while (!done)
//      {
//        // Fetch the payload, and see if this was the last one.
//        len = radio.getDynamicPayloadSize();
//        radio.read( receive_payload, len );
//          if (len >0)
//            done = 1;
//
//       // Put a zero at the end for easy printing
//       receive_payload[len] = 0;
//
//      // Spew it
//      printf("Got payload size=%i value=%s\n\r",len,receive_payload);
//      }
//
//      // First, stop listening so we can talk
//      radio.stopListening();
//      //delay(1000);
//      
//      for (j = 0; j < 9; j++)
//         nodeAddr[j] = receive_payload[j]; //node_address_s.node_status[node_index];
//      nodeAddr[9] = '\0';
//      printf("nodeAddr ::%s\n\r",nodeAddr);
//      if ((strncmp(nodeAddr, "BBA619550",9) == 0)) {
//          for (j = 0; j < 11; j++)
//            nodeStatus[j] = receive_payload[j + 9]; //node_address_s.node_status[node_index];
//          nodeStatus[j] = '\0';
//          if ((strncmp(nodeStatus, "STATUS_REQ",10) == 0)) {
            radio.stopListening();
            delay(1000);
            int aqval = aqi();
            Serial.println(aqval);
            delay(1000);
             DHT.read21(DHT21_PIN);
             temp = DHT.temperature;
             hum = DHT.humidity;
             sprintf(txbuf,"BBA619550%04d%02d%04d",aqval,temp,hum);
             radio.write( txbuf, 19 );
             Serial.println("Sent response..");
             delay(180000);
//          }
//      }
//    }

      
  
//  int aqval = aqi();
//  DHT.read21(DHT21_PIN);
//  temp = DHT.temperature;
//  hum = DHT.humidity;
//  sprintf(txbuf,"BBA619550%04d%02d%04d",aqval,temp,hum);
//  Serial.println(txbuf);
//  delay(4000);
  
/*
  // READ DATA
  Serial.print("DHT11, \t");
  chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
    Serial.print("OK,\t");
    break;
    case DHTLIB_ERROR_CHECKSUM:
    Serial.print("Checksum error,\t");
    break;
    case DHTLIB_ERROR_TIMEOUT:
    Serial.print("Time out error,\t");
    break;
    default:
    Serial.print("Unknown error,\t");
    break;
  }
  // DISPLAY DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);

  delay(1000);
*/
}
//
// END OF FILE
//

