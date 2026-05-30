#include "pitches.h" // include a library of music notes that can be used with the speaker

// constants that won't change, they're used here to set pin numbers
const int buttonPinYellow = 2;  // the number of the yellow pushbutton pin
const int buttonPinRed = 3;     // the number of the red pushbutton pin
const int ledPin = 13;          // the number of the LED pin manufactured with the arduino
const int low = 0;              // assign the normal button state (unpressed) as 0
const int high = 1;             // assign the pushed button state (pressed) as 1
int motorPin = 4;               // vibration motor is connected to pin 4

// notes in the melody
int count[] = {NOTE_E4};                      // sound of a count
int warning[] = {NOTE_G4, NOTE_G4, NOTE_G4};  // warning sound
int reset[] = {NOTE_E4, NOTE_G4, NOTE_B4};    // reset sound
int full[] = {NOTE_C4, NOTE_E4, NOTE_G4};     // sound representing a full/almost full canister
int half[] = {NOTE_C4, NOTE_E4};              // sound representing a halfway full canister
int low[] = {NOTE_C4};                        // sound representing a low canister

// note durations: 4 = quarter note, 8 = eighth note, etc.
int noteDurations1[] = {2};       // play a half note
int noteDurations2[] = {4, 4, 4}; // play 3 quarter notes
int noteDurations3[] = {8, 8, 8}; // play 3 eighth notes
int noteDurations4[] = {4, 4, 4}; // play 3 quarter notes
int noteDurations5[] = {4, 4};    // play 2 quarter notes
int noteDurations6[] = {4};       // play a quarter note

// variables
int buttonStateYellow = low;  // variable for reading the yellow pushbutton status, starts off as "low"
int buttonStateRed = low;     // variable for reading the red pushbutton status, starts off as "low"
int lastButtonState = LOW;    // the previous reading from the input pin
int buttonRedTime = 0;        // the red button will be tracked on the amount of time pressed
int count = 10;               // start the program with 10 counts
int setCount = 10;            // a count that does not decrement with each press

/* The following variables are unsigned longs because the time, measured in
milliseconds, will quickly become a bigger number than what can be stored in an int. */
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long myTime;                // time since the program has started

// button hold code variables
unsigned long minButtonLongPressDuration = 2000;    // the time we wait before we classify the press as a long press
unsigned long buttonLongPressMillis;                // the time in ms since the button was pressed
bool buttonStateLongPress = false;                  // initialized as false, will be true if there is a long press
const int intervalButton = 50;                      // the time between two readings of the button state
unsigned long previousButtonMillis;                 // the timestamp of the latest reading
unsigned long buttonPressDuration;                  // the time the button is pressed in ms

void setup() {
  Serial.begin(9600);                 // initialize the serial monitor
  pinMode(ledPin, OUTPUT);            // initialize the LED pin as an output
  pinMode(buttonPinYellow, INPUT);    // initialize the yellow pushbutton pin as an input
  
  /* pinMode(motorPin, OUTPUT); would be used to initialize the vibration motor, however, we don't 
  because this will make the motor ON which we don't want right now */
}

