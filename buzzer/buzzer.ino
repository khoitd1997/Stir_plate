
#define BUZZER_PIN 6//PWM pin that controls the buzzer
#define TEMPO 65
#define SONG_LENGTH 50//More beep if bigger number
#define BEATS 2
#define DURATION BEATS*TEMPO//duration of each note played
#define FREQUENCY 523 //pick from 262, 294, 330, 349, 392, 440, 494, 523
#define NUM_ARLARM_TO_PLAY 22
void play_alarm(void);
void setup()
{
        pinMode(BUZZER_PIN, OUTPUT);
}


void loop()
{

play_alarm();
delay(5000);
}



void play_alarm(void){

static unsigned int i=0;//doesn't matter if i overflow
while(i<NUM_ARLARM_TO_PLAY){
if(i++%2!=0)
tone(BUZZER_PIN, FREQUENCY, DURATION);
delay(DURATION+TEMPO/10);
}
}
