#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte BEEP_PIN = 10;
const byte LED_PIN_RED = 11;
const byte LED_PIN_GREEN = 12;
const byte LED_PIN_BLUE = 13;

const byte KEYPAD_ROWS_COUNT = 4;
const byte KEYPAD_COLS_COUNT = 4;

const byte ROW_PINS[KEYPAD_ROWS_COUNT] = {9, 8, 7, 6}; //connect to the row pinouts of the kpd
const byte COL_PINS[KEYPAD_COLS_COUNT] = {5, 4, 3, 2}; //connect to the column pinouts of the kpd

char KEYPAD_KEYS[KEYPAD_ROWS_COUNT][KEYPAD_COLS_COUNT] = {
        {'1', '2', '3', 'C'},
        {'4', '5', '6', 'X'},
        {'7', '8', '9', '-'},
        {'*', '0', '#', 'O'},
};

String password = "";
String msg = "Enter password";
String validPassword = "12345";

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad kpd = Keypad(makeKeymap(KEYPAD_KEYS), ROW_PINS, COL_PINS, KEYPAD_ROWS_COUNT, KEYPAD_COLS_COUNT);

bool isOpen = false;

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN_GREEN, OUTPUT);
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_BLUE, OUTPUT);
    pinMode(BEEP_PIN, OUTPUT);
    lcd.init();
    lcd.backlight();
    lcd.print(msg);
}

void loop() {

    char key = kpd.getKey();
    if (key) {
        int passwordLength = password.length();
        if (isOpen || key == '-') {
            if (key == '-') {
                isOpen = false;
            }
        } else if (key == 'C') password = "";
        else if (key == 'X') {
            if ( passwordLength) {
                password.remove(passwordLength - 1);
            }
        }
        else if (key == 'O') {
            if (password == validPassword) {
                isOpen = true;
                password = "";
            } else {
                msg = "invalid password";
                password = "";
                lcd.clear();
                lcd.print(msg);
                delay(2000);
            }

        } else {
            if (passwordLength < 15) {
                password.concat(key);
            }
        }

        if (isOpen) msg = "open";
        else msg = "Enter password";

        lcd.clear();
        lcd.print(msg);
        lcd.setCursor(0, 1);
        lcd.print(password);

        digitalWrite(BEEP_PIN, HIGH);
        delay(20);
        digitalWrite(BEEP_PIN, LOW);
    }

    if (isOpen) {
        digitalWrite(LED_PIN_GREEN, HIGH);
        digitalWrite(LED_PIN_BLUE, LOW);
        digitalWrite(LED_PIN_RED, LOW);
    } else {
        digitalWrite(LED_PIN_GREEN, LOW);
        digitalWrite(LED_PIN_BLUE, LOW);
        digitalWrite(LED_PIN_RED, HIGH);
    }

}