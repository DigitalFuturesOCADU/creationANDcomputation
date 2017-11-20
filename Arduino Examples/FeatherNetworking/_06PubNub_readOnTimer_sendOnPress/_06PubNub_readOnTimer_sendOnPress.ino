/*
 * Creation & Computation - Digital Futures, OCAD University
 * Kate Hartman / Nick Puckett
 * 
 *read 2 random values from PubNub on a timer, send new values on button press
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


int ledPin = 10;                  //the pin the LED is connected to



int buttonPin = 11;               //the pin the button is connected to
int buttonPrev = 1;               //previous value of the button
int buttonVal;                     //current value of the button


unsigned long lastRefresh = 0;
int publishRate = 2000;              //refresh rate for reading values

int sensorPin1 = A0;                //the pin the potentiometer is attached  to


int yourVal1;                       //variables holding the values coming from the server
int yourVal2;

int myVal1;                         //variable holding the vaues going to the server
int myVal2;


void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);  //set the pinmode for the button
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
  connectToServer();


}

void loop() 
{
  
analogWrite(ledPin,yourVal2);                   //adjust the value of the led using the value from the server

buttonVal = digitalRead(buttonPin);             //read the button  


myVal1 = analogRead(sensorPin1);                //read the potentiometer
myVal2 = random(100,200);                       //store a random value



   
if((buttonVal==1)&&(buttonPrev==0))  //trigger the feed update with a button, uses both current and prev value to only change on the switch
{  
publishToPubNub();                    //send your values to PubNub
}



  if(millis()-lastRefresh>=publishRate)     //theTimer to trigger the reads 
  {
  readFromPubNub();                   //read values from PubNub
 
  lastRefresh=millis();   
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
      yourVal1 = sMessage["randoVal1"];  //store the value from the JSON into the variable
      Serial.print("randoVal1 ");        //you need to know the name of the parameter in the message for this to work
      Serial.println(yourVal1);
      yourVal2 = sMessage["randoVal2"];  //this is the one that connects to the LED
      Serial.print("randoVal2 ");
      Serial.println(yourVal2);
      
    }
    
    
  }
  
  sClient->stop();

}




