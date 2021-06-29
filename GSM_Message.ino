/* Arduino Code for Sending a Message to a particular Mobile Phone using GSM-Module
 *  Developed By Naga Sai Ganesh */

#include <String.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);

String msg;

void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);// Setting the baud rate of Serial Monitor

  delay(100);
}


void loop()
{
	msg=(String)"The Water is Over-Flow from Maximum-Level:"+temp;
}


//  Module for Sending SMS to Mobile

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+91XXXXXXXXXX\"\r"); // Replace X with mobile number towhich you want SMS
  delay(1000);
  mySerial.println(msg);// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
