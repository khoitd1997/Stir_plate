
#define BUZZER_PIN 9//PWM pin that controls the buzzer
#define TEMPO 65
#define SONG_LENGTH 50//More beep if bigger number
#define BEATS 2
#define DURATION BEATS*TEMPO//duration of each note played
#define FREQUENCY 523 //pick from 262, 294, 330, 349, 392, 440, 494, 523
#define ALARM_ON 1//1 for sounding alarm 0 for not

void setup()
{
        pinMode(BUZZER_PIN, OUTPUT);
}


void loop()
{
static unsigned long start_time=millis();//millis() may overflow but the device is expected to operate only for short DURATION

if((millis()-start_time>DURATION+TEMPO/10) && ALARM_ON){
play_alarm();
start_time=millis();
}

}

void play_alarm(void){

  static unsigned int i=0;//doesn't matter if i overflow
if(i++%2!=0)

tone(BUZZER_PIN, FREQUENCY, DURATION);


}
