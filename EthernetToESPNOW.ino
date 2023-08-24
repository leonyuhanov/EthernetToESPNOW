#include <Arduino.h>
#include <ETH.h>
#include <AsyncUDP.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <math.h>
#include "genBuffer.h"

//Ethernet Config
IPAddress thisDevice(192,168,100,250);   
IPAddress gateway(192,168,100,254);   
IPAddress subnet(255,255,255,0);

//ESPNOW
#define WIFI_CHANNEL 1
esp_now_peer_info_t* ESPClients;
uint8_t localMac[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x00};
const uint8_t maxDataFrameSize=250;
uint8_t dataToSend[maxDataFrameSize];

//UDP config
const unsigned int updRXPort = 10000;

//ESPNOW Clients
const unsigned short int numberOfDevices = 2;
const uint8_t deviceMacAddresses[numberOfDevices][6] = {{0x30,0x30,0x30,0x30,0x30,0x01},{0x30,0x30,0x30,0x30,0x30,0x02}};

AsyncUDP udp;

//Data BUffer
genBuffer globalBuffer;

void setup()
{
	Serial.begin(115200);
	Serial.printf("\r\n\r\nBooting.");
  
  ETH.begin();
	ETH.config(thisDevice, gateway, subnet);
	Serial.printf("\r\nONLINE via Ethernet\t");
	Serial.print(ETH.localIP());

  //Set up ESPNOW
  WiFi.mode(WIFI_STA);
  //WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
  esp_wifi_set_mac(WIFI_IF_STA, &localMac[0]);
  Serial.printf("\r\nESPNOW MAC Address\t");
  Serial.println( WiFi.macAddress() );
  WiFi.disconnect();
  ESP_ERROR_CHECK(esp_wifi_set_channel(WIFI_CHANNEL,WIFI_SECOND_CHAN_NONE));
  if(esp_now_init() == ESP_OK)
  {
    Serial.printf("\r\nESP NOW INIT!");
  }
  else
  {
    Serial.printf("\r\nESP NOW INIT FAILED....");
  }
  ESPClients = new esp_now_peer_info_t[numberOfDevices];
  for(unsigned short int devCount=0; devCount<numberOfDevices; devCount++)
  {
    memcpy( &ESPClients[devCount].peer_addr, &deviceMacAddresses[devCount], 6 );
    ESPClients[devCount].channel = WIFI_CHANNEL;
    ESPClients[devCount].encrypt = 0;
    ESPClients[devCount].ifidx = WIFI_IF_STA;
    if( esp_now_add_peer(&ESPClients[devCount]) == ESP_OK)
    {
    Serial.printf("\r\n\tAdded ESPNOW Peer\t%d", devCount);
    Serial.printf("\t[%X:%X:%X:%X:%X:%X]", deviceMacAddresses[devCount][0], deviceMacAddresses[devCount][1], deviceMacAddresses[devCount][2], deviceMacAddresses[devCount][3], deviceMacAddresses[devCount][4], deviceMacAddresses[devCount][5]);
    }    
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  //Set up UDP
  udp.listen(updRXPort);
  udp.onPacket(onUDPData);
}

void loop()
{
  //check if theres anythign in the buffer
  if(globalBuffer.totalNodes)
  {
    //send current block to espnow
    forwardToESPNOW(globalBuffer.getData(), globalBuffer.getDataLen());
    globalBuffer.deleteFirst();
  }
  yield();
	
}

void onUDPData(AsyncUDPPacket &packet)
{
  globalBuffer.add(packet.data(), packet.length());
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  //maybe code up return path from ESPNOW to UDP
}

void forwardToESPNOW(uint8_t* sourceData, unsigned short int sourceDataLength)
{
    //SPlit up your data into maxDataFrameSize blocks tehen TX like this
	//use sourceData[0] for device addressing
	//esp_now_send(ESPClients[sourceData[0]].peer_addr, dataToSend, maxDataFrameSize);
}