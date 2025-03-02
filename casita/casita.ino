const int switchPin = 3;  // Switch input pin
const int bulbPin = 2;   // Output pin for the bulb (or relay)
int switchState = 1;      // Variable to store switch state
int bulbState = 0;

void setup() {
    Serial.print("Setting up\n");
    pinMode(switchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(bulbPin, OUTPUT);          // Set bulb pin as output
    digitalWrite(bulbPin, LOW);        // Ensure bulb starts OFF
    Serial.begin(9600);
}

void switchBulbState(int switchState){
  if (switchState == LOW) {
    Serial.print("Switch triggered\n");
    if (bulbState == HIGH) {
      Serial.print("Turning bulb off\n");
      digitalWrite(bulbPin, LOW);
      bulbState = 0;
    }else{
      Serial.print("Turning bulb on\n");
      digitalWrite(bulbPin, HIGH);
      bulbState = 1;
    }
    delay(1000);
  }
}

void loop() {
    switchState = digitalRead(switchPin); // Read the switch state
    switchBulbState(switchState);
}