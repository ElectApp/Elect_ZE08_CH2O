/*
 * Arduino library for ZE08-CH2O module
 * https://www.winsen-sensor.com/d/files/PDF/Gas%20Sensor%20Module/Formaldehyde%20Detection%20Module/ZE08-CH2O%20V1.0.pdf
 * 
 * Created by Somsak Elect, 17-Jan-2021
 * 
*/

#ifndef ELECT_ZE08_CH2O_H
#define ELECT_ZE08_CH2O_H

#include <Arduino.h>

//Follow table 4 at ZE08-CH2O datasheet
typedef enum {
  ZE08_START=0,
  ZE08_GAS_NAME,
  ZE08_UNIT,
  ZE08_CONCENTRATION=4, //2 byte (H + L)
  ZE08_RANGE=6,         //2 byte (H + L)
  ZE08_CHK=8,
  ZE08_FRAME_LEN
}ZE08_FRAME_INDEX;

typedef enum {
  ZE08_NO_ERROR=0,
  ZE08_INVALID_LENGTH,
  ZE08_INVALID_HEADER,
  ZE08_INVALID_GAS_NAME,
  ZE08_INVALID_CHK
}ZE08_ERROR_CODE;

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define CALLBACK_NEW_DATA std::function<void(short, ZE08_ERROR_CODE)> callbackNewData
#else
#define CALLBACK_NEW_DATA void (*callbackNewData)(short, ZE08_ERROR_CODE)
#endif

class Elect_ZE08_CH2O {
public:
  //Object
  Elect_ZE08_CH2O(HardwareSerial &_stream, CALLBACK_NEW_DATA);

  //Function
  void begin(int RXD_Pin, int TXD_Pin);     //Initial Serial port
  void handle();  //Call at main loop

  //Callback
  //Elect_ZE08_CH2O& setCallbackNewData(CALLBACK_NEW_DATA);
  

private:
  //Serial port
  HardwareSerial* _stream;
  
  //Save last time that received data for checking end frame
  unsigned long lastTime = 0;
  
  //Buffer
  byte frameLen = 0;
  byte frame[ZE08_FRAME_LEN];

  //Function
  void checkFrame();
  void sendCallbackNewData(short data, ZE08_ERROR_CODE ex);

  //Callback
  CALLBACK_NEW_DATA;  //Pass 'CH2O data' and 'Error Code'
};

#endif
