#include <LedControl.h>
const int DATA_IN = 2;   
const int LOAD = 3;      
const int CLOCK = 4;     
const int MAX_DEVICES = 1; 
const int BUTTON_PIN = 5;  

LedControl lc = LedControl(DATA_IN, CLOCK, LOAD, MAX_DEVICES);

// Smiley face pattern
byte smileyFace[8] = {
  B00111100,  
  B01000010,
  B10101001,
  B10000101,
  B10000101,
  B10101001,
  B01000010,
  B00111100,
};

// Check mark pattern
byte checkMark[8] = {
  B00010000,  
  B00001000,  
  B00000100,  
  B00000010,  
  B00000001, 
  B00000010,  
  B00000100,  
  B00000000,  
};

// X pattern
byte xPattern[8] = {
  B10000001,  
  B01000010,  
  B00100100,  
  B00011000,  
  B00011000,  
  B00100100,  
  B01000010,  
  B10000001,  
};

unsigned long buttonPressTime = 0; 
unsigned long lastPressTime = 0;   
bool xDisplayed = false;          
bool checkMarkDisplayed = false;   
bool buttonPressedOnce = false;    
bool isButtonPressed = false;      

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  
  for (int i = 0; i < MAX_DEVICES; i++) {
    lc.shutdown(i, false);      
    lc.setIntensity(i, 8);    
    lc.clearDisplay(i);          
  }

  // :) 
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, smileyFace[row]);
  }
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    unsigned long currentTime = millis();
    
    // Buffer to avoid showing check early
    if (currentTime - lastPressTime > 200) {
      lastPressTime = currentTime;

      if (buttonPressedOnce && currentTime - buttonPressTime < 500 && !xDisplayed) {         // Detects second press
        for (int row = 0; row < 8; row++) {
          lc.setRow(0, row, xPattern[row]);
        }
        xDisplayed = true;
        buttonPressTime = currentTime;
        buttonPressedOnce = false;
      } 
      else if (!buttonPressedOnce && !xDisplayed) {
        buttonPressTime = currentTime; 
        buttonPressedOnce = true; 
      }
    }
  } 
  else if (buttonPressedOnce && millis() - buttonPressTime > 250) {
    if (!xDisplayed) {
      for (int row = 0; row < 8; row++) {
        lc.setRow(0, row, checkMark[row]);
      }
      checkMarkDisplayed = true; 
      buttonPressedOnce = false;  
      buttonPressTime = millis(); 
    }
  }

  if (checkMarkDisplayed && millis() - buttonPressTime >= 1000) {
    for (int row = 0; row < 8; row++) {
      lc.setRow(0, row, smileyFace[row]);
    }
    checkMarkDisplayed = false;  
  }

  if (xDisplayed && millis() - buttonPressTime >= 1000) {
    for (int row = 0; row < 8; row++) {
      lc.setRow(0, row, smileyFace[row]);
    }
    xDisplayed = false;  
  }
}
