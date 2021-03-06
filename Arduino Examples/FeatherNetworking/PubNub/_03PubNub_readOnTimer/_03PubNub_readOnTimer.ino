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

const static char pubkey[] = "pub-c-bb1c713d-dc79-438c-9013-d51df38097c2";  //get this from your PUbNub account
const static char subkey[] = "sub-c-b8b4a804-c406-11e7-adbc-0adecfecf8a2";  //get this from your PubNub account


const static char pubChannel[] = "channel2"; //choose a name for the channel to publish messages to
const static char subChannel[] = "channel1"; //choose a name for the channel to publish messages to


unsigned long lastRefresh = 0;
int readRate = 2000;                //controls how often it reads from pubnub



int yourVal1;                       //these variables hold the values read from PubNub
int yourVal2;


void setup() 
{
  Serial.begin(9600);
  connectToServer();                        //connect to the wifi and pubnub server


}

void loop() 
{
  



   

  if(millis()-lastRefresh>=readRate)
  {
  readFromPubNub();                             //trigger the read from pubnub function
 
  lastRefresh=millis();   
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

  if (!sClient) {                                     //error check to the connection
    Serial.println("message read error");
    delay(1000);
    return;
  }

  while (sClient->connected())                                //if it is connected
  {
    while (sClient->connected() && !sClient->available()) ; // wait for data
    char c = sClient->read();
    JsonObject& sMessage = inBuffer.parse(*sClient);        //make the json object to hold the message
    
    if(sMessage.success())
    {
      //sMessage.prettyPrintTo(Serial); //uncomment to see the JSON message in the serial monitor
      
      yourVal1 = sMessage["randoVal1"];         //read the value from the json object into 
      Serial.print("randoVal1 ");               //for this to work you need to know the parameter name being sent
      Serial.println(yourVal1);
      yourVal2 = sMessage["randoVal2"];
      Serial.print("randoVal2 ");
      Serial.println(yourVal2);
      
    }
    
    
  }
  
  sClient->stop();                                //stop the connection

}




