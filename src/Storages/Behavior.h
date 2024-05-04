enum Cmd_Behavior: uint8_t {
   ACTION_OUTPUT = 0xA0,
   ACTION_WS2812 = 0xA1,
   ACTION_REPORT = 0xA2,
   ACTION_SEND = 0xA3,
   ACTION_NONE = 0xAF
};

enum BCue_Trigger: uint8_t {
   BTRIGGER_STARTUP = 0xB0,
   BTRIGGER_SINGLECLICK = 0xB1,
   BTRIGGER_DOUBLECLICK = 0xB2,
   BTRIGGER_PIR = 0xB3,
   BTRIGGER_IR = 0xB4,
   BTRIGGER_STATE = 0xB5,
   BTRIGGER_NONE = 0xF1,
};

struct Data_Behavior {
   BCue_Trigger cue;
   uint8_t peerId;
   uint8_t data[32]; 
   
   void toCharArr(char* charArr) const {
      memcpy(charArr, this, sizeof(Data_Behavior));
   }

   bool check(uint8_t peerIdIdVal, BCue_Trigger cueVal) {
      return peerId == peerIdIdVal && cue == cueVal;
   }
   
   template <typename T>
   void load(uint8_t peerIdVal, BCue_Trigger cueVal, T* control) {
      memcpy(data, control, sizeof(T));
      peerId = peerIdVal;
      cue = cueVal;
   }

   template <typename T>
   void load(uint8_t peerIdVal, const char* cueStr, T* control) {
      BCue_Trigger trigger = BTRIGGER_NONE;

      if (strcmp(cueStr, "1CLICK") == 0) {
         trigger = BTRIGGER_SINGLECLICK;
      }
      else if (strcmp(cueStr, "2CLICK") == 0) {
         trigger = BTRIGGER_DOUBLECLICK;
      }
      else if (strcmp(cueStr, "CUE_PIR") == 0) {
         trigger = BTRIGGER_PIR;
      }
      else if (strcmp(cueStr, "CUE_IR") == 0) {
         trigger = BTRIGGER_IR;
      }
      else if (strcmp(cueStr, "CUE_STATE") == 0) {
         trigger = BTRIGGER_STATE;
      }
      else if (strcmp(cueStr, "CUE_THRESHOLD") == 0) {
         trigger = BTRIGGER_NONE;
      }

      load(peerIdVal, trigger, control);
   }

   template <typename T>
   void produce(T *control) {
      memcpy(control, data, sizeof(T));
   }

   void clear() {
      peerId = 255;
      cue = BTRIGGER_NONE;
      memset(data, 0, sizeof(data));
   }

   void printData() {
      Serial.printf("cue = %u, peerId = %u", cue, peerId);
      // AppPrintHex(data, sizeof(data));
   }
};

class ControlOutput {
   Cmd_Behavior actionCmd = ACTION_OUTPUT;

   public:
      uint8_t pin, value;

      ControlOutput(uint8_t pinVal, uint8_t valueVal) {
         pin = pinVal;
         value = valueVal;
      }

      bool extract(Data_Behavior* behav) {
         behav->produce(this);
         return actionCmd == ACTION_OUTPUT;
      }
};

class ControlWS2812 {
   Cmd_Behavior actionCmd = ACTION_WS2812;

   public:
      uint8_t pin, value;

      ControlWS2812(uint8_t pinVal, uint8_t valueVal) {
         pin = pinVal;
         value = valueVal;
      }


      bool extract(Data_Behavior* behav) {
         behav->produce(this);
         return actionCmd == ACTION_WS2812;
      }
};

class ControlReport {
   public:
      float value1, value2, value3;
};

class ControlSend {
   public:
      char myStr[16] = "testStr";
      Cmd_Behavior actionCmd = ACTION_SEND;
};

