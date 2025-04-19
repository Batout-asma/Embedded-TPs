#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buttons (connected to GND)
const int BTN_G = 18; // Player 1
const int BTN_Y = 19; // Player 2

int scoreG = 0;
int scoreY = 0;

void setup() {
  lcd.begin();
  lcd.backlight();

  pinMode(BTN_G, INPUT_PULLUP); // Use internal pull-up
  pinMode(BTN_Y, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("  Reflex Game!");
  lcd.setCursor(0, 1);
  lcd.print(" First to 5 wins");
  delay(5000);

  randomSeed(analogRead(0)); // Initialize random seed
}

void loop() {
  // Reset scores at the beginning
  scoreG = 0;
  scoreY = 0;

  while (scoreG < 5 && scoreY < 5) {
    // Random delay before "GO!" to make it unpredictable
    int waitTime = random(2000, 5000); // between 2s and 5s

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Get Ready...");
    delay(waitTime);

    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("GO!");

    // Wait for first button press
    int winner = -1;
    while (winner == -1) {
      if (digitalRead(BTN_G) == LOW) {
        winner = 0; // Player 1
      } else if (digitalRead(BTN_Y) == LOW) {
        winner = 1; // Player 2
      }
    }

    // Update score
    if (winner == 0) scoreG++;
    else scoreY++;

    // Display result
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player ");
    lcd.print(winner + 1);
    lcd.print(" scores!");
    lcd.setCursor(0, 1);
    lcd.print("P1: ");
    lcd.print(scoreG);
    lcd.print(" P2: ");
    lcd.print(scoreY);
    delay(2000);
  }

  // Game Over screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("      WOHO!");
  lcd.setCursor(0, 1);
  lcd.print(scoreG == 5 ? "Player 1 Wins!" : "Player 2 Wins!");
  delay(5000);

  // Restart info
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting again!");
  lcd.setCursor(0, 1);
  lcd.print("Credits: BATOUT");
  delay(4000);
}
