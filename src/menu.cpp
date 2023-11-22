#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define centerButton 35
#define leftButton 34
#define rightButton 32

String opcionesMenu[] = {"Menu", "Jugar", "Bañar", "Guardar"};
int total = 0;

SemaphoreHandle_t xButtonSemaphore;

bool enMenu = false;

void blink() {
    static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    total += 1;
  }
  last_interrupt_time = interrupt_time;
}



void setup() {

    Serial.begin(9600);

    xButtonSemaphore = xSemaphoreCreateBinary();

    if (xButtonSemaphore == NULL) {
        Serial.println("Semaphore creation failed!");
        while (1);
    }
    else{
        Serial.println("Semaphore creation Good!");
    }

    pinMode(centerButton, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(centerButton), blink, CHANGE);

}


void loop() {
  // put your main code here, to run repeatedly:
    Serial.println(total);
    delay(500);
}