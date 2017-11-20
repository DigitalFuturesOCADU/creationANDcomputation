/*
 * Creation & Computation - Digital Futures, OCAD University
 * Kate Hartman / Nick Puckett
 * 
 *read 2 random values from PubNub when the button is pressed
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


int ledPin = 10;        //the pin the led is connect to 



int buttonPin = 11;         //the pin the button is connected to

int buttonPrev = 1;         //variable to hold the previous button state
int buttonVal;              //variable to hold the current button state




int yourVal1;               //these variables hold the value coming FROM pubnub
int yourVal2;

int myVal1;                 //the variable hodl teh values to send TO pubnub
int myVal2;


void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
  connectToServer();


}

void loop() 
{
  
analogWrite(ledPin,yourVal2);

buttonVal = digitalRead(buttonPin);     //read the value from the button pin
   
if((buttonVal==1)&&(buttonPrev==0))  //trigger the feed update with a button, uses both current and prev value to only change on the switch
{  

readFromPubNub();                   //trigger the read function
}



buttonPrev = buttonVal; //store the value of this cycle to compare next loop

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



