#include <EEPROM.h>
#define EEPROM_ADDR 0 // EEPROM address to store the state

const int onOffSwitchPin = 2;   // Switch input pin
const int stateSwitchPin = 3;   // Switch input pin
const int bulbPin = 9;          // Output pin for the bulb (or relay)

int onOffState = 1;             // Variable to store switch state
int bulbState = 0;              // Bulb state
int numStates = 2;              // Number of states

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

void increaseLED() {
  for(int i = 0; i <= 255; i++){
    analogWrite(bulbPin, i*onOffState);
    delay(10);
  }
}

void decreaseLED(){
  for(int i = 255; i >= 0; i--){
    analogWrite(bulbPin, i*onOffState);
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

void setBulbState(int bulbState){
  if (bulbState == 0){
    analogWrite(bulbPin, 255*onOffState);
  }
  if (bulbState == 1){
    increaseLED();
    decreaseLED();
  }
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
      analogWrite(bulbPin, 255*onOffState);
    }
    delay(1000);
  }

  if (bulbState == 1){
    increaseLED();
    decreaseLED();
  }
}

void loop() {
    stateSwitch = digitalRead(stateSwitchPin); // Read the switch state
    onOffSwitch = digitalRead(onOffSwitchPin); // Read the switch state
    switchBulbState(stateSwitch);
    switchOnOffState(onOffSwitch);
}