/* Arduino Code for Water-Level Monitoring through ThingSpeak API and also Indiacted through SMS to Provided Mobile
 *  Developed By Naga Sai Ganesh, Team VYAKTA
 *  Start-Up and Innovation Cell, Acharya Nagarjuna University  */


#include <String.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11);


// Data Related to Ultrasonic
 int val,duration,distance;
#define echo 6
#define trigger 7 


// Data Related to Message
int temp=0;
String msg;


void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);// Setting the baud rate of Serial Monitor


  // Pin Modes
  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);


  delay(100);
}




void loop()
{
  //Triggering Ultrasonic
  digitalWrite(trigger,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigger,LOW);


  //Echo the Sound from Ultrasonic
  duration = pulseIn(echo,HIGH);


  //convert Analog Value into Distance
  distance = (duration/2)/29.1;


 //Triggering the Message 
 if(temp != distance)
 {
    if(distance > 90 || distance < 40)
    {
    
      delay(1000);


      temp = distance;  // Update Current Value
  
      if(distance > 90)
      {
        msg=(String)"The Water is Draining from Critical-Level:"+temp;
      }
      if(distance < 40)
      {
        msg=(String)"The Water is Over-Flow from Maximum-Level:"+temp;
      }
  
      Serial.println(msg); //Print on Serial Monitor
    
      SendMessage();  // Sending SMS to Mobile
    
      Send2Api(); // Sending Distance to ThingSpeakAPI
    
    }
 }
  
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


//  Module for Sending Distance to ThingSpeak-API


void Send2Api()
{
  mySerial.println("AT");
  delay(1000);


  mySerial.println("AT+CPIN?");
  delay(1000);


  mySerial.println("AT+CREG?");
  delay(1000);


  mySerial.println("AT+CGATT?");
  delay(1000);


  mySerial.println("AT+CIPSHUT");
  delay(1000);


  mySerial.println("AT+CIPSTATUS");
  delay(2000);


  mySerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  mySerial.println("AT+CSTT=\"airtelgprs.com\"");//Setting APN airtelgprs.com ->> AIRTEL  imis/internet ->> IDEA 
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIICR");//Bringing up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIFSR");//Getting IP address
  delay(2000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
 
   ShowSerialData();
  
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//Connecting with ThingSpeak
  delay(6000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSEND");//Sending Data
  delay(4000);
  ShowSerialData();
  
    String str="GET http://api.thingspeak.com/update?api_key=XXXXXXXXXXXXX&field1=" + String(msg); //Replace X with ThingSpeak-API Key towhich you want Update
  mySerial.println(str);//Sent Data to ThingSpeak-API
  delay(4000);
  ShowSerialData();


  mySerial.println((char)26);//sending
  delay(5000);f     
  mySerial.println();
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 


//  To show Interaction of API in Serial-Monitor


void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}
