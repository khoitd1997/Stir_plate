//Use pin 10 and timer 1, which will not affect millis and delay
#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define BIT(x) (0x01 << (x))

//potentiometer must be at A3, hardcoded into the value of the register
#define PWM_FAN_PIN 10//fans that feed the PWM value to the fans, must be a PWM pin
#define FAN_ON_OFF_PIN 6//turn on or off the transistor that controls the fan



void setup()
{
        TCCR1A = 0x23;//configure the TCCR1A register on mode of operation, TOP
        TCCR1B = 0x19; //configure the TCCR1B register on mode of operation, TOP
        OCR1A = 639; // limit by 16 bits, determine the frequency of the output
        OCR1B = 70; // the ratio of OCR1B/OCR1A will set duty cycle
        DDRD=0x10;// enable the PWM output (you now have a PWM signal on digital pin 10)
        DDRB=0x04;//enable the digital output on pin 4
        PORTD=0x10;//set pin 4 to HIGH
}
void loop() {

        int PWMValue=map(constrain(Read_Potentiometer(),0,932),0, 932, 50, 400);//map the PWM value into a certain range from the potentiometer value
        if(PWMValue>75)//threshold that the potentiometer will actually impact the fan speed
        {
                PORTD=0x10;//turn on the plate
                OCR1B=PWMValue;
        }
        else PORTD=0x00;//turn off the plate


}
int Read_Potentiometer()
{
  //select reference as AVcc with external capacitor at AREF
        bit_set(ADMUX,BIT(6));
        bit_clear(ADMUX, BIT(7));

//select ADC 3 or analogpin 3 on the Arduino
        bit_clear(ADMUX, BIT(3));
        bit_clear(ADMUX, BIT(2));
        bit_set(ADMUX, BIT(1));
        bit_set(ADMUX, BIT(0));

//turn off the auto trigger and then initiate the ADC
        bit_clear(ADCSRA,BIT(5));
        bit_set(ADCSRA,BIT(7));
        bit_set(ADCSRA,BIT(6));//stay high as long as conversion in progress


        while(ADSC==0b1) {//check if conversion is done
        }

        return ADC;
}
