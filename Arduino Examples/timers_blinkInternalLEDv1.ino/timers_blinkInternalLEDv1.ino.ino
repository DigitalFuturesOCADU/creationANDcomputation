/*
 * Creation & Computation Examples
 * Nick Puckett
 * 
 * Class1  - blink the internal LED with a timer
 *  this example uses the internal LED , attached to pin 13
 * to use it with a differen LED, just change the pin number
 * v1 Extended code
 */


int ledPin1 = 13;
int blinkRate = 100;
boolean ledState1 = false;


long lastTimeYouBlinked;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); //turn on the Serial port so we can print values
}

void loop() {
  // put your main code here, to run repeatedly:

if(millis()-lastTimeYouBlinked>=blinkRate) //this very simple statement is the timer,
{                                          //it subtracts the value of the moment in time the last blink happened, and sees if that number is larger than your set blinking value
  if(ledState1==false)                     //This statement is used to see if it should be turning the LED on or off by looking at its current state            
    {
     digitalWrite(ledPin1,HIGH);          //This statement addresses the LED itself and turns it on. because it was previously off
     ledState1=true;                      //It then flips then changes its current value to true
    }
  else                                    //Since there are only 2 options, we can use an else
  {
    digitalWrite(ledPin1, LOW);           //this event happens when the LED is on so we write the pin to be LOW , which means off
    ledState1=false;                      //save that value
  }

lastTimeYouBlinked = millis();            //save the value in time that this switch occured, so we can use it again.
   
}






}
