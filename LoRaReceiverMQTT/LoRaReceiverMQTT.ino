/*****************************************************************************************************************************************************************************************
           < LoRa Reciver MQTT >
  < WASTE MESUREMENT Data Collector >
******************************************************************************************************************************************************************************************/
#include <PubSubClient.h>
#include <WiFi.h>
#include <LoRa.h>
#include <SPI.h>
// ESP 32 LoRa Pin-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ss   5
#define rst 14
#define dio0 2
// WiFi Set-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* ssid ="Nhahee family 2.4G";// your ssid
const char* password = "69696969"; // your password
// MQTT Set-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* mqtt_server = "192.168.1.19";   // your mqtt server IP 
const char* mqtt_username = "";             // your mqtt username
const char* mqtt_password = "";             // your mqtt password
const uint16_t mqtt_port = 1883;
WiFiClient tcpClient;                       // create tcp client object
PubSubClient mqttClient(tcpClient);         // pass tcp client to mqtt client
// My Node Byte----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte Node1 = 0xBB;
byte Node2 = 0xCC;
byte Node3 = 0xDD;
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String outgoing;              // outgoing message
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte msgCount = 0;            // count of outgoing messages
String incoming = "";
uint32_t lastTimestamp = millis();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Tracks the time since last event fired
unsigned long previousMillis   = 0;
unsigned long int previoussecs = 0;
unsigned long int currentsecs  = 0;
unsigned long currentMillis    = 0;
int interval = 1 ; // updated every 1 second
int Secs = 0;
//------------------------------------------------------------------------------
void setup_wifi(const char* ssid, const char* password){
  Serial.println("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\r\nWiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}
//------------------------------------------------------------------------------
void reconnect(){
  while(!mqttClient.connected()){
    Serial.println("Attemping MQTT Connection !");
    String clientID = "ESP32Client-" + String(WiFi.macAddress());
    Serial.print("ClientID : ");
    Serial.println(clientID);
    if (mqttClient.connect(clientID.c_str(),mqtt_username,mqtt_password)){
      Serial.println("MQTT Broker Connected");
      String msg = "Hello from " + clientID;
      mqttClient.publish("ESP32/heartbeat",msg.c_str());
    }
    else{
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println("try again in 1 seconds");
      delay(1000);
    }
  }
}
//------------------------------------------------------------------------------
void callBack(char* topic, byte* payload, uint16_t payload_length){
  String msg;
  String topic_str(topic);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] :");
  for (uint16_t i = 0; i < payload_length; i++ ){
    char c = (char)payload[i];
    msg += c;
  }
  Serial.println(msg);
}
//------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);
  setup_wifi(ssid,password);
  mqttClient.setServer(mqtt_server,mqtt_port);
  mqttClient.setCallback(callBack);
  while (!Serial);
  Serial.println("LoRa Receiver");
  Serial.println("LoRa Master Node");
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
 
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  //LoRa.setSyncWord(0xBB); // Target
  //LoRa.setSyncWord(0xA4); // Target
  Serial.println("LoRa Initializing OK!");
}
 