// the button functions, put in through a loop
void loop() {
  // read the state of each button:
  int buttonStateYellow = digitalRead(buttonPinYellow);
  int buttonStateRedRead = digitalRead(buttonPinRed);

  myTime = millis(); // keep track of time

   // *** YELLOW/READ BUTTON ***
   // check if the yellow pushbutton is pressed. If it is, the buttonState is HIGH
  if (buttonStateYellow == high) 
  {
    digitalWrite(ledPin, high); // turn the LED on

    /* The following is a WIP implementation of a more detailed analysis of the canister, to be worked on after the term:
    Serial.println("MEDICATION INFORMATION -------------------");
    Serial.print("DOSES ORIGINALLY IN CANISTER: ");
    Serial.println(setCount);
    Serial.print("CURRENT COUNT: ");
    Serial.println(count);
    Serial.print("DOSES USED: ");
    Serial.println(setCount - count);
    delay(1000); */

    if (count <= setCount && count > (setCount/2)) {  /* when the READ button is pressed and the canister is more than halfway and less/equal 
    to full, beep and vibrate three times */

      for (int thisNote = 0; thisNote < 3; thisNote++) { // the speaker will play three notes
        // to calculate the note duration, take one second divided by the note type (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
        int noteDuration = 1000 / noteDurations1[thisNote];
        tone(8, full[thisNote], noteDuration);
        // to distinguish the notes, set a minimum time between them, the note's duration + 30% seems to work well
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes); // stop the tone playing
        noTone(8); // stops the generation of a square wave triggered by tone()
      }

      for (int i = 0; i < 3; i++) {   // the vibration motor will buzz three times using a for loop
        pinMode(motorPin, OUTPUT);    // initialize the vibration motor (which turns it ON)
        digitalWrite(motorPin, HIGH); // register the vibration motor in the HIGH state (ON state)
        delay(200);                   // Vibrate for 200ms
        digitalWrite(motorPin, LOW);  // Turn off the motor
        delay(500);                   // wait 500ms before the next vibrate
        pinMode(motorPin, INPUT);     // de-initialize the vibration motor
      }

      Serial.println("THE CANISTER IS FULL OR ALMOST FULL!");
      
    } 

    else if (count <= (setCount/2) && count > (setCount/4)) { /* when the READ button is pressed and the canister is more than quarter-way and less/equal 
    to halfway, beep and vibrate two times */
      
      for (int thisNote = 0; thisNote < 2; thisNote++) { // the speaker will play two notes
        // to calculate the note duration, take one second divided by the note type (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
        int noteDuration = 1000 / noteDurations1[thisNote];
        tone(8, half[thisNote], noteDuration);
        // to distinguish the notes, set a minimum time between them, the note's duration + 30% seems to work well
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes); // stop the tone playing
        noTone(8); // stops the generation of a square wave triggered by tone()
      }

      for (int i = 0; i < 2; i++) {   // the vibration motor will buzz two times using a for loop
        pinMode(motorPin, OUTPUT);    // initialize the vibration motor (which turns it ON)
        digitalWrite(motorPin, HIGH); // register the vibration motor in the HIGH state (ON state)
        delay(200);                   // Vibrate for 200ms
        digitalWrite(motorPin, LOW);  // Turn off the motor
        delay(500);                   // wait 500ms before the next vibrate
        pinMode(motorPin, INPUT);     // de-initialize the vibration motor
      }

      Serial.println("THE CANISTER IS HALFWAY FULL!");
      
    }
    
    else { /* when the READ button is pressed and the canister is less than quarter-way, beep and vibrate once */
      
      for (int thisNote = 0; thisNote < 1; thisNote++) { // the speaker will play a note
        // to calculate the note duration, take one second divided by the note type (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
        int noteDuration = 1000 / noteDurations1[thisNote];
        tone(8, low[thisNote], noteDuration);
        // to distinguish the notes, set a minimum time between them, the note's duration + 30% seems to work well
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing
        noTone(8); // stops the generation of a square wave triggered by tone()
      }
      
      pinMode(motorPin, OUTPUT);    // initialize the vibration motor (which turns it ON)
      digitalWrite(motorPin, HIGH); // register the vibration motor in the HIGH state (ON state)
      delay(200);                   // Vibrate for 200ms
      digitalWrite(motorPin, LOW);  // Turn off the motor
      delay(500);                   // wait 500ms before the next vibrate
      pinMode(motorPin, INPUT);     // de-initialize the vibration motor
      
      Serial.println("THE CANISTER IS LOW!");  
            
    } 
  }

  // *** RED/COUNT BUTTON ***
  // check if the red pushbutton is pressed. If it is, the buttonState is HIGH:
  // check to see if you just pressed the button (i.e. the input went from LOW to HIGH), and you've waited long enough since the last press to ignore any noise:    
  if(myTime - previousButtonMillis > intervalButton) {
  
  int buttonState = digitalRead(buttonPinRed); // Read the digital value of the button (LOW/HIGH)
   
  if (buttonState == HIGH && buttonStateRed == LOW && !buttonStateLongPress) { /* If the button has been pushed AND if the button wasn't pressed before AND if there was not
    already a measurement running to determine how long the button has been pressed: */
    buttonLongPressMillis = myTime;
    buttonStateRed = HIGH;
    Serial.println("Button pressed");
  }
  
  // calculate how long the button has been pressed
  buttonPressDuration = myTime - buttonLongPressMillis;
  // If the button is pressed AND if there is no measurement running to determine how long the button is pressed AND if the time the button has been pressed is larger or 
  // equal to the time needed for a long press
  if (buttonState == HIGH && !buttonStateLongPress && buttonPressDuration >= minButtonLongPressDuration) {
    
    buttonStateLongPress = true; // it is true now that the button has been pressed long
    Serial.println("Button long pressed");
    count = 0; // let count = 0 so in case the user resets the count to the wrong number, they can easily try again
    int i = 0; // set up a counter

    while (i < 50) { // max out i at 50 for a maximum of 500 doses

      count = count + 10; // increase the count by increments of 10
      Serial.print("COUNT: ");
      Serial.println(count); // display the count on the serial monitor
      delay(0);

      
      for (int thisNote = 0; thisNote < 3; thisNote++) { // the speaker will play three notes
        // to calculate the note duration, take one second divided by the note type (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
        int noteDuration = 500 / noteDurations1[thisNote];
        tone(8, reset[thisNote], noteDuration);
        // to distinguish the notes, set a minimum time between them, the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 0.8;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(8); // stops the generation of a square wave triggered by tone()
      }
      
      pinMode(motorPin, OUTPUT);    // initialize the vibration motor (which turns it ON)
      digitalWrite(motorPin, HIGH); // register the vibration motor in the HIGH state (ON state)
      delay(0);                     // wait 0 seconds before the next vibrate (there is only only vibration)
      digitalWrite(motorPin, LOW);  // Turn off the motor
      delay(500);                   // vibrate for 500ms
      pinMode(motorPin, INPUT);     // de-initialize the vibration motor
      
      delay(1000);
      
      buttonState = digitalRead(buttonPinRed); // read the button state at this point
        /* since this is a while loop, this would go on forever until i < 50.
        this is not desired, so if the button state is low (unpressed) anytime while this loop goes on, 
        the loop should break */
        if (buttonState == LOW) {  
          setCount = count; // let the new storage of the canister be equal to whatever the user set it to
          break; // break the loop
        }
      i++; // increment the loop count by one
    } 
  }

  // If the button is released AND if the button was pressed before
  if (buttonState == LOW && buttonStateRed == HIGH) {

    buttonStateRed = LOW; // register the state of the COUNT button as low
    buttonStateLongPress = false; // whether the button was long pressed or not before release, let it be false now
    Serial.println("Button released");
    
    // If there is no measurement running to determine how long the button was pressed AND if the time the button has been pressed is smaller than the minimal time needed 
    // for a long press
    if (buttonPressDuration < minButtonLongPressDuration) {

      Serial.println("Button pressed shortly");
      digitalWrite(ledPin, high); // the LED pin should light up
      count --; // everytime a button press is registered as a short press, decrement the count by one
      Serial.print("COUNT: ");
      Serial.println(count); // display the count on the serial monitor
      
      for (int thisNote = 0; thisNote < 1; thisNote++) { // the speaker will play a note
        // to calculate the note duration, take one second divided by the note type (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
        int noteDuration = 1000 / noteDurations1[thisNote];
        tone(8, count[thisNote], noteDuration);
        // to distinguish the notes, set a minimum time between them, the note's duration + 30% seems to work well
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing
        noTone(8); // stops the generation of a square wave triggered by tone()
      }
      
      if (count > (setCount/5)) {     // when the count is not at a critical point, the vibration motor should buzz only once
        pinMode(motorPin, OUTPUT);    // initialize the vibration motor (which turns it ON)
        digitalWrite(motorPin, HIGH); // register the vibration motor in the HIGH state (ON state)
        delay(0);                     // wait 0 seconds before the next vibrate (there is only only vibration)
        digitalWrite(motorPin, LOW);  // Turn off the motor
        delay(200);                   // vibrate for 200ms
        pinMode(motorPin, INPUT);     // de-initialize the vibration motor
      }

      if(count <= (setCount/5)) { // when the count is at a critical point, the attachment should beep and buzz three times as a warning
        
        for (int thisNote = 0; thisNote < 3; thisNote++) { // the speaker will play three notes
          // to calculate the note duration, take one second divided by the note type (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
          int noteDuration = 1000 / noteDurations2[thisNote];
          tone(8, warning[thisNote], noteDuration);
          // to distinguish the notes, set a minimum time between them, the note's duration + 30% seems to work well
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          // stop the tone playing
          noTone(8); // stops the generation of a square wave triggered by tone()
        } 

        for (int i = 0; i < 3; i++) {       // the vibration motor will buzz three times using a for loop
          pinMode(motorPin, OUTPUT);        // initialize the vibration motor (which turns it ON)
          digitalWrite(motorPin, HIGH);     // register the vibration motor in the HIGH state (ON state)
          delay(50);                        // wait 0 seconds before the next vibrate (there is only only vibration)
          digitalWrite(motorPin, LOW);      // Turn off the motor
          delay(250);                       // vibrate for 250ms
          pinMode(motorPin, INPUT);         // de-initialize the vibration motor
        }

        Serial.println(" *WARNING* *PLEASE CHANGE* "); 

      }
    }
  }
    // store the current timestamp in previousButtonMillis
    previousButtonMillis = myTime;
  }
} 
