#include <Adafruit_LiquidCrystal.h>

// =====================================================
// LCD
// Tinkercad 16x2 I2C LCD (MCP23008)
// Address shown in Tinkercad: 0x20
// =====================================================
Adafruit_LiquidCrystal lcd(0);


// =====================================================
// BINARY SWITCHES
//
// Switch 8 -> D2
// Switch 4 -> D3
// Switch 2 -> D4
// Switch 1 -> D5
//
// The switches use INPUT_PULLUP:
// Switch OFF = HIGH
// Switch ON  = LOW
// =====================================================
const int switch8 = 2;
const int switch4 = 3;
const int switch2 = 4;
const int switch1 = 5;


// =====================================================
// CHECK BUTTON
// =====================================================
const int checkButton = 6;


// =====================================================
// LEDS
//
// Green LED -> D7
// Red LED   -> D8
// =====================================================
const int greenLED = 7;
const int redLED = 8;


// =====================================================
// GAME VARIABLES
// =====================================================
int targetNumber = 0;
int attempts = 0;
int correctAnswers = 0;


// =====================================================
// SETUP
// Runs once when Arduino starts
// =====================================================
void setup() {

  // -----------------------------
  // Binary switches
  // -----------------------------
  pinMode(switch8, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch1, INPUT_PULLUP);


  // -----------------------------
  // Check button
  // -----------------------------
  pinMode(checkButton, INPUT_PULLUP);


  // -----------------------------
  // LEDs
  // -----------------------------
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);


  // Make sure LEDs start OFF
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);


  // -----------------------------
  // Start LCD
  // -----------------------------
  lcd.begin(16, 2);

  // Turn LCD backlight ON
  lcd.setBacklight(1);


  // -----------------------------
  // Random number generator
  // -----------------------------
  randomSeed(analogRead(A1));


  // -----------------------------
  // Start first question
  // -----------------------------
  startNewQuestion();
}


// =====================================================
// MAIN LOOP
// =====================================================
void loop() {

  // Check whether CHECK button is pressed
  if (digitalRead(checkButton) == LOW) {

    // Small debounce delay
    delay(50);


    // -----------------------------------------------
    // Read the binary switches
    // -----------------------------------------------
    int enteredNumber = readBinaryNumber();


    // -----------------------------------------------
    // Count this as one attempt
    // -----------------------------------------------
    attempts++;


    // -----------------------------------------------
    // Compare entered number with target number
    // -----------------------------------------------
    if (enteredNumber == targetNumber) {

      // Correct answer
      correctAnswers++;


      // Green LED ON
      digitalWrite(greenLED, HIGH);

      // Red LED OFF
      digitalWrite(redLED, LOW);


      // Display result
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("CORRECT!");

      lcd.setCursor(0, 1);
      lcd.print("Binary: ");
      lcd.print(enteredNumber);
    }


    else {

      // Incorrect answer

      // Green LED OFF
      digitalWrite(greenLED, LOW);

      // Red LED ON
      digitalWrite(redLED, HIGH);


      // Display result
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("WRONG!");

      lcd.setCursor(0, 1);
      lcd.print("Ans: ");
      lcd.print(targetNumber);
    }


    // -----------------------------------------------
    // Calculate success rate
    // -----------------------------------------------
    int successRate =
      (correctAnswers * 100) / attempts;


    // Keep result visible
    delay(1500);


    // -----------------------------------------------
    // Check whether 10 attempts are completed
    // -----------------------------------------------
    if (attempts >= 10) {

      // Turn LEDs OFF
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, LOW);


      // Display final score
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("10 Attempts");

      lcd.setCursor(0, 1);
      lcd.print("Success: ");
      lcd.print(successRate);
      lcd.print("%");


      // Stop the game
      while (true) {
        // Game finished
      }
    }


    // -----------------------------------------------
    // Turn LEDs OFF before next question
    // -----------------------------------------------
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);


    // -----------------------------------------------
    // Generate next question
    // -----------------------------------------------
    startNewQuestion();


    // -----------------------------------------------
    // Wait until CHECK button is released
    // -----------------------------------------------
    while (digitalRead(checkButton) == LOW) {
      delay(10);
    }
  }
}


// =====================================================
// GENERATE A NEW QUESTION
// =====================================================
void startNewQuestion() {

  // Generate random number from 0 to 15
  //
  // random(0,16) gives:
  // 0,1,2,3,...14,15
  //
  // Four binary switches can represent 0-15.
  targetNumber = random(0, 16);


  // Clear LCD
  lcd.clear();


  // Display target number
  lcd.setCursor(0, 0);
  lcd.print("Number: ");
  lcd.print(targetNumber);


  // Instruction
  lcd.setCursor(0, 1);
  lcd.print("Set Binary");
}


// =====================================================
// READ FOUR BINARY SWITCHES
// =====================================================
int readBinaryNumber() {

  int number = 0;


  // -----------------------------------------------
  // 8's place
  // -----------------------------------------------
  if (digitalRead(switch8) == LOW) {
    number += 8;
  }


  // -----------------------------------------------
  // 4's place
  // -----------------------------------------------
  if (digitalRead(switch4) == LOW) {
    number += 4;
  }


  // -----------------------------------------------
  // 2's place
  // -----------------------------------------------
  if (digitalRead(switch2) == LOW) {
    number += 2;
  }


  // -----------------------------------------------
  // 1's place
  // -----------------------------------------------
  if (digitalRead(switch1) == LOW) {
    number += 1;
  }


  // Return decimal number
  return number;
}
