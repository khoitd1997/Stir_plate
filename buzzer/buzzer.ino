
#define BUZZER_PIN 9
#define TEMPO 65
#define SONG_LENGTH 50
#define BEATS 2
#define DURATION BEATS*TEMPO
#define FREQUENCY 523 //pick from 262, 294, 330, 349, 392, 440, 494, 523
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

  for ( unsigned int i = 0; i < SONG_LENGTH; i++)
  {
          i%2?delay(DURATION):tone(BUZZER_PIN, FREQUENCY, DURATION), delay(DURATION);
          delay(TEMPO/10);  // brief pause between notes
}
}
