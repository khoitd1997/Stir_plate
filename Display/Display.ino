// this code is used for interacing with the testing team to select the mode
// of operation
#define BUTTON_PIN A0
#define CHEM1_TIME 2000
#define CHEM2_TIME 2500
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int state=0;//the state of the display 0 for the main menu, 1 for manual, 2 for profile mode
//we can afford to use pin 7,8,13
//use analog pin for voltage division stuffs
//One button for emergency shutdown or select profile, one button for navigation
//Use internal pull-up to not have to use the 5V source

//Use 5 10K resistors in series for voltage divion circuit
//no button will be>990
//>220 for 1st button
//2nd  >360
// 3rd >450
// 4th >520
// 5th >570

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
        lcd.begin(16, 2);
        pinMode(A0, INPUT_PULLUP);
        Serial.begin(9600);
}

void loop() {
        // lcd.setCursor(0, 0);//first row, first column, row is the 2nd number
        // int a=analogRead(A0);
        // lcd.clear();

        // delay(10);//10 is good enough for not noticing tearing

        unsigned button_reading;
        unsigned long start_stir_time;
        unsigned long time_stir_left;
        switch(state) {
        case 0:
                lcd.clear();
                lcd.print("Menu Mode:")
                lcd.setCursor(0,1);
                lcd.print("1/Manual 2/Profiles");
                switch(check_button) {
                case 1:
                        state=1;//1 for manual mode
                        break;
                case 2:
                        state=2;//2 for profile mode
                        break;
                }
                break;
        case 1:
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Manual Mode");
                while((button_reading=check_button)!=1) { //press 1 to return to menu
                        analogWrite(9,random(0,255));//fan control code go here
                        delay(200);
                }
                state=0;
                break;
        case 2:
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Profile Mode:")
                lcd.setCursor(0,1);
                lcd.print("Chem 1 Chem 2");
                while((button_reading=check_button)) {
                        switch(button_reading) {
                        case 1:
                                lcd.clear()
                                start_stir_time=millis();
                                while((time_stir_left=(millis()-start_stir_time))<CHEM1_TIME) {
                                        lcd.clear();
                                        lcd.setCursor(0,0);
                                        lcd.print("Time left for 1");
                                        lcd.setCursor(0,1);
                                        lcd.print(time_stir_left);
                                        delay(10);
                                }
                                state=0;
                                break;
                        case 2:
                                lcd.clear()
                                start_stir_time=millis();
                                while((time_stir_left=(millis()-start_stir_time))<CHEM2_TIME) {
                                        lcd.clear();
                                        lcd.setCursor(0,0);
                                        lcd.print("Time left for 2");
                                        lcd.setCursor(0,1);
                                        lcd.print(time_stir_left);
                                        delay(10);
                                }
                                state=0;
                                break;
                        }

                }
                break;
        }
}
void check_button(){//wait for input from buttons
        int reading;
        while ((reading=analogRead(BUTTON_PIN))>900) {
                ;
        }

        if(reading>570)
                return 5;
        else if(reading>520)
                return 4;
        else if(reading>450)
                return 3;
        else if(reading>360)
                return 2;
        else if(reading>220)
                return 1;
}
