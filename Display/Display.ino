// this code is used for interacing with the testing team to select the mode
// of operation
#define BUTTON_PIN A0
#define CHEM1_TIME 2000
#define CHEM2_TIME 2500
#define DEBOUNCE_TIME 200
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//the state of the display 0 for the main menu, 1 for manual, 2 for profile mode
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

unsigned check_button(void){//Check for button input
        unsigned reading=analogRead(BUTTON_PIN);
        if(reading>900)
                return 0; //nothing is pressed
        else if(reading>360)
                return 2;
        else if(reading>220)
                return 1;
}

void setup() {
        lcd.begin(16, 2);
        pinMode(BUTTON_PIN, INPUT_PULLUP);
        Serial.begin(9600);
}

void loop() {
        // lcd.setCursor(0, 0);//first row, first column, row is the 2nd number
        // int a=analogRead(A0);
        // lcd.clear();

        // delay(10);//10 is good enough for not noticing tearing
        static unsigned state=0;
        static unsigned button_reading;
        static unsigned long start_stir_time;
        static unsigned long time_stir_left;
        //Serial.println(analogRead(BUTTON_PIN));
        switch(state) {
        case 0:
                //add turning off the fan at this state to make sure everything is clean
                delay(DEBOUNCE_TIME);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("1/Manual");
                lcd.setCursor(0,1);
                lcd.print("2/Profiles");


                while((button_reading=check_button())==0) {
                        ;

                }
                //Serial.println(button_reading);
                switch(button_reading) {
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
                delay(DEBOUNCE_TIME);
                while(!(check_button())) { //press any to return to menu
                        analogWrite(9,random(0,255));//fan control code go here
                        delay(50);
                }
                state=0;
                break;
        case 2:
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Profile Mode:");
                lcd.setCursor(0,1);
                lcd.print("1/Chem1 2/Chem2");
                delay(DEBOUNCE_TIME);
                while((button_reading=check_button())==0) {//wait for input button
                        ;
                }
                delay(DEBOUNCE_TIME);
                switch(button_reading) {
                case 1:
                        lcd.clear();
                        start_stir_time=millis();
                        //button 1 for emergency exit
                        while((time_stir_left=(millis()-start_stir_time))<CHEM1_TIME && check_button()!=1) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("Time left for 1");
                                lcd.setCursor(0,1);
                                lcd.print((CHEM1_TIME-time_stir_left));
                                lcd.setCursor(5,1);
                                lcd.print("Exit:1");
                                delay(10);
                        }
                        state=0;//return to menu when done
                        break;
                case 2:
                        lcd.clear();
                        start_stir_time=millis();
                        while((time_stir_left=(millis()-start_stir_time))<CHEM2_TIME && check_button()!=1) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("Time left for 2");
                                lcd.setCursor(0,1);
                                lcd.print((CHEM2_TIME-time_stir_left));
                                lcd.setCursor(5,1);
                                lcd.print("Exit:1");
                                delay(10);
                        }
                        state=0;
                        break;
                }


                break;
        }
}
