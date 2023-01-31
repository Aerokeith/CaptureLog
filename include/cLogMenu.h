/* The CaptureLogMenu library (consisting of cLogMenu.h and cLogMenu.cpp) is intended to be used in conjuntion with the
    CaptureLog library. The CaptureLogMenu library implements a C++ class that enables selective printing of capture
    logs via a simple character-based menu that utilizes the Serial Monitor (or equivalent terminal emulator). 
*/

#include <Arduino.h>
#include "cLog.h"

  // messages used to provide user feedback
const char continueMsg[] = "[Press any key to continue]";
const char promptMsg[] = "Select debug log";
const char emptyLogMsg[] = "<log is empty>";
const char emptyMenuMsg[] = "No logs have been added to the menu";
const char badSelMsg[] = "Unknown command";
const char exitMsg[] = "Exit";
const char exitChar = 'x';

  // Structure used to describe the cLogs added to the menu
struct menuConfigStruct {
  cLogClass * logP;         // pointer to a cLog that is added to the menu
  char selectionChar;       // menu character used to select this cLog
  const char *headerString; // descriptive header string used when printing cLog pages
};

  // Class containing the data and menthods needed to implement the cLog menu
class cLogMenuClass {
  uint8_t maxTableLogs;         // max number of cLogs that can be added to this menu object
  menuConfigStruct * menuTable; // pointer to a dynamically allocated array of menuConfigStruct
  uint8_t endIndex;             // index of the first unused menuConfigStruct in menuTable
  uint16_t pageLength;          // page length (number of entries) for printing cLogs
public:
    // see cLogMenu.cpp for documentation of the following class methods
  cLogMenuClass(uint8_t maxLogs, uint16_t pageLen);
  void addLog(cLogClass *log, char selChar, const char *header);
  void printLog(uint8_t index);
  void logMenu();
};
