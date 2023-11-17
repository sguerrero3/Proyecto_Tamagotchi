#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <iostream>

// Prototipos de las funciones
void vStateUpdateTask(void *pvParameters);

// Declaración de variables globales y clases
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

// RQ2: Función para actualizar el estado de la mascota
void vStateUpdateTask(void *pvParameters) {
    // Declaración del semáforo Mutex para la sincronización de recursos compartidos
    SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();

    // Obtener la instancia de Pet desde los parámetros
    Pet* pet = static_cast<Pet*>(pvParameters);

    // Bucle de actualización de estado continuo
    while (true) {
        // Pedir recursos compartidos
        xSemaphoreTake(xMutex, portMAX_DELAY);
        // Actualizar el estado de la mascota sin intervención del usuario
        pet->updateState();

        // Imprimir el estado de la mascota en la consola usando Serial.print
        Serial.print("Estado de la mascota - Hambre: ");
        Serial.print(pet->hambre);
        Serial.print(", Felicidad: ");
        Serial.print(pet->felicidad);
        Serial.print(", Energia: ");
        Serial.print(pet->energia);
        Serial.print(", Sueño: ");
        Serial.print(pet->suenio);
        Serial.print(", Higiene: ");
        Serial.print(pet->higiene);
        Serial.print(", Salud: ");
        Serial.print(pet->salud);
        Serial.print(", Edad: ");
        Serial.print(pet->edad);
        Serial.print(", Peso: ");
        Serial.print(pet->peso);
        Serial.println();
                  
        // Liberar recursos compartidos
        xSemaphoreGive(xMutex);

        // Dormir durante un tiempo antes de la próxima actualización
        vTaskDelay(pdMS_TO_TICKS(1000)); // Esperar 1000 milisegundos (1 segundo)
    }
}


int main() {
    // Inicializar una instancia de la clase Pet
    Pet mascota;

    // Crear tarea y pasar la instancia de Pet como parámetro
    xTaskCreate(vStateUpdateTask, "StateUpdateTask", 1000, &mascota, 1, NULL);

    // Iniciar el programador de tareas de FreeRTOS
    vTaskStartScheduler();

    // Nunca deberías llegar aquí si el programador de tareas se inicia correctamente
    return 0;
}
