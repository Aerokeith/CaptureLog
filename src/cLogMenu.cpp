/* The CaptureLogMenu library (consisting of cLogMenu.h and cLogMenu.cpp) is intended to be used in conjuntion with the
    CaptureLog library. The CaptureLogMenu library implements a C++ class that enables selective printing of capture
    logs via a simple character-based menu that utilizes the Serial Monitor (or equivalent terminal emulator). 
*/

#include <Arduino.h>
#include "cLog.h"
#include "cLogMenu.h"

/* cLogMenuClass::cLogMenuClass()
    Class object constructor, called when a new cLog menu object is instantiated
  Parameters:
    uint8_t maxLogs: max number of cLogs that can be added to this menu, used for memory allocation
    uint16_t pageLen: max number of cLog entries to print per page. pageLen = 0 disabled pagination
  Returns: None
*/
cLogMenuClass::cLogMenuClass(uint8_t maxLogs, uint16_t pageLen) {
  maxTableLogs = maxLogs;   // remember this for later
  pageLength = pageLen;     // remember this for later
  endIndex = 0;             // menuTable is empty
  menuTable = new menuConfigStruct[maxTableLogs];   // allocate memory for menu config structures
}


/* cLogMenuClass::addLog()
    Adds a new cLog (and associated config parameters) to the menuTable
  Parameters: 
    cLogClass *log: Pointer to the cLog to be added to the menu
    char selChar: Character to be used to select this cLog from the menu prompt
    const char *header: Header string to be printed at the top of each cLog page
  Returns: None
*/
void cLogMenuClass::addLog(cLogClass *log, char selChar, const char *header) {
  if (endIndex < maxTableLogs) {                  // if table isn't full
    menuTable[endIndex].selectionChar = selChar;
    menuTable[endIndex].headerString = header;
    menuTable[endIndex].logP = log;
    endIndex++;
  }
}


/* cLogMenuClass::printLog()
    Prints a specified cLog fromn the menu table, with pagination is enabled
  Parameters:
    uint8_t index: index into menuTable specifying the cLog to be printed
  Returns: None
*/
void cLogMenuClass::printLog(uint8_t index) {
  menuConfigStruct * configP; // temp pointer to an entry in menuTable
  cLogClass *logPtr;          // temp pointer to a cLog
  uint16_t entry;             // temp used to count the number of cLog entries printed
  uint16_t entryPageCount;    // counts the number of entries printyed on the current page
  char *logData;              // pointer to the cLog entry (string)
  char inputChar;             // keystroke from serial monitor
  
  if (index >= endIndex)  // if index is past the last entry in the menu table (shouldn't happen)
    return ;
  configP = &menuTable[index];  // get a pointer to the selected menuTable entry
  Serial.println(configP->headerString);  // print the cLog-specific header
  entry = 0;  // no entries printed yet
  entryPageCount = 0;   // no entries printed on the first page
  logPtr = configP->logP;   // get a pointer to the selected cLog
  if (logPtr->numEntries == 0) {    // if the cLog has no entries
    Serial.println(emptyLogMsg);    
    return ;
  }
  while (entry < logPtr->numEntries) {    // for each entry in the cLog
    logData = logPtr->get(entry);         // get pointer to the current cLog entry
    Serial.print('[');
    Serial.print(entry);
    Serial.print("] ");
    Serial.println(logData);              // print it preceded by the entry number
    entry++;            // count the total entries printed
    entryPageCount++;   // count the entries printed on this page
    if (pageLength != 0) {  // if pagination isn't disabled
        // if not done and page length limit is reached
      if ((entry < logPtr->numEntries) && (entryPageCount == pageLength)) {
        entryPageCount = 0;   //; reset the page count
        Serial.println(continueMsg);        // print the continue prompt
        Serial.println();
        while (!Serial.available()) { }     // wait for a key to be pressed
        inputChar = Serial.read();          // get the char
        while (Serial.available())          // discard any following chars (CR/LF from Arduino serial monitor)
          Serial.read();
        if (inputChar == exitChar)          // if it's the exit char, bail out
          return ;
        Serial.println(configP->headerString);  // print the header to start the next page
      }
    }
  }
}

/* cLogMenuClass::logMenu()
    Implements the menu for printing specified cLogs
  Parameters: Nobne
  Returns: None
*/
void cLogMenuClass::logMenu() {
  char selection;   // temp for storing a keystroke fromn the serial monitor
  uint8_t n;        // used as index to search menuTable

  if (endIndex == 0) {    // if the menuTable is empty
    Serial.println(emptyMenuMsg);
    return;
  }
  while (1) {         // repeat until exit caused by pressing exit char
    Serial.println();
    Serial.print(promptMsg);    // print the first part of the menu prompt
    Serial.print("[ ");
    for (n = 0; n < endIndex; n++) {            //; print each of the specified selection characters
      Serial.print(menuTable[n].selectionChar);
      Serial.print(' ');
    }
    Serial.print("] >> ");            // finish printing the menu prmpt
    while (!Serial.available()) { }   // wait until a serial monitor key is pressed
    selection = Serial.read();        // get the char
    while (Serial.available())        // clear the input buffer (get rid of CR/LF on Arduino serial monitor)
      Serial.read();
    if (selection == exitChar) {        // if its an exit char
      Serial.println(exitMsg);         // print the menu exit message
      return ;
    }
    else  // key pressed was not exit char
      Serial.println(selection);  // echo the character
    Serial.println();
    n = 0;    // prepare to search through the selection characters in menuTable
      // while end not reached and no selection character match found
    while ((n < endIndex) && (selection != menuTable[n].selectionChar)) { n++; }
    if (n == endIndex)  // if end was reached with no match
      Serial.println(badSelMsg);  // print the "unrecognized selection" message
    else {  // match was found
      printLog(n);    // print the specified cLog
    }
  }
}

