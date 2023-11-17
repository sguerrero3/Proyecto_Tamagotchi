#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// LED rates
static const int rate_1 = 500;  // ms

// Pins
static const int led_pin = LED_BUILTIN;


class Pet {
  public:
    // Variables de estado de la mascota por defecto
    int hambre = 20;
    int felicidad = 50;
    int energia = 100;
    int suenio = 0;
    int higiene = 100;
    int salud = 100;
    int edad = 0;
    int peso = 30;

  // Constructor de la clase
  Pet() {}

  // Destructor de la clase
  ~Pet() {}

  // Función para actualizar el estado de la mascota por defecto
  void updateState() {
    hambre += 1;
    felicidad -= 1;
    energia -= 1;
    suenio += 1;
    higiene -= 1;
    salud -= 1;
    edad += 1;
    peso -= 1;
  }

  // Funciones para actualizar el estado de la mascota con parámetros
  void updateHambre(int value) {
    hambre += value;
  }

  void updateFelicidad(int value) {
    felicidad += value;
  }

  void updateEnergia(int value) {
    energia += value;
  }

  void updateSuenio(int value) {
    suenio += value;
  }

  void updateHigiene(int value) {
    higiene += value;
  }

  void updateSalud(int value) {
    salud += value;
  }

  void updateEdad(int value) {
    edad += value;
  }

  void updatePeso(int value) {
    peso += value;
  }

};


// Our task: blink an LED at one rate
void vStateUpdateTask(void *parameter) {
  while(1) {
    Serial.println("hola");
  }
}




// Our task: blink an LED at one rate
void toggleLED_1(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}



void setup() {

  Serial.begin(9600);

  // Configure pin
  pinMode(led_pin, OUTPUT);

  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED_1,  // Function to be called
              "Toggle 1",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  xTaskCreatePinnedToCore(
              vStateUpdateTask, 
              "StateUpdateTask", 
              1024, 
              NULL, 
              1, 
              NULL,
              app_cpu);


  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}


void loop() {
  // put your main code here, to run repeatedly:
}