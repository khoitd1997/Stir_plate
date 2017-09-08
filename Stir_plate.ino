//Use pin 9 because it will only affect the servo function
//Pin 9 base frequency is 31250 Hz
void setup() {
//this will use timer 2, which can affect millis(). Timer 2 affects 3 and 11, which can affect delay
        /*
           TCCR2A = 0x23;
           TCCR2B = 0x0C; // select timer2 clock as 16 MHz I/O clock / 64 = 250 kHz
           OCR2A = 9; //gives 25.0 kHz square wave
           pinMode(3, OUTPUT); // enable the PWM output (you now have a PWM signal on digital pin 3)
           OCR2B = 0;
         */
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
