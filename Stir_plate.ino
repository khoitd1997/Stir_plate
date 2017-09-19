//Use pin 10 and timer 1, which will not affect millis and delay
#define SPEED_CONTROL_PIN A5//Pins that read the value of the potentiometer
#define PWM_FAN_PIN 10//fans that feed the PWM value to the fans
#define FAN_ON_OFF_PIN 4//turn on or off the transistor that controls the fan
void setup()
{
        TCCR1A = 0x23;//configure the TCCR1A register on mode of operation, TOP
        TCCR1B = 0x19; //configure the TCCR1B register on mode of operation, TOP
        OCR1A = 639; // limit by 16 bits, determine the frequency of the output
        OCR1B = 70; // the ratio of OCR1B/OCR1A will set duty cycle
        DDRD=0x10;// enable the PWM output (you now have a PWM signal on digital pin 10)
        DDRB=0x04;//enable the digital output on pin 4
        PORTD=0x10;//set pin 4 to HIGH
        Serial.begin(9600);
}
void loop() {

        int PWMValue=map(constrain(analogRead(SPEED_CONTROL_PIN),0,932),0, 932, 50, 400);
        Serial.print(analogRead(SPEED_CONTROL_PIN));
        Serial.print("      ");
        Serial.println(PWMValue);
        if(PWMValue>75)
        {
                PORTD=0x10;
                OCR1B=PWMValue;
        }
        else PORTD=0x00;  //set pin 4 to LOW

}
int Read_Potentiometer()
{
        return 0;
}
