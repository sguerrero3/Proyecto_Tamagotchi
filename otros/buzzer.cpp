#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#define BUZZER_PIN 33

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 1000);
}

void loop()
{

}