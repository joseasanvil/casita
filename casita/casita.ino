#include <EEPROM.h>
#define EEPROM_ADDR 0 // EEPROM address to store the state

const int onOffSwitchPin = 2;   // Switch input pin
const int stateSwitchPin = 3;   // Switch input pin
const int bulbPin = 9;          // Output pin for the bulb (or relay)
int bulbPower = 0;

int onOffState = 1;             // Variable to store switch state
int bulbState = 0;              // Bulb state
int numStates = 4;              // Number of states

int stateSwitch = 0;
int onOffSwitch = 0;

int state4Counter = 0;

void setup() {
    Serial.print("Setting up\n");
    pinMode(stateSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(onOffSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(bulbPin, OUTPUT);               // Set bulb pin as output
    bulbState = EEPROM.read(EEPROM_ADDR);   // Recover latest state from memory
    setBulbState(bulbState);                // Ensure bulb starts on last recorded state
    Serial.begin(9600);
}

void loop() {
    stateSwitch = digitalRead(stateSwitchPin); // Read the switch state
    onOffSwitch = digitalRead(onOffSwitchPin); // Read the switch state
    if (stateSwitch == LOW) {
      switchBulbState(stateSwitch);
    }
    if (onOffSwitch == LOW) {
      switchOnOffState(onOffSwitch);
    }
    if (bulbState == 1){
      stateBLINK();
    }
    if (bulbState == 2){
      stateDIM();
    }
    if (bulbState == 3){
      stateHORROR();
    }
    if (bulbState == 4){
      stateCANDLE();
    }
}

void setBulbState(int bulbState){
  if (bulbState == 0){
    stateON();
  }
  if (bulbState == 1){
    stateBLINK();
  }
  if (bulbState == 2){
    stateDIM();
  }
  if (bulbState == 3){
    stateHORROR();
  }
  if (bulbState == 4){
    stateCANDLE();
  }
}

void switchBulbState(int stateSwitch){
  if (stateSwitch == LOW) {
    bulbState = bulbState + 1;
    if (bulbState > numStates){
      bulbState = 0;
    }
    if (EEPROM.read(EEPROM_ADDR) != bulbState) {
        EEPROM.write(EEPROM_ADDR, bulbState);
    }
    if (bulbState == 0){
      stateON();
    }
    delay(1000);
  }
}

void switchOnOffState(int onOffSwitch){
  if (onOffSwitch == LOW) {
    if (onOffState == 0){
      onOffState = 1;
    }else{
      onOffState = 0;
    }
    if (bulbState == 0){
      analogWrite(bulbPin, 255*onOffState);
    }
  }
  delay(1000);
}

void stateON(){ // State 0: Keep LED on
  setIntensityLED(255);
}

void stateDIM(){ // State 1: Dim LED on and off
  dimLED(0,255);
  dimLED(254,1);
}

void stateBLINK(){ // State 2: Suppose to work on/off LED with 1/2 a second intervals
  setIntensityLED(255);
  delay(500);
  setIntensityLED(0);
  delay(500);
}

void stateHORROR(){ // State 3: Simulate the LED to light like in a horror movie 
  state4Counter = state4Counter + 1;
  int waitCounter = random(100, 1000);
  if (state4Counter <= waitCounter){
    int intensity = random(0, 255);
    setIntensityLED(intensity);
    int waitTime1 = random(10, 600);
    delay(waitTime1);
  }else{
    setIntensityLED(0); 
    int waitTime2 = random(2000, 10000);
    delay(waitTime2);
    dimLED(0,255);
    state4Counter = 0;
  }
}

void stateCANDLE(){ // State 4: Simulate the LED to light like a candle
  int intensity = random(0, 255);
  int waitTime = random(10, 500);
  dimLED(bulbPower,intensity);
  delay(waitTime);
}

void setIntensityLED(int intensity){
  bulbPower = intensity;
  analogWrite(bulbPin, intensity*onOffState);
}

void dimLED(int start, int end){
  if (start > end){
    for(int i = start; i >= end; i--){
      setIntensityLED(i);
      delay(10);
    }
  }else{
    for(int i = start; i <= end; i++){
      setIntensityLED(i);
      delay(10);
    }
  }
}

// #include <EEPROM.h>
// #define EEPROM_ADDR 0 // EEPROM address to store the state

// const int onOffSwitchPin = 2;   // Switch input pin
// const int stateSwitchPin = 3;   // Switch input pin
// const int bulbPin = 9;          // Output pin for the bulb (or relay)
// int bulbPower = 0;

// int bulbStateSwitch = 0;
// int onOffStateSwitch = 0;

// int horrorCounter = 0;

// enum State {
//   LED_ON,
//   BLINK,
//   DIM,
//   HORROR,
//   CANDLE
// };

// enum Power {
//   OFF,
//   ON
// };

// Power onOffState = ON;
// State bulbState = LED_ON;

// void setup() {
//     Serial.print("Setting up\n");
//     pinMode(stateSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
//     pinMode(onOffSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
//     pinMode(bulbPin, OUTPUT);               // Set bulb pin as output
//     // bulbState = EEPROM.read(EEPROM_ADDR);   // Recover latest state from memory
//     bulbState = LED_ON;
//     setBulbState(bulbState);                // Ensure bulb starts on last recorded state
//     Serial.begin(9600);
// }

// void loop() {
//     bulbStateSwitch = digitalRead(stateSwitchPin); // Read the switch state
//     onOffStateSwitch = digitalRead(onOffSwitchPin); // Read the switch state
//     if (bulbStateSwitch == LOW) {
//       switchBulbState();
//     }
//     if (onOffStateSwitch == LOW) {
//       switchOnOffState();
//     }
//     setBulbState(bulbState);
// }

// void setBulbState(int bulbState){
//   switch(bulbState){
//     case LED_ON:
//       onState();
//     case BLINK:
//       blinkState();
//     case DIM:
//       dimState();
//     case HORROR:
//       horrorState();
//     case CANDLE:
//       candleState();
//   }
// }

// void switchBulbState(){
//   switch(bulbState){
//     case LED_ON:
//       bulbState = BLINK;
//     case BLINK:
//       bulbState = DIM;
//     case DIM:
//       bulbState = HORROR;
//     case HORROR:
//       bulbState = CANDLE;
//     case CANDLE:
//       bulbState = LED_ON;
//   }
//   delay(1000);
//   // EEPROM.write(EEPROM_ADDR, bulbState);
//   Serial.print(bulbState);
// }

// void switchOnOffState(){
//   switch(onOffState){
//     case ON:
//       onOffState = OFF;
//       analogWrite(bulbPin, 255*0);
//     case OFF:
//       onOffState = ON;
//   }
//   delay(1000);
// }

// void onState(){ // Keep LED on
//   setIntensityLED(255);
// }

// void blinkState(){ // Blink LED
//   setIntensityLED(255);
//   delay(500);
//   setIntensityLED(0);
//   delay(500);
// }

// void dimState(){ // Dim LED on and off
//   dimLED(0,255);
//   dimLED(254,1);
// }

// void horrorState(){ // Simulate the LED to light like in a horror movie 
//   horrorCounter = horrorCounter + 1;
//   if (horrorCounter <= 500){
//     int intensity = random(0, 255);
//     int waitTime = random(10, 777);
//     setIntensityLED(intensity);
//     delay(waitTime);
//   }else{
//     setIntensityLED(0); 
//     delay(5000);
//     dimLED(0,255);
//     horrorCounter = 0;
//   }
// }

// void candleState(){ // Simulate the LED to light like a candle
//   int intensity = random(0, 255);
//   int waitTime = random(10, 500);
//   dimLED(bulbPower,intensity);
//   delay(waitTime);
// }

// void setIntensityLED(int intensity){
//   bulbPower = intensity;
//   switch (onOffState){
//     case ON:
//       analogWrite(bulbPin, intensity);
//     case OFF:
//       analogWrite(bulbPin, 0);
//   }
// }

// void dimLED(int start, int end){
//   if (start > end){
//     for(int i = start; i >= end; i--){
//       setIntensityLED(i);
//       delay(10);
//     }
//   }else{
//     for(int i = start; i <= end; i++){
//       setIntensityLED(i);
//       delay(10);
//     }
//   }
// }