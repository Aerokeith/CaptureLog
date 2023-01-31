#include <Arduino.h>

#define CLOG_ENABLE true    // this must be defined before cLog.h is included
#include "cLog.h"

const uint16_t maxEntries = 10;
const uint16_t maxEntryChars = 20;

  // Create a cLog
CLOG_NEW myLog1(maxEntries, maxEntryChars, NO_TRIGGER, NO_WRAP);

void setup() {
  randomSeed(4013687787);
  Serial.begin(9600);
  delay(3000);

    // Fill each cLog with random numbers
  for (uint8_t i = 0; i < maxEntries; i++) {
    CLOG(myLog1.add(), "foo[%hu] = %d", i, (int) random(100));
  }

  // Compile this section only if CLOG_ENABLE is true
#if CLOG_ENABLE
  Serial.println("This is myLog1");
  for (uint16_t entry = 0; entry < myLog1.numEntries; entry++) {
    Serial.println(myLog1.get(entry));
  }
#endif

}


void loop() {
}