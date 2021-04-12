#include "RCSwitch.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <DeviceSwitch.h>
#include <DeviceSwitchFactory.h>
#include <RFSenderRCSwitch.h>
#include <Log.h>
#include <SerialLogger.h>

#define PIN_D3 0
#define PIN_D2 4
#define PIN_D4_LED 2
RFSenderRCSwitch mySwitch = RFSenderRCSwitch(RCSwitch());
DeviceSwitchFactory g_deviceSwitchFactory(&mySwitch);
// Update these with values suitable for your network.

const char *ssid = "Drop it like it's Hotspot";
const char *pswd = "fortunabier";
const char *mqtt_server = "rhasspy.local";
const String SUBSCRIBE_TOPIC_DIP = "rfdevices/station1/dip/+/switch";
const String TOPIC_ROOT_DIP = "rfdevices/station1/dip/";

const String SUBSCRIBE_TOPIC_CODEID = "rfdevices/station1/dip/+/switch";
const String TOPIC_ROOT_CODEID = "rfdevices/station1/codeid/";

const String TOPIC_SWITCH = "/switch";
const String TOPIC_STATE = "/status";

WiFiClient espClient;
PubSubClient client(espClient);

int status = WL_IDLE_STATUS; // the starting Wifi radio's status

void setup_wifi()
{
  delay(10);
  Log::infof("Connecting to %s", ssid);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Log::info(".");
  }
  Log::info("WiFi connected");
  Log::infof("IP address: %s", WiFi.localIP().toString().c_str());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Log::infof("Message arrived [%s]", topic);
  char strPayload[length + 1];
  memcpy(strPayload, payload, length);
  strPayload[length] = 0; // Null termination.
  Log::infof("Payload: %s\n", strPayload);

  IDeviceSwitch *deviceSwitch = g_deviceSwitchFactory.create(topic);

  if (deviceSwitch == NULL)
  {
    Log::error("Unexpected Switch");
    return;
  }

  String pubTopic(topic);
  pubTopic.replace(TOPIC_SWITCH, TOPIC_STATE);
  if (strcmp(strPayload, "ON") == 0)
  {
    Log::info("Received ON_REQUEST");
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    deviceSwitch->turnOn();
    delay(100);
    Log::infof("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "ON", true);
  }
  if (strcmp(strPayload, "OFF") == 0)
  {
    Log::info("Received OFF_REQUEST");
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (Note that LOW is the voltage level
    deviceSwitch->turnOff();
    delay(100);
    Log::infof("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "OFF", true);
  }
}

String macToStr(const uint8_t *mac)
{
  String result;
  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

String composeClientID()
{
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String clientId;
  clientId += "esp-";
  clientId += macToStr(mac);
  return clientId;
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Log::info("Attempting MQTT connection...");

    String clientId = composeClientID();
    clientId += "-";
    clientId += String(micros() & 0xff, 16); // to randomise. sort of

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Log::info("connected");
      // Once connected, publish an announcement...
      //client.publish(ROOT_TOPIC.c_str(), ("connected " + composeClientID()).c_str() , true );
      // ... and resubscribe
      // topic + clientID + in
      String subscribeTopic = TOPIC_ROOT_DIP + "+" + TOPIC_SWITCH;
      client.subscribe(subscribeTopic.c_str());
      Log::infof("subscribed to: ", subscribeTopic);

      String subscribeTopicCodeId = TOPIC_ROOT_CODEID + "+" + TOPIC_SWITCH;
      client.subscribe(subscribeTopicCodeId.c_str());
      Log::infof("subscribed to: ", subscribeTopicCodeId);
    }
    else
    {
      Log::errorf("failed, rc=%d", client.state());
      Log::errorf(" wifi=%d", WiFi.status());
      Log::error(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Log::init(new SerialLogger());
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  // Transmitter is connected to Arduino Pin #10
  mySwitch.enableTransmit(PIN_D3);
  //mySwitch.enableReceive(PIN_D2); // Receiver input on interrupt 0 (D2)

  // Optional set pulse length.
  //mySwitch.setPulseLength(pulseLength);

  // Optional set protocol (default is 1, will work for most outlets)
  //mySwitch.setProtocol(protocol);

  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  // confirm still connected to mqtt server
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  // TODO: implement receiver
  /*
  if (mySwitch.available())
  {
    Serial.print("Received ");
    //Serial.print(mySwitch.getReceivedValue());
    Serial.print(" / ");
    //Serial.print(mySwitch.getReceivedBitlength());
    Serial.print("Bit ");
    Serial.print(" // ");
    Serial.print("Protocol: ");
    //Serial.println(mySwitch.getReceivedProtocol());
    Serial.printf("Delay: %d\n", mySwitch.getReceivedDelay());
    mySwitch.resetAvailable();
  }
  */
}
