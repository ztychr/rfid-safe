//property of cryptosvanen
// release alpha 1.2

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial RFID(2, 3); // RX and TX
Servo myServo0;


int tag1[14] = {XXXXXXXXXX};
int tag2[14] = {XXXXXXXXXX};
int tag3[14] = {XXXXXXXXXX};

int scan[14];
int match = -1;

int buttonState = 0;
const int buttonPin = 4;
const int buzzer = 12;

void setup() {

  myServo0.attach(9);
  myServo0.write(135);
  pinMode(buttonPin, INPUT);
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(7, HIGH);

}


int kort(int check[14]) {
  int pass = 1;
  for (int i = 0; i < 14; i++) {
    if (scan[i] != check[i]) {
      pass = 0;
    }
  }
  return pass;
  RFID.flush();
}
int state = 0;
unsigned long timeout;
void loop() {

  buttonState = digitalRead(buttonPin);
  //  digitalWrite(7, HIGH);
  if (buttonState == LOW) {
    myServo0.write(135);
    match = -1;
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);

  }

  if (RFID.available() > 0)
  {
    delay(15);
    for (int i = 0; i < 14; i++) {
      scan[i] = RFID.read();
      if (kort(tag1) == 1 || kort(tag2) == 1 || kort(tag3) == 1) {
        timeout = millis() + 1000;

        match = 1;
      } else {
        match = 0;
        timeout = millis() + 1000;
      }
    }
  }


  switch (state)
  {
    case 0:
      if (match == 1) {

        state = 1;
        tone(buzzer, 1300);
        delay(70);
        noTone(buzzer);
        delay(50);
        tone(buzzer, 1300);
        delay(70);
        noTone(buzzer);
        delay(700);
        Serial.println("Access granted. Welcome");
        myServo0.write(25);
        digitalWrite(7, LOW);
        digitalWrite(8, HIGH);
        delay(1000);
        // digitalWrite(8,LOW);
        
        // SSL added and removed here ;-)


      }

      if (match == 0) {
        state = 1;
        Serial.println("Access denied.");
        RFID.flush();
        match = -1;
        tone(buzzer, 500);
        delay(400);
        noTone(buzzer);
        myServo0.write(135);
        digitalWrite(8, LOW);
        digitalWrite(7, HIGH);
      }
      break;
    case 1:
      if (millis() > timeout)
      {
        state = 0;
        Serial.println("timeout");

      }
      if (match == 1) {

        timeout = millis() + 100;
      }



      break;


  }

  match = -1;
}


