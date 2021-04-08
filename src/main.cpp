#include "RCSwitch.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <DeviceSwitch.h>
#include <DeviceSwitchFactory.h>
#include <RFSenderRCSwitch.h>

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

long timeBetweenMessages = 1000 * 20 * 1;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;

int status = WL_IDLE_STATUS; // the starting Wifi radio's status

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleDipSwitch(char *topic, char *strPayload)
{
  char code1[6] = "";
  char code2[6] = "";

  if (!DeviceSwitchFactory::extractDipCodeFromTopic(topic, code1, code2))
  {
    Serial.printf("Failed to decode codes");
    return;
  }

  Serial.printf("Code1: %s\n", code1);
  Serial.printf("Code2: %s\n", code2);

  String pubTopic = TOPIC_ROOT_DIP + String(code1) + ":" + String(code2) + TOPIC_STATE;
  if (strcmp(strPayload, "ON") == 0)
  {
    Serial.println("Received ON_REQUEST");
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    mySwitch.switchOn(code1, code2);
    delay(100);
    Serial.printf("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "ON", true);
  }
  if (strcmp(strPayload, "OFF") == 0)
  {
    Serial.println("Received OFF_REQUEST");
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (Note that LOW is the voltage level
    mySwitch.switchOff(code1, code2);
    delay(100);
    Serial.printf("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "OFF", true);
  }
}

void handleCodeIdSwitch(char *topic, char *strPayload)
{
  long codeid = 0;
  if (!DeviceSwitchFactory::extractCodeIdFromTopic(topic, &codeid))
  {
    Serial.printf("Failed to decode codes");
    return;
  }

  Serial.printf("CodeID: %ld\n", codeid);

  String pubTopic = TOPIC_ROOT_CODEID + String(codeid) + TOPIC_STATE;
  if (strcmp(strPayload, "ON") == 0)
  {
    Serial.println("Received ON_REQUEST");
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    //mySwitch.switchOn(code1, code2);
    //mySwitch.send(10400573, 24);
    //mySwitch.send(codeid, 24);
    delay(100);
    Serial.printf("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "ON", true);
  }
  if (strcmp(strPayload, "OFF") == 0)
  {
    Serial.println("Received OFF_REQUEST");
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (Note that LOW is the voltage level
    //mySwitch.switchOff(code1, code2);
    //mySwitch.send(10400565, 24);
    //mySwitch.send(codeid - 8, 24);
    delay(100);
    Serial.printf("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "OFF", true);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char strPayload[length + 1];
  memcpy(strPayload, payload, length);
  strPayload[length] = 0; // Null termination.
  Serial.printf("Payload: %s\n", strPayload);
  IDeviceSwitch *deviceSwitch = g_deviceSwitchFactory.create(topic);

  if (deviceSwitch == NULL)
  {
    Serial.printf("Unexpected Switch");
    return;
  }

  String pubTopic(topic);
  pubTopic.replace(TOPIC_SWITCH, TOPIC_STATE);
  if (strcmp(strPayload, "ON") == 0)
  {
    Serial.println("Received ON_REQUEST");
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    //mySwitch.switchOn(code1, code2);
    //mySwitch.send(10400573, 24);
    //mySwitch.send(codeid, 24);
    deviceSwitch->turnOn();
    delay(100);
    Serial.printf("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "ON", true);
  }
  if (strcmp(strPayload, "OFF") == 0)
  {
    Serial.println("Received OFF_REQUEST");
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (Note that LOW is the voltage level
    //mySwitch.switchOff(code1, code2);
    //mySwitch.send(10400565, 24);
    //mySwitch.send(codeid - 8, 24);
    deviceSwitch->turnOff();
    delay(100);
    Serial.printf("pub: %s\n", pubTopic.c_str());
    client.publish(pubTopic.c_str(), "OFF", true);
  }

  /*
  DeviceSwitchFactory::switch_type switchType = DeviceSwitchFactory::getSwitchType(topic);
  if (switchType == DeviceSwitchFactory::dip)
  {
    handleDipSwitch(topic, strPayload);
  }
  else if (switchType == DeviceSwitchFactory::codeid)
  {
    handleCodeIdSwitch(topic, strPayload);
  }
  else{
    Serial.printf("Unexpected Switch");
  }
  */

  /*
  if(strlen(strPayload) != 11){
    Serial.printf("Unpexcted rf code length (expected 10): %d\n", strlen(strPayload));
    return;
  }

  char code1[6];
  char code2[6];
  strncpy ( code1, strPayload, sizeof(code1) );
  strncpy ( code2, strPayload + sizeof(code1), sizeof(code2) );
  code1[5] = 0;
  code2[5] = 0;

  Serial.printf("Code1: %s\n", code1);
  Serial.printf("Code2: %s\n", code2);
  String strTopic = String(topic);
  if(strTopic.equals(TOPIC_ON)){
    Serial.print("Received ON_REQUEST");
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    mySwitch.switchOn(code1, code2);
  }
  if(strTopic.equals(TOPIC_OFF)){
    Serial.print("Received OFF_REQUEST");
    digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    mySwitch.switchOff(code1, code2);
  }
  */
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
    Serial.print("Attempting MQTT connection...");

    String clientId = composeClientID();
    clientId += "-";
    clientId += String(micros() & 0xff, 16); // to randomise. sort of

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(ROOT_TOPIC.c_str(), ("connected " + composeClientID()).c_str() , true );
      // ... and resubscribe
      // topic + clientID + in
      String subscribeTopic = TOPIC_ROOT_DIP + "+" + TOPIC_SWITCH;
      client.subscribe(subscribeTopic.c_str());
      Serial.print("subscribed to : ");
      Serial.println(subscribeTopic);

      String subscribeTopicCodeId = TOPIC_ROOT_CODEID + "+" + TOPIC_SWITCH;
      client.subscribe(subscribeTopicCodeId.c_str());
      Serial.print("subscribed to : ");
      Serial.println(subscribeTopicCodeId);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print(" wifi=");
      Serial.print(WiFi.status());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  // Transmitter is connected to Arduino Pin #10
  mySwitch.enableTransmit(PIN_D3);
  mySwitch.enableReceive(PIN_D2); // Receiver input on interrupt 0 (D2)

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
  /*
  long now = millis();
  if (now - lastMsg > timeBetweenMessages)
  {
    lastMsg = now;
    ++value;
    String payload = "{\"micros\":";
    payload += micros();
    payload += ",\"counter\":";
    payload += value;
    payload += ",\"client\":";
    payload += composeClientID();
    payload += "}";
    String pubTopic;
    pubTopic += "test";
    pubTopic += "/";
    pubTopic += composeClientID();
    pubTopic += "/out";
    Serial.print("Publish topic: ");
    Serial.println(pubTopic);
    Serial.print("Publish message: ");
    Serial.println(payload);
    client.publish((char *)pubTopic.c_str(), (char *)payload.c_str(), true);
  }
  */
}
