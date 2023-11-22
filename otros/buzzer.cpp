#include <Arduino.h>
#define BUZZER_PIN 33

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  tone(BUZZER_PIN, 1000); //Send 1KHz sound signal
  delay(1000);

  noTone(BUZZER_PIN); //Stop sound
  delay(1000);
}