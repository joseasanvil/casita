#include <EEPROM.h>
#define EEPROM_ADDR 0 // EEPROM address to store the state

const int onOffSwitchPin = 2;   // Switch input pin
const int stateSwitchPin = 3;   // Switch input pin
const int bulbPin = 9;          // Output pin for the bulb (or relay)

int onOffState = 1;             // Variable to store switch state
int bulbState = 0;              // Bulb state
int numStates = 4;              // Number of states

int stateSwitch = 0;
int onOffSwitch = 0;

void setup() {
    Serial.print("Setting up\n");
    pinMode(stateSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(onOffSwitchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(bulbPin, OUTPUT);               // Set bulb pin as output
    bulbState = EEPROM.read(EEPROM_ADDR);
    setBulbState(bulbState);           // Ensure bulb starts OFF
    Serial.begin(9600);
}

void setIntensityLED(int intensity){
  analogWrite(bulbPin, intensity*onOffState);
}

void increaseLED() {
  for(int i = 0; i <= 255; i++){
    setIntensityLED(i);
    delay(10);
  }
}

void decreaseLED(){
  for(int i = 255; i >= 0; i--){
    setIntensityLED(i);
    delay(10);
  }
}

void switchOnOffState(int onOffSwitch){
  if (onOffSwitch == LOW) {
    if (onOffState == 0){
      // Serial.print("Turning ON\n");
      onOffState = 1;
    }else{
      // Serial.print("Turning OFF\n");
      onOffState = 0;
    }
    if (bulbState == 0){
      analogWrite(bulbPin, 255*onOffState);
    }
  }
  delay(1000);
}

void state1(){
  lightOn();
}

void state2(){
  increaseLED();
  decreaseLED();
}

void state3(){
  lightOn();
  delay(500);
  lightOff();
  delay(500);
}

void state4(){
  int intensity = random(0, 255);
  int waitTime = random(10, 1000);
  setIntensityLED(intensity);
  delay(waitTime);
}

void setBulbState(int bulbState){
  if (bulbState == 0){
    state1();
  }
  if (bulbState == 1){
    state2();
  }
  if (bulbState == 2){
    state3();
  }
  if (bulbState == 3){
    state4();
  }
}

void lightOn(){
  analogWrite(bulbPin, 255*onOffState);
}

void lightOff(){
  analogWrite(bulbPin, 0*onOffState);
}

void switchBulbState(int stateSwitch){
  if (stateSwitch == LOW) {
    // Serial.print("Switch triggered\n");
    bulbState = bulbState + 1;
    if (bulbState >= numStates){
      bulbState = 0;
    }
    // Serial.print("Bulb state:");
    // Serial.print(bulbState);
    // Serial.print("\n");
    if (EEPROM.read(EEPROM_ADDR) != bulbState) {
        EEPROM.write(EEPROM_ADDR, bulbState);
    }
    if (bulbState == 0){
      state1();
    }
    delay(1000);
  }
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
      state2();
    }
    if (bulbState == 2){
      state3();
    }
    if (bulbState == 3){
      state4();
    }
}