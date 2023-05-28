/*****************************************************************************************************************************************************************************************
           < LoRa Reciver >
  < WASTE MESUREMENT Data Collector >
******************************************************************************************************************************************************************************************/
#include <LoRa.h>
#include <SPI.h>
// ESP 32 LoRa Pin-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ss   5
#define rst 14
#define dio0 2
// My Node Byte----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte Node1 = 0xBB;
byte Node2 = 0xCC;
byte Node3 = 0xDD;
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String outgoing;              // outgoing message
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte msgCount = 0;            // count of outgoing messages
String incoming = "";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Tracks the time since last event fired
unsigned long previousMillis   = 0;
unsigned long int previoussecs = 0;
unsigned long int currentsecs  = 0;
unsigned long currentMillis    = 0;
int interval = 1 ; // updated every 1 second
int Secs = 0;


void setup() 
{
  Serial.begin(115200);
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
      Serial.print("Level: ");
      while (LoRa.available())              // read packet
      {
        String LoRaData1 = LoRa.readString();
        //Serial.print(LoRaData1); 
        int pos1 = LoRaData1.indexOf("/"); 
        int pos2 = LoRaData1.indexOf("//");
        String L_B1 = LoRaData1.substring(0, pos1); 
        String ID_1 = LoRaData1.substring(pos1 +1,pos2);
        Serial.print(L_B1); 
        Serial.println(" cm"); 
        Serial.print("Node :"); 
        Serial.print(ID_1); 
        //Serial.println(ID_1);
      }
      Serial.print(" with RSSI ");         // print RSSI of packet
      Serial.println(LoRa.packetRssi());
      Serial.println("=================================");
      }
    }
    // Time for Node 2 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if ( (Secs >= 6 ) && (Secs <= 10))
    {
      LoRa.setSyncWord(Node2); // Target
      int packetSize = LoRa.parsePacket();    // try to parse packet
      if (packetSize) 
      {
      Serial.print("Level: ");
      while (LoRa.available())              // read packet
      {
        String LoRaData2 = LoRa.readString();
        //Serial.print(LoRaData2); 
        int pos1 = LoRaData2.indexOf("/"); 
        int pos2 = LoRaData2.indexOf("//");
        String L_B2 = LoRaData2.substring(0, pos1); 
        String ID_2 = LoRaData2.substring(pos1 +1,pos2);
        Serial.print(L_B2); 
        Serial.println(" cm"); 
        Serial.print("Node :"); 
        Serial.print(ID_2);
        //Serial.println(ID_2);
      }
      Serial.print(" with RSSI ");         // print RSSI of packet
      Serial.println(LoRa.packetRssi());
      Serial.println("=================================");
      }
    }
    // Time for Node 3 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    if ( (Secs >=11 ) && (Secs <= 15))
    {
      LoRa.setSyncWord(Node3); // Target
      int packetSize = LoRa.parsePacket();    // try to parse packet
      if (packetSize) 
      {
      Serial.print("Level: ");
      while (LoRa.available())              // read packet
      {
        String LoRaData3 = LoRa.readString();
        //Serial.print(LoRaData3); 
        int pos1 = LoRaData3.indexOf("/"); 
        int pos2 = LoRaData3.indexOf("//");
        String L_B3 = LoRaData3.substring(0, pos1); 
        String ID_3 = LoRaData3.substring(pos1 +1,pos2);
        Serial.print(L_B3); 
        Serial.println(" cm"); 
        Serial.print("Node :"); 
        Serial.print(ID_3);  
        //Serial.println(ID_3); 
      }     
      Serial.print(" with RSSI ");         // print RSSI of packet
      Serial.println(LoRa.packetRssi());
      Serial.println("=================================");
      }
    }
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    previoussecs = currentsecs;
  }
  // parse for a packet, and call onReceive with the result:
}
