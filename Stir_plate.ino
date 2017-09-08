//Use pin 10 and timer 1, which will not affect millis and delay
void setup()
{
        TCCR1A = 0x23;//configure the TCCR1A register on mode of operation, TOP
        TCCR1B = 0x19; //configure the TCCR1B register on mode of operation, TOP
        OCR1A = 639; // limit by 16 bits, determine the frequency of the output
        pinMode(10, OUTPUT); // enable the PWM output (you now have a PWM signal on digital pin 10)
        OCR1B = 300; // the ratio of OCR1B/OCR1A will set duty cycle
        Serial.begin(9600);
}

void loop() {
        Serial.println(millis());
        delay(5000);
}
