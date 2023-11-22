#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>

// Definiciones de variables globales
Pet mascota;

// Prototipos de funciones para tareas
void vUITask(void *pvParameters);
void vStateUpdateTask(void *pvParameters);
void vFeedingTask(void *pvParameters);
void vGameTask(void *pvParameters);
void vSleepTask(void *pvParameters);
void vPowerControlTask(void *pvParameters);

// Definición de colas y semáforos
QueueHandle_t xUserInputQueue;
QueueHandle_t xGameResultQueue;
SemaphoreHandle_t xFoodAvailableSemaphore;
SemaphoreHandle_t xMultipleGamesSemaphore;
SemaphoreHandle_t xUserInputSemaphore;
SemaphoreHandle_t xDataMutex;

// Creación de la estructura de datos principal
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

void setup() {
  // Inicialización de periféricos y recursos aquí
  // Instanciar la clase Pet
  mascota = Pet();

  // Crear colas y semáforos
  xUserInputQueue = xQueueCreate(5, sizeof(uint8_t));
  xGameResultQueue = xQueueCreate(5, sizeof(uint8_t));
  xFoodAvailableSemaphore = xSemaphoreCreateBinary();
  xMultipleGamesSemaphore = xSemaphoreCreateMutex();
  xUserInputSemaphore = xSemaphoreCreateBinary();
  xDataMutex = xSemaphoreCreateMutex();

  // Crear tareas
  xTaskCreate(vUITask, "UI Task", 2048, NULL, 2, NULL);
  xTaskCreate(vStateUpdateTask, "State Update Task", 2048, NULL, 2, NULL);
  xTaskCreate(vFeedingTask, "Feeding Task", 2048, NULL, 2, NULL);
  xTaskCreate(vGameTask, "Game Task", 2048, NULL, 2, NULL);
  xTaskCreate(vSleepTask, "Sleep Task", 2048, NULL, 2, NULL);
  xTaskCreate(vPowerControlTask, "Power Control Task", 2048, NULL, 2, NULL);

  // Iniciar el planificador de FreeRTOS
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void vStateUpdateTask(void* pvParameters) {
  while (1) {
  // Actualizar estado general de la mascota

  // Actualizar datos compartidos
  xSemaphoreTake(xDataMutex, portMAX_DELAY);
  mascota.updateState();
    // Mostrar información del estado en el puerto serie (serial)
    Serial.println("Estado de la mascota:");
    Serial.print("Hambre: "); Serial.println(mascota.hambre);
    Serial.print("Felicidad: "); Serial.println(mascota.felicidad);
  xSemaphoreGive(xDataMutex);
  vTaskDelay(pdMS_TO_TICKS(1000)); // Esperar 1 segundo
  }
}

void vFeedingTask(void* pvParameters) {
  while (1) {
    // Esperar a que el usuario ingrese un valor
    xSemaphoreTake(xUserInputSemaphore, portMAX_DELAY);
    // Actualizar datos compartidos
    xSemaphoreTake(xDataMutex, portMAX_DELAY);
    mascota.updateHambre(-10);
    xSemaphoreGive(xDataMutex);
    // Liberar semáforo de comida disponible
    xSemaphoreGive(xFoodAvailableSemaphore);
  }
}
