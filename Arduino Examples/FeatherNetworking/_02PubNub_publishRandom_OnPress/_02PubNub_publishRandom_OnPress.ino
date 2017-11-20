/*
 * Creation & Computation - Digital Futures, OCAD University
 * Kate Hartman / Nick Puckett
 * 
 * publish 1 random value and 1 potentiometer value when a button is pressed
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

int sensorPin1 = A0;              //the pin the potentiometer is connected to                

int buttonPin = 11;               //the pin the button is attached to
int buttonPrev;                   //holds the previous value of the button , used to trigger only once
int buttonVal;                    //holds the current state of the button


int myVal1;                       //variables to hold values to send
int myVal2;




void setup() 
{

  pinMode(buttonPin, INPUT_PULLUP);             //set the pin mode for the button
  Serial.begin(9600);
  connectToServer();                            //execute the connection funtion.  //connect to wifi and pubnub


}

void loop() 
{
  buttonVal = digitalRead(buttonPin);             //read the button value and save it



  
  myVal1 = analogRead(sensorPin1);                //read the value from the potentiometer
  myVal2 = random(100,200);                       //save a random value to send as well




if((buttonVal==1)&&(buttonPrev==0))  //trigger the feed update with a button, uses both current and prev value to only change on the switch
{  
  Serial.println(myVal1);
publishToPubNub();                   //trigger the publish function to send the values to pubnub
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
  
  while (client->available()) 
  {
    Serial.write(client->read());
  }
  client->stop();
  Serial.println("Successful Publish");


  
}