//================================================================================================================================================
void loop() 
{
  //-----------------------------------------------------
  if(!mqttClient.connected()){
    reconnect();
  }
  if(millis() - lastTimestamp > 1000){
    String msg("Timestamp from ESP32 [");
    msg += String(millis()) += String("]");
    Serial.print("Publish message: ");
    Serial.println(msg);
    mqttClient.publish("ESP32/heartbeat",msg.c_str());
    mqttClient.loop();
  //-----------------------------------------------------
  currentMillis = millis();
  currentsecs = currentMillis / 1000;
  if ((unsigned long)(currentsecs - previoussecs) >= interval) {
    Secs = Secs + 1;
    //Serial.println(Secs);
    if ( Secs >= 16 )
    {
      Secs = 0;
      //Serial.println("Master Node on Rest........");
    }
    // Time for Node 1 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if ( (Secs >= 1) && (Secs <= 5) )
    {
      LoRa.setSyncWord(Node1); // Target
      int packetSize = LoRa.parsePacket();    // try to parse packet
      if (packetSize) 
      {
      while (LoRa.available())              // read packet
      {
        String LoRaData1 = LoRa.readString();
        //Serial.print(LoRaData1); 
        int pos1 = LoRaData1.indexOf("/"); 
        int pos2 = LoRaData1.indexOf("//");
        String L_B1 = LoRaData1.substring(0, pos1); 
        String ID_1 = LoRaData1.substring(pos1 +1,pos2);
        Serial.print("Level: ");
        Serial.print(L_B1); 
        Serial.println(" cm"); 
        Serial.print("Node :"); 
        Serial.print(ID_1);
             if (ID_1 == String("1")){ mqttClient.publish("FromESP/LevelBin1", L_B1.c_str()); mqttClient.publish("FromESP/ValBin1", ID_1.c_str()); } 
        else if (ID_1 == String("2")){ mqttClient.publish("FromESP/LevelBin2", L_B1.c_str()); mqttClient.publish("FromESP/ValBin2", ID_1.c_str()); }
        else if (ID_1 == String("3")){ mqttClient.publish("FromESP/LevelBin3", L_B1.c_str()); mqttClient.publish("FromESP/ValBin3", ID_1.c_str()); }
      }
      Serial.print(" with RSSI ");         // print RSSI of packet
      Serial.println(LoRa.packetRssi());
      Serial.println("======================================");
      }
    }
    // Time for Node 2 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if ( (Secs >= 6 ) && (Secs <= 10))
    {
      LoRa.setSyncWord(Node2); // Target
      int packetSize = LoRa.parsePacket();    // try to parse packet
      if (packetSize) 
      {
      while (LoRa.available())              // read packet
      {
        String LoRaData2 = LoRa.readString();
        //Serial.print(LoRaData2); 
        int pos1 = LoRaData2.indexOf("/"); 
        int pos2 = LoRaData2.indexOf("//");
        String L_B2 = LoRaData2.substring(0, pos1); 
        String ID_2 = LoRaData2.substring(pos1 +1,pos2);
        Serial.print("Level: ");
        Serial.print(L_B2); 
        Serial.println(" cm"); 
        Serial.print("Node :"); 
        Serial.print(ID_2);
             if (ID_2 == String("1")){ mqttClient.publish("FromESP/LevelBin1", L_B2.c_str()); mqttClient.publish("FromESP/ValBin1", ID_2.c_str()); } 
        else if (ID_2 == String("2")){ mqttClient.publish("FromESP/LevelBin2", L_B2.c_str()); mqttClient.publish("FromESP/ValBin2", ID_2.c_str()); }
        else if (ID_2 == String("3")){ mqttClient.publish("FromESP/LevelBin3", L_B2.c_str()); mqttClient.publish("FromESP/ValBin3", ID_2.c_str()); }
      }
      Serial.print(" with RSSI ");         // print RSSI of packet
      Serial.println(LoRa.packetRssi());
      Serial.println("======================================");
      }
    }
    // Time for Node 3 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if ( (Secs >=11 ) && (Secs <= 15))
    {
      LoRa.setSyncWord(Node3); // Target
      int packetSize = LoRa.parsePacket();    // try to parse packet
      if (packetSize) 
      {
      while (LoRa.available())              // read packet
      {
        String LoRaData3 = LoRa.readString();
        //Serial.print(LoRaData3); 
        int pos1 = LoRaData3.indexOf("/"); 
        int pos2 = LoRaData3.indexOf("//");
        String L_B3 = LoRaData3.substring(0, pos1); 
        String ID_3 = LoRaData3.substring(pos1 +1,pos2);
        Serial.print("Level: ");
        Serial.print(L_B3); 
        Serial.println(" cm"); 
        Serial.print("Node :"); 
        Serial.print(ID_3); 
             if (ID_3 == String("1")){ mqttClient.publish("FromESP/LevelBin1", L_B3.c_str());mqttClient.publish("FromESP/ValBin1", ID_3.c_str()); } 
        else if (ID_3 == String("2")){ mqttClient.publish("FromESP/LevelBin2", L_B3.c_str());mqttClient.publish("FromESP/ValBin2", ID_3.c_str()); }
        else if (ID_3 == String("3")){ mqttClient.publish("FromESP/LevelBin3", L_B3.c_str());mqttClient.publish("FromESP/ValBin3", ID_3.c_str()); }
      }
      Serial.print(" with RSSI ");         // print RSSI of packet
      Serial.println(LoRa.packetRssi());
      Serial.println("======================================");
      }
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    previoussecs = currentsecs;
  }
  // parse for a packet, and call onReceive with the result:
  lastTimestamp = millis();
  }
}
