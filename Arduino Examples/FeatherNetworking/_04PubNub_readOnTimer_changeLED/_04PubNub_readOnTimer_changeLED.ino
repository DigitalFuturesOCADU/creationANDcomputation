/*
 * Creation & Computation - Digital Futures, OCAD University
 * Kate Hartman / Nick Puckett
 * 
 *read 2 random values to PubNub at a given interval
 *  
 * 
 * 
 */
#include <ArduinoJson.h> 
#include <SPI.h>

#include <WiFi101.h>
#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>

static char ssid[] = "ocadu-embedded";      //SSID of the wireless network
static char pass[] = "internetofthings";    //password of that network
int status = WL_IDLE_STATUS;                // the Wifi radio's status

const static char pubkey[] = "";  //get this from your PUbNub account
const static char subkey[] = "";  //get this from your PubNub account

const static char pubChannel[] = "channel2"; //choose a name for the channel to publish messages to
const static char subChannel[] = "channel1"; //choose a name for the channel to publish messages to


int ledPin = 10;                        //pin the led is connecto to. must be a PWM pin ~

unsigned long lastRefresh = 0;
int readRate = 2000;                    //read rate for reading from the server




int yourVal1;
int yourVal2;



void setup() 
{
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
  connectToServer();


}

void loop() 
{
  
analogWrite(ledPin,yourVal2);                 //output the value to the led (must be between 0-255)


   

  if(millis()-lastRefresh>=readRate)              //timer for controlling the read
  {
  readFromPubNub();                               //run th read function
    
  lastRefresh=millis();                           //save the value 
  }



}


void connectToServer()
{
  WiFi.setPins(8,7,4,2); //This is specific to the feather M0
 
  status = WiFi.begin(ssid, pass);                    //attempt to connect to the network
  Serial.println("***Connecting to WiFi Network***");


 for(int trys = 1; trys<=10; trys++)                    //use a loop to attempt the connection more than once
 { 
    if ( status == WL_CONNECTED)                        //check to see if the connection was successful
    {
      Serial.print("Connected to ");
      Serial.println(ssid);
  
      PubNub.begin(pubkey, subkey);                      //connect to the PubNub Servers
      Serial.println("PubNub Connected"); 
      break;                                             //exit the connection loop     
    } 
    else 
    {
      Serial.print("Could Not Connect - Attempt:");
      Serial.println(trys);

    }

    if(trys==10)
    {
      Serial.println("I don't this this is going to work");
    }
    delay(1000);
 }

  
}




void readFromPubNub()
{
  StaticJsonBuffer<1200> inBuffer;                    //create a memory buffer to hold a JSON Object
 PubSubClient *sClient = PubNub.subscribe(subChannel);

  if (!sClient) {
    Serial.println("message read error");
    delay(1000);
    return;
  }

  while (sClient->connected()) 
  {
    while (sClient->connected() && !sClient->available()) ; // wait
    char c = sClient->read();
    JsonObject& sMessage = inBuffer.parse(*sClient);
    
    if(sMessage.success())
    {
      //sMessage.prettyPrintTo(Serial); //uncomment to see the JSON message in the serial monitor

      yourVal2 = sMessage["randoVal2"];  //save the value fromt the server into the variable , the variable then controls the LED
      Serial.print("randoVal2 ");
      Serial.println(yourVal2);
      
    }
    
    
  }
  
  sClient->stop();

}




