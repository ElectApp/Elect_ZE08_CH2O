/*
 * Arduino library for ZE08-CH2O module
 * https://www.winsen-sensor.com/d/files/PDF/Gas%20Sensor%20Module/Formaldehyde%20Detection%20Module/ZE08-CH2O%20V1.0.pdf
 * 
 * Created by Somsak Elect, 17-Jan-2021
 * 
*/

#include "Elect_ZE08_CH2O.h"


/**
 * Initialize the serial object.
 * 
 */
Elect_ZE08_CH2O::Elect_ZE08_CH2O(HardwareSerial &_stream, CALLBACK_NEW_DATA){
  this->_stream = &_stream;
  this->callbackNewData = callbackNewData;
}

/*
 * Config TX and RX pin of serial port
*/
void Elect_ZE08_CH2O::begin(int RXD_Pin, int TXD_Pin){
  //Initial Serial port
  _stream->begin(9600, SERIAL_8N1, RXD_Pin, TXD_Pin);
}

/*
 * Waiting data from sensor module
*/
void Elect_ZE08_CH2O::handle(){
  //Get serial data
  if(_stream->available()>0){
    while(_stream->available()){
      if(frameLen>=ZE08_FRAME_LEN){
        //Clear
        _stream->read();
      }else{
        //Save
        frame[frameLen++] = _stream->read();
        //Serial.println(frame[frameLen-1], HEX);
      }
    }
    //Save time
    lastTime = millis();
  }
  
  //End frame?
  if(frameLen>0 && (millis()-lastTime>10)){
    checkFrame();
  }
}

/*
 * Frame Checking
*/
void Elect_ZE08_CH2O::checkFrame(){
  //Serial.println();
  //Length
  if(frameLen<ZE08_FRAME_LEN || frame[ZE08_START]!=0xFF || frame[ZE08_GAS_NAME]!=0x17){
    sendCallbackNewData(0, ZE08_INVALID_LENGTH);
    return;   
  }  
  //Header
  if(frame[ZE08_START]!=0xFF){
    sendCallbackNewData(0, ZE08_INVALID_HEADER);
    return;   
  }  
  //Gas name
  if(frame[ZE08_GAS_NAME]!=0x17){
    sendCallbackNewData(0, ZE08_INVALID_GAS_NAME);
    return;   
  }    
  //Cal. Checksum = Not(Byte1+Byte2+...Byte7)
  frameLen -= 1;
  byte j, sum = 0;
  for(j=0; j<frameLen; j++){
    sum += frame[j];
  }
  sum = (~sum);
  //Serial.print("Checksum: "); Serial.print(sum); Serial.print(" vs "); Serial.println(frame[ZE08_CHK]);
  
  //Final Check
  if(sum!=frame[ZE08_CHK]){
    //Error
    sendCallbackNewData(0, ZE08_INVALID_CHK);
  }else{
    //Normal
    sendCallbackNewData(word(frame[ZE08_CONCENTRATION], frame[ZE08_CONCENTRATION+1]), ZE08_NO_ERROR);
  }
}

/*
 * Pass Data
*/
void Elect_ZE08_CH2O::sendCallbackNewData(short data, ZE08_ERROR_CODE ex){
  //Clear
  frameLen = 0;
  //Callback
  if(callbackNewData){
    callbackNewData(data, ex);
  }
}
