#include <Arduino.h>

#define CLOG_ENABLE true    // this must be defined before cLog.h is included
#include "cLog.h"
#include "cLogMenu.h"

const uint8_t numLogs = 3;
const uint16_t maxEntries = 10;
const uint16_t maxEntryChars = 20;
const uint16_t pageLen = 5;

  // Create 3 cLogs with identical characteristics
CLOG_NEW myLog1(maxEntries, maxEntryChars, NO_TRIGGER, NO_WRAP);
CLOG_NEW myLog2(maxEntries, maxEntryChars, NO_TRIGGER, NO_WRAP);
CLOG_NEW myLog3(maxEntries, maxEntryChars, NO_TRIGGER, NO_WRAP);

void setup() {
  randomSeed(4013687787);
  Serial.begin(9600);
  delay(3000);

    // Fill each cLog with random numbers
  for (uint8_t i = 0; i < maxEntries; i++) {
    CLOG(myLog1.add(), "foo[%hu] = %d", i, (int) random(100));
    CLOG(myLog2.add(), "foo[%hu] = %d", i, (int) random(100));
    CLOG(myLog3.add(), "foo[%hu] = %d", i, (int) random(100));
  }

  // Compile this section only if CLOG_ENABLE is true
#if CLOG_ENABLE
  cLogMenuClass menu(numLogs, pageLen);         // create a menu structure that can accommodate all 3 cLogs
    // Add each of tghe cLogs to the menu
  menu.addLog(&myLog1, '1', "This is myLog1");
  menu.addLog(&myLog2, '2', "This is myLog2");
  menu.addLog(&myLog3, '3', "This is myLog3");
  menu.logMenu();   // initiate the menu for selectively printing the cLogs
#endif

}


void loop() {
}