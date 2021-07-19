#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

/*
const char* ssid     = ""; // EDIT
const char* password = ""; // EDIT
 */
#include "wifi_config.h"

extern const char* ssid;
extern const char* password;

// display
TFT_eSPI tft = TFT_eSPI();

#define CURRENCY 6
// 1 - USD; 6 - PLN

// GLOBALS
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

// time settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// json object
StaticJsonDocument<256> doc;
String jsonData = "";

// item class
struct ItemData{
  String shortName;
  String fullName;
  String appId;
  String priceHistory = "unk";
  String volumeHistory = "unk";

  ItemData(String shortName, String fullName, String appId){ // constructor
    this->shortName = shortName;
    this->fullName = fullName;
    this->appId = appId;
  }
};

// item object
int selectedItem = 0;
const int itemCount = 3;
ItemData items[itemCount] = {
  {"Broken Fang Case", "Operation%20Broken%20Fang%20Case", "730"},
  {"Battle Scarred Holo", "Sticker%20%7C%20Battle%20Scarred%20%28Holo%29", "730"},
  {"TF2 Key", "Mann%20Co.%20Supply%20Crate%20Key", "440"},
  };

void clearLineAfterCursor(){
  while(tft.getCursorX() < tft.width()){
    tft.print(" ");
  }
}

// printing time in the first line
void printLocalTime(){
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    tft.print("Failed to get time");
  } else {
    tft.print(&timeinfo, "%H:%M %d-%m-%Y");
  }
  clearLineAfterCursor();
}

// printing short item name
void printItemName(ItemData items[], int selectedItem){
  tft.fillRect(0, 20, tft.width(), 38, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 20);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);
  tft.print(items[selectedItem].shortName);
}

// print history header
void printHistoryHeader(){
  tft.setCursor(0, 58);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.print("Price:");
  clearLineAfterCursor();
  tft.setCursor(0, 98);
  tft.print("Volume:");
  clearLineAfterCursor();
}

// get JSON data
void getJsonData(ItemData items[], int selectedItem){
  String endpoint = "https://steamcommunity.com/market/priceoverview/?appid=" \
  + items[selectedItem].appId \
  + "&currency=" + CURRENCY \
  + "&market_hash_name=" + items[selectedItem].fullName;
  //Serial.println(endpoint);
  
  HTTPClient http;
  String jsonData;
  http.begin(endpoint);
  int httpCode = http.GET();

  if (httpCode > 0){
    jsonData = http.getString();
    Serial.println(jsonData);
    Serial.println(httpCode);
  } else {
    Serial.println("Error on HTTP request");
  }
  http.end();

  deserializeJson(doc, jsonData);
  
}

// get price of item
String getPrice(){
  String price = doc["lowest_price"];
  price.replace("zł", " pln");

  return price;
}

// get volume of item
String getVolume(){
  String volume = doc["volume"];
  volume.replace(",", " ");
  
  return volume;
}

// update history
void updateHistory(ItemData items[], int itemCount){
  for(int i = 0; i < itemCount; ++i){
    delay(500);
    getJsonData(items, i); // get prices from steam market
    items[i].priceHistory = getPrice();
    items[i].volumeHistory = getVolume();
  }
}

// print history
void printHistory(ItemData items[], int selectedItem){
  tft.setTextSize(2);
  tft.setCursor(0, 78);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.print(items[selectedItem].priceHistory);
  clearLineAfterCursor();
  tft.setCursor(0, 118);
  tft.print(items[selectedItem].volumeHistory);
  clearLineAfterCursor();
}

void setup() {
  Serial.begin(115200);
  
  // setup display
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);

  int backlight[5] = {10,30,60,120,220};
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, backlight[2]);

  // connect to WiFi
  Serial.printf("Connecting to: %s\n", ssid);
  tft.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      tft.print(".");
  }
  tft.fillScreen(TFT_BLACK);
  Serial.println("CONNECTED!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // init time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // print history header
  printHistoryHeader();

  //tft.setTextColor(TFT_WHITE,TFT_BLACK);
  // disconnect WiFi
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
  
}

const int slowLoopN = 120; // 120 * 5s = 10min
int loopN = slowLoopN; // loop every slowLoopN times
void loop() {
  
  if (selectedItem > itemCount - 1) selectedItem = 0;
  
  if(loopN >= slowLoopN){ // SLOW LOOP
    loopN = 0;
    // print time LOOP SLOW
    printLocalTime();
  
    // update history LOOP SLOW
    updateHistory(items, itemCount); // itemCount * delay(500) + network
  }
  
  // print item name LOOP
  printItemName(items, selectedItem);

  // print history LOOP
  printHistory(items, selectedItem);

  selectedItem++;
  loopN++;
  delay(5000);
}
