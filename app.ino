#include <LiquidCrystal.h>

// Button / Switch Pins
#define MODE_SWITCH_PIN 13
#define INCREMENT_PIN 9
#define DECREMENT_PIN 8
#define BYPASS_PIN 7

// LCD Pins
#define LCD_BACKLIGHT 10
#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

// Program Constants
#define LCD_ON_TIME_MS 5000

enum LCDStates {
    TURNING_ON,
    DISPLAYING,
    TURNING_OFF,
    DISABLED
};

enum SwitchStates {
    IS_OPEN,
    IS_RISING,
    IS_CLOSED,
    IS_FALLING
};

class SwitchMachine
{
    private:
        byte pinNumber;
        byte pinValue;

    public:
        SwitchStates state;
        SwitchMachine(byte n): pinNumber(n) {}

        void run() {
            pinValue = digitalRead(pinNumber);

            switch (state) {
                case IS_OPEN: {
                    if (pinValue == HIGH) {
                        state = IS_RISING;
                    }
                    break;
                }
                case IS_RISING: {
                    state = IS_CLOSED;
                    break;
                }
                case IS_CLOSED: {
                    if (pinValue == LOW) {
                        state = IS_FALLING;
                    }
                    break;
                }
                case IS_FALLING: {
                    state = IS_OPEN;
                    break;
                }
            }
        }
};

class LCDMachine
{
    private:
        LiquidCrystal * lcd;
        SwitchMachine * modeSwitch;
        unsigned long lcdTurnOnTime;

        void showLCD() {
            //
        }

        void turnOffLCD() {
            //
        }

    public:
        LCDStates state;
        LCDMachine(LiquidCrystal * lcd, SwitchMachine * modeSwitch): lcd(lcd), modeSwitch(modeSwitch) {}

        void run() {
            switch (state) {
                case TURNING_ON: {
                    state = DISPLAYING;
                    lcdTurnOnTime = millis();

                    break;
                }
                case DISPLAYING: {
                    showLCD();

                    if (modeSwitch->state == IS_RISING) {
                        lcdTurnOnTime = millis();
                    }

                    if (millis() > lcdTurnOnTime + LCD_ON_TIME_MS) {
                        state = TURNING_OFF;
                    }

                    break;
                }
                case TURNING_OFF: {
                    turnOffLCD();
                    state = DISABLED;

                    break;
                }
                case DISABLED: {
                    if (modeSwitch->state == IS_RISING) {
                        state = TURNING_ON;
                    }

                    break;
                }
            }
        }
};

LiquidCrystal lcdPanel(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

SwitchMachine modeSwitch(MODE_SWITCH_PIN);
LCDMachine lcd(&lcdPanel, &modeSwitch);

void setup() {
    //
}

void loop() {
    modeSwitch.run();
    lcd.run();
}
