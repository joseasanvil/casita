#include <EEPROM.h>
#define EEPROM_ADDR 0 // EEPROM address to store the state

const int bulbPin = 9;          // Output pin for the bulb (or relay)
int bulbPower = 0;

enum State {LED_ON, BLINK, DIM, HORROR, CANDLE};
State bulbState = LED_ON;              // Bulb state

enum Power {OFF, ON};
Power onOffState = ON;             // Variable to store switch state

const int onOffSwitchPin = 2;   // Switch input pin
int stateSwitch = 0;

const int stateSwitchPin = 3;   // Switch input pin
int onOffSwitch = 0;

int horrorCounter = 0;

void setup() {
    Serial.print("Setting up\n");
    pinMode(stateSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(onOffSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(bulbPin, OUTPUT);               // Set bulb pin as output
    bulbState = EEPROM.read(EEPROM_ADDR);   // Recover latest state from memory
    setBulbState();                // Ensure bulb starts on last recorded state
    Serial.begin(9600);
}

void loop() {
    stateSwitch = digitalRead(stateSwitchPin); // Read the switch state
    onOffSwitch = digitalRead(onOffSwitchPin); // Read the switch state
    if (stateSwitch == LOW) {
      switchBulbState();
    }
    if (onOffSwitch == LOW) {
      switchOnOffState();
    }
    setBulbState();
}

void setBulbState(){
  switch(bulbState){
    case LED_ON:
      stateON();
      break;
    case BLINK:
      stateBLINK();
      break;
    case DIM:
      stateDIM();
      break;
    case HORROR:
      stateHORROR();
      break;
    case CANDLE:
      stateCANDLE();
      break;
    default:
      stateON();
      break;
  }
}

void switchBulbState(){
  switch(bulbState){
    case LED_ON:
      bulbState = BLINK;
      break;
    case BLINK:
      bulbState = DIM;
      break;
    case DIM:
      bulbState = HORROR;
      break;
    case HORROR:
      bulbState = CANDLE;
      break;
    case CANDLE:
      bulbState = LED_ON;
      break;
    default:
      bulbState = LED_ON;
      break;
  }
  if (EEPROM.read(EEPROM_ADDR) != bulbState) {
      EEPROM.write(EEPROM_ADDR, bulbState);
  }
  delay(1000);
}

void switchOnOffState(){
  switch(onOffState){
    case OFF:
      onOffState = ON;
      break;
    case ON:
      onOffState = OFF;
      break;
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
  horrorCounter = horrorCounter + 1;
  int waitCounter = random(100, 1000);
  if (horrorCounter <= waitCounter){
    int intensity = random(0, 255);
    setIntensityLED(intensity);
    int waitTime1 = random(10, 600);
    delay(waitTime1);
  }else{
    setIntensityLED(0); 
    int waitTime2 = random(2000, 10000);
    delay(waitTime2);
    dimLED(0,255);
    horrorCounter = 0;
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
