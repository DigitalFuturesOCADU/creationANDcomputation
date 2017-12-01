#include "AdafruitIO_WiFi.h"



//get this data from your adafruit.io account
#define IO_USERNAME "npuckett"
#define  IO_KEY "657dc16674f347e5ad2f4c805abb0f60"


//for ocad wifi make sure you have registered the MAC address of your board
//if you are testing this somewhere else, you'll need to update these settings
#define WIFI_SSID "ocadu-embedded"
#define WIFI_PASS "internetofthings"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *val1 = io.feed("sensor1");


int refreshRate = 2000;
unsigned long lastRefresh = 0;
int errorCounter = 0;


int sensorPin1 = A0;
int sensorVal1;



void setup() 
{

Serial.begin(9600);
while(! Serial);

connectToAIO();
}

void loop() 
{

sensorVal1 = analogRead(sensorPin1);
  
updateAIOfeeds();



}

void connectToAIO()
{
io.connect();  //connect to the server using your login credentials

Serial.println("Connecting to Server");

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
  val1->save(sensorVal1);
  //val2->save(rando2);
  Serial.print("sent ");
  Serial.print(sensorVal1);
  Serial.print(" @");
  Serial.println(millis());

  io.run(); //update everything on AIO
  
  lastRefresh=millis();
  }




}


