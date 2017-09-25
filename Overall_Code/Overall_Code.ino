//header for bit manipulation
#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define BIT(x) (0x01 << (x))


//header for display
#define BUTTON_PIN A0
#define CHEM1_TIME 20000
#define CHEM2_TIME 25000
#define DEBOUNCE_TIME 200
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned check_button(void);

//header for the fan
//potentiometer must be at A3, hardcoded into the value of the register
#define PWM_FAN_PIN 10//fans that feed the PWM value to the fans, must be a PWM pin, affect pin 9 too
//Use pin 7 for turning on and off the fan
#define CHEM1_SPEED 200//value between 100 and 400 higher the faster
#define CHEM2_SPEED 300//value between 100 and 400 higher the faster
#define POTENTIOMETER_PIN A3

//header for buzzer
#define BUZZER_PIN 6//PWM pin that controls the buzzer
#define TEMPO 65
#define SONG_LENGTH 50//More beep if bigger number
#define BEATS 2
#define DURATION BEATS*TEMPO//duration of each note played
#define FREQUENCY 523 //pick from 262, 294, 330, 349, 392, 440, 494, 523
#define NUM_ARLARM_TO_PLAY 22
void play_alarm(void);

void setup() {
        bit_clear(PORTD,BIT(7));
        Serial.begin(9600);
        //display
        lcd.begin(16, 2);
        pinMode(BUTTON_PIN, INPUT_PULLUP);

        //fan controls


        //buzzer
        pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
        TCCR1A = 0x23;//configure the TCCR1A register on mode of operation, TOP
        TCCR1B = 0x19; //configure the TCCR1B register on mode of operation, TOP
        OCR1A = 639; // limit by 16 bits, determine the frequency of the output
        OCR1B = 70; // the ratio of OCR1B/OCR1A will set duty cycle
        bit_set(DDRD,BIT(7));//enable the output of pin 7
        bit_set(DDRB, BIT(2));//enable the PWM output on pin 10
        bit_clear(PORTD,BIT(7));//turn off pin 7
        static unsigned state=0;
        static unsigned button_reading;
        static unsigned long start_stir_time;
        static unsigned long time_stir_left;
        bit_clear(PORTD,BIT(7));
        switch(state) {
        case 0:
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
                        int PWMValue=map(constrain(analogRead(POTENTIOMETER_PIN),0,932),0, 932, 50, 400); //map the PWM value into a certain range from the potentiometer value
                        if(PWMValue>75)//threshold that the potentiometer will actually impact the fan speed
                        {
                                bit_set(PORTD,BIT(7));//turn on the plate
                                OCR1B=PWMValue;
                        }
                        else bit_clear(PORTD,BIT(7)); //turn off the plate
                }
                OCR1B=20;
                bit_clear(PORTD,BIT(7));
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
                        bit_set(PORTD,BIT(7));//turn on the plate
                        OCR1B=CHEM1_SPEED;
                        while((time_stir_left=(millis()-start_stir_time))<CHEM1_TIME && check_button()!=1) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("Time left for 1");
                                lcd.setCursor(0,1);
                                lcd.print((CHEM1_TIME-time_stir_left));
                                lcd.setCursor(5,1);
                                lcd.print("Exit:1");
                                delay(50);
                        }
                        OCR1B=20;
                        bit_clear(PORTD,BIT(7));
                        bit_clear(DDRD,BIT(7));
                        play_alarm();
                        state=0;//return to menu when done
                        break;
                case 2:
                        lcd.clear();
                        start_stir_time=millis();
                        bit_set(PORTD,BIT(7));//turn on the plate
                        OCR1B=CHEM2_SPEED;
                        while((time_stir_left=(millis()-start_stir_time))<CHEM2_TIME && check_button()!=1) {
                                lcd.clear();
                                lcd.setCursor(0,0);
                                lcd.print("Time left for 2");
                                lcd.setCursor(0,1);
                                lcd.print((CHEM2_TIME-time_stir_left));
                                lcd.setCursor(5,1);
                                lcd.print("Exit:1");
                                delay(50);
                        }
                        OCR1B=20;
                        bit_clear(PORTD,BIT(7));
                        bit_clear(DDRD,BIT(7));
                        play_alarm();
                        state=0;
                        break;
                }


                break;
        }
}

unsigned check_button(void){//Check for button input
        unsigned reading=analogRead(BUTTON_PIN);
        if(reading>900)
                return 0; //nothing is pressed
        else if(reading>360)
                return 2;
        else if(reading>220)
                return 1;
}

void play_alarm(void){

        unsigned i=0;//doesn't matter if i overflow
        while(i<NUM_ARLARM_TO_PLAY) {
                if(i++%2!=0) {
                        Serial.println("It played the music");
                        tone(BUZZER_PIN, FREQUENCY, DURATION);
                }
                delay(DURATION+TEMPO/10);
        }
}
