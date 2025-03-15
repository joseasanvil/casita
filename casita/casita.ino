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
      state1();
    }
    if (bulbState == 2){
      state2();
    }
    if (bulbState == 3){
      state3();
    }
    if (bulbState == 4){
      state4();
    }
}

void setBulbState(int bulbState){
  if (bulbState == 0){
    state0();
  }
  if (bulbState == 1){
    state1();
  }
  if (bulbState == 2){
    state2();
  }
  if (bulbState == 3){
    state3();
  }
  if (bulbState == 4){
    state4();
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
      state0();
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

void state0(){ // State 0: Keep LED on
  setIntensityLED(255);
}

void state1(){ // State 1: Dim LED on and off
  dimLED(0,255);
  dimLED(254,1);
}

void state2(){ // State 2: Suppose to work on/off LED with 1/2 a second intervals
  setIntensityLED(255);
  delay(500);
  setIntensityLED(0);
  delay(500);
}

void state3(){ // State 3: Simulate the LED to light like in a horror movie 
  state4Counter = state4Counter + 1;
  if (state4Counter <= 500){
    int intensity = random(0, 255);
    int waitTime = random(10, 600);
    setIntensityLED(intensity);
    delay(waitTime);
  }else{
    setIntensityLED(0); 
    delay(5000);
    dimLED(0,255);
    state4Counter = 0;
  }
}

void state4(){ // State 4: Simulate the LED to light like a candle
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