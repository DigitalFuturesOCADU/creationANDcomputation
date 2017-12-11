import java.util.Arrays;
import java.util.List;



String publishChannel = "testingChannel";
String pubKey = "getYourkey";
String subKey = "getYourkey";

PNConfiguration pnConfiguration;
PubNub pubnub;


float incomingRand;
String incomingPerson;

void setup()
{
  

    pnConfiguration = new PNConfiguration();
    pnConfiguration.setSubscribeKey(subKey);
    pnConfiguration.setPublishKey(pubKey);
    pnConfiguration.setSecure(true);
    
    
    pubnub = new PubNub(pnConfiguration);
    pubnub.subscribe()
    .channels(Arrays.asList(publishChannel)) // subscribe to channels information
    .execute();
    
    pubnub.addListener(subscribeCallback);
}


void draw()
{
  
}

void mousePressed()
{
float randoSend1 = random(0,100);        //picking some random numbers to add to the message
float randoSend2 = random(100,200);
  
  
 JsonObject pnMessage = new JsonObject();    //create a JSON object the hold the message
 
 pnMessage.addProperty("sender", "user1");        //add keys / values to the message
 pnMessage.addProperty("randoVal1", randoSend1);
 pnMessage.addProperty("randoVal2", randoSend2);
 
 pubnub.publish()                                  //publish the message
    .message(pnMessage)
    .channel(publishChannel)
    .async(new PNCallback<PNPublishResult>() {
        @Override
        public void onResponse(PNPublishResult result, PNStatus status) {
            // handle publish result, status always present, result if successful
            // status.isError() to see if error happened
            if(!status.isError()) {
                System.out.println("pub timetoken: " + result.getTimetoken());
            }
            System.out.println("pub status code: " + status.getStatusCode());
        }
    });
 
 
 
}

SubscribeCallback subscribeCallback = new SubscribeCallback() {
    @Override
    public void status(PubNub pubnub, PNStatus status) {
 
    }
 
    @Override
    public void message(PubNub pubnub, PNMessageResult message) ////this is the function that actually handles incoming messages
    {
 
    println(message.getMessage());

    JSONObject inMessage = parseJSONObject(message.getMessage().toString());
    
    incomingRand = inMessage.getFloat("randoVal1");
    incomingPerson = inMessage.getString("sender");
    
    }
 
    @Override
    public void presence(PubNub pubnub, PNPresenceEventResult presence) {
 
    }
};