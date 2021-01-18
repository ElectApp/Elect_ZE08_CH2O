/*
 * ZE08-CH2O Testing
 * 
 * Tested on ESP32
 * By Somsak Elect, 18-Jan-2021
 * 
 * Connection:
 * Serial2 (RX2=16, TX2=17) -> ZE08
 * Serial (RX1=3, TX1=1)   -> Debug
 * 
 * 
*/

#include "Elect_ZE08_CH2O.h"


#define ZE08_RX_PIN   16
#define ZE08_TX_PIN   17

Elect_ZE08_CH2O ze08(Serial2, &callbackNewData);  

//This function will work when has a new data (Every 1 seconds)
void callbackNewData(short data, ZE08_ERROR_CODE ex){
  if(ex){
    Serial.print("CH2O ERROR: "); Serial.println(ex);
  }else{
    Serial.print("CH2O: "); Serial.print(data); Serial.println(" ppb");
  }
}

void setup() {
  //Initial debuger
  Serial.begin(115200);
  //Initial
  ze08.begin(ZE08_RX_PIN, ZE08_TX_PIN);
}

void loop() {
  ze08.handle();
}
