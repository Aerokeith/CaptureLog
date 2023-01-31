#include <Arduino.h>

#define CLOG_ENABLE true
#include "cLog.h"
#include "cLogMenu.h"

const int maxLines = 20;

CLOG_NEW myLog1(20, 20, NO_TRIGGER, NO_WRAP);
CLOG_NEW myLog2(20, 20, NO_TRIGGER, NO_WRAP);
CLOG_NEW myLog3(20, 20, NO_TRIGGER, NO_WRAP);

void setup() {
  randomSeed(4013687787);
  Serial.begin(9600);
  delay(3000);

  for (uint8_t i = 0; i < 20; i++) {
    CLOG(myLog1.add(), "foo[%hu] = %d", i, (int) random(100));
    CLOG(myLog2.add(), "foo[%hu] = %d", i, (int) random(100));
    CLOG(myLog3.add(), "foo[%hu] = %d", i, (int) random(100));
  }

#if CLOG_ENABLE
  cLogMenuClass menu(3, 10);
  menu.addLog(&myLog1, '1', "This is myLog1");
  menu.addLog(&myLog2, '2', "This is myLog2");
  menu.addLog(&myLog3, '3', "This is myLog3");
  menu.logMenu();
#endif

}


void loop() {
}