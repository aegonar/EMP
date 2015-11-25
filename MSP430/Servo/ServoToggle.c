#include <Servo.h> 
 
Servo myservo;   
int pos = 0;   
const int  buttonPin = PUSH2;    // the pin that the pushbutton is attached to
const int ledPin = RED_LED;       // the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;  
 
void setup() 
{ 
  myservo.attach(9);

  pinMode(buttonPin, INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);
  
} 
 
void loop() 
{ 

  buttonState = digitalRead(buttonPin);

    if (buttonState == HIGH) {
      myservo.write(0);              // tell servo to go to position in variable 'pos' 
      delay(500); 
    } 
    else {
       myservo.write(90);              // tell servo to go to position in variable 'pos' 
      delay(500); 
    }

} 