#include <Arduino.h>
#include "SerialControl.h"
#include "2Storage.h"

SerialControl serial;
Mng_Storage storage;

void setup() {
  Serial.begin(115200);
}

void loop() {
  serial.run([&](char* inputStr) {
      if (strcmp(inputStr, "ping") == 0)  {
          Serial.println("What is thy bidding my Master?");
      }       
      else if (strcmp("wifiReset", inputStr) == 0) {
          // onHandleResetWifi();
      }
      else if (strcmp("startAP", inputStr) == 0) {
          // onHandleStartAP();
      }
      else if (storage.handleConsoleCmd(inputStr)) {
      } 
  });
}
