/*
 * Creation & Computation - Digital Futures, OCAD University
 * Kate Hartman / Nick Puckett
 * 
 * publish 2 random values to PubNub at a given interval
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

const static char pubChannel[] = "channel1"; //choose a name for the channel to publish messages to


unsigned long lastRefresh = 0;    //used to make the timer work
int publishRate = 2000;           //how often to update the value on pubnub in milliseconds






int myVal1;                       //variables to hold values to send
int myVal2;




void setup() 
{
  Serial.begin(9600);             //turn on the serial port
  connectToServer();              //execute the connection funtion.  //connect to wifi and PubNub


}

void loop() 
{
  myVal1 = random(0,10);          //this example just sends random numbers.  pick the two numbers
  myVal2 = random(100,200);
 



   

  if(millis()-lastRefresh>=publishRate)   //timer used to publish the values at a given rate
  {
  publishToPubNub();                      //execute the function that sends the values to pubnub
  lastRefresh=millis();                   //save the value so that the timer works
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

    if(trys==10)                                              //what to print if it doesn't work after 10 tries
    {
      Serial.println("I don't this this is going to work");
    }
    delay(1000);
 }

  
}


void publishToPubNub()
{
  WiFiClient *client;
  StaticJsonBuffer<800> messageBuffer;                    //create a memory buffer to hold a JSON Object
  JsonObject& pMessage = messageBuffer.createObject();    //create a new JSON object in that buffer
  
 ///the imporant bit where you feed in values
  pMessage["randoVal1"] = myVal1;                      //add a new property and give it a value
  pMessage["randoVal2"] = myVal2;                     //add a new property and give it a value


///                                                       //you can add/remove parameter as you like
  
  //pMessage.prettyPrintTo(Serial);   //uncomment this to see the messages in the serial monitor
  
  
  int mSize = pMessage.measureLength()+1;                     //determine the size of the JSON Message
  char msg[mSize];                                            //create a char array to hold the message 
  pMessage.printTo(msg,mSize);                               //convert the JSON object into simple text (needed for the PN Arduino client)
  
  client = PubNub.publish(pubChannel, msg);                      //publish the message to PubNub

  if (!client)                                                //error check the connection
  {
    Serial.println("client error");
    delay(1000);
    return;
  }
  
  if (PubNub.get_last_http_status_code_class() != PubNub::http_scc_success)  //check that it worked
  {
    Serial.print("Got HTTP status code error from PubNub, class: ");
    Serial.print(PubNub.get_last_http_status_code_class(), DEC);
  }
  
  while (client->available())                                 //get feedback from PubNub
  {
    Serial.write(client->read());
  }
  client->stop();                                             //stop the connection
  Serial.println("Successful Publish");


  
}






