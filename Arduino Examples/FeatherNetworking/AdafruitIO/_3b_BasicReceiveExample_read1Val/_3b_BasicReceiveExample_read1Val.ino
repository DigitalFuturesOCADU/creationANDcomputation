#include "AdafruitIO_WiFi.h"



//get this data from your adafruit.io account
#define IO_USERNAME "npuckett"
#define  IO_KEY "657dc16674f347e5ad2f4c805abb0f60"


//for ocad wifi make sure you have registered the MAC address of your board
//if you are testing this somewhere else, you'll need to update these settings
#define WIFI_SSID "UPSTAIRSWEB"
#define WIFI_PASS "interweb"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *val1 = io.feed("sensor1");
int refreshRate = 2000;
unsigned long lastRefresh = 0;
int errorCounter = 0;

long remoteVal1;




void setup() 
{


Serial.begin(9600);


connectToAIO();
}

void loop() 
{
updateAIOfeeds();



}

void connectToAIO()
{
io.connect();  //connect to the server using your login credentials
Serial.println("Connecting to Server");


val1->onMessage(readIncoming);  ///connect the feed to a callback function to handle incoming data


//update the status in the console as it attempts to make a connection
  while(io.status() < AIO_CONNECTED) 
  {
    Serial.print("-notYet-");
    delay(500);
    errorCounter++;

      if(errorCounter>=10)
      {
        Serial.println("**Something isn't working**");
      }
    
  }
Serial.println("***Success***");  //Successfully connected
Serial.println(io.statusText());
  
}




void updateAIOfeeds()
{
  if(millis()-lastRefresh>=refreshRate)
  {

  io.run(); //update everything on AIO
  
  lastRefresh=millis();
  }
}


void readIncoming(AdafruitIO_Data *inData)
{
  
  remoteVal1=atol(inData->value());
  Serial.println(remoteVal1);

}


