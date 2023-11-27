#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define BUZZER_PIN 33
#define centerButton 18
#define leftButton 5
#define rightButton 19
#define WIRE Wire
// Definiciones de variables globales
#ifndef Pet_h
#define Pet_h
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
#endif

// Prototipos de funciones para tareas
void vUITask(void *pvParameters);
void vStateUpdateTask(void *pvParameters);
void vFeedingTask(void *pvParameters);
void vWashTask(void *pvParameters);
void vGameTask(void *pvParameters);
void vSleepTask(void *pvParameters);
void vPowerControlTask(void *pvParameters);

// Definición de colas y semáforos
QueueHandle_t xGameResultQueue;
SemaphoreHandle_t xMultipleGamesSemaphore;
SemaphoreHandle_t xDataMutex;

xQueueHandle menuQueue = xQueueCreate(1, sizeof(int));
xQueueHandle comidaQueue = xQueueCreate(1, sizeof(int));
xQueueHandle lavadoQueue = xQueueCreate(1, sizeof(int));

Pet mascota;

// Definición de booleanos globales
bool requestFeeding = false;
int contadorMovimiento(float, float, float, float, float, float, int);

String opcionesMenu[] = {"< Menu >","<Casa>","<Comer>", "< Jugar >", "<Limpiar>", "<Guardar>"};
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &WIRE);
Adafruit_MPU6050 mpu;
int estado = 0;
float actual;

// Detectar boton del centro
void center_Interrupt()
{
  static unsigned long c_last_interrupt_time = 0;
  unsigned long c_interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (c_interrupt_time - c_last_interrupt_time > 200)
  {
    if (estado != 1)
    {
      estado = 1;
      int dataToSend = 1;
      xQueueSend(menuQueue, &dataToSend, portMAX_DELAY);
    }
    else if (estado == 1)
    {
      int dataToSend = 4;
      xQueueSend(menuQueue, &dataToSend, portMAX_DELAY);
    }
  }
  c_last_interrupt_time = c_interrupt_time;
}

// Detectar boton de la izquierda
void left_Interrupt()
{
  static unsigned long l_last_interrupt_time = 0;
  unsigned long l_interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (l_interrupt_time - l_last_interrupt_time > 200)
  {
    if (estado == 1)
    {
      int dataToSend = 2;
      xQueueSend(menuQueue, &dataToSend, portMAX_DELAY);
    }
    else if(estado == 3){
      int dataToSend = 1;
      xQueueSend(comidaQueue, &dataToSend, portMAX_DELAY);
    }
  }
  l_last_interrupt_time = l_interrupt_time;
}

// Detectar boton derecha
void right_Interrupt()
{
  static unsigned long r_last_interrupt_time = 0;
  unsigned long r_interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (r_interrupt_time - r_last_interrupt_time > 200)
  {
    if (estado == 1)
    {
      int dataToSend = 3;
      xQueueSend(menuQueue, &dataToSend, portMAX_DELAY);
    }
  }
  r_last_interrupt_time = r_interrupt_time;
}

// Task para el menu
void vMenuTask(void *parameter)
{
  int receivedData = 0;
  int indexMenu = 0;

  while (1)
  {

    if (xQueueReceive(menuQueue, &receivedData, portMAX_DELAY))
    {

      if (receivedData == 1)
      {
        indexMenu = 0;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(opcionesMenu[indexMenu]);
        display.display();
        receivedData = 0;
      }
      else if (receivedData == 2)
      {
        if (indexMenu == 0){indexMenu = 5;}
        else{indexMenu -= 1;}
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(opcionesMenu[indexMenu]);
        display.display();
        receivedData = 0;
      }
      else if (receivedData == 3)
      {
        if (indexMenu == 5){indexMenu = 0;}
        else{indexMenu += 1;}
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(opcionesMenu[indexMenu]);
        display.display();
        receivedData = 0;
      }
      else if (receivedData == 4)
      {
        if(indexMenu == 1){
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Task Casa");
          display.display();
          estado = 2;
          receivedData = 0;
        }
        else if(indexMenu == 2){
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Task Comer");
          display.display();
          estado = 3;
          receivedData = 0;
          requestFeeding = true;
        }
        else if (indexMenu == 4) {
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Task Limpiar");
          display.display();
          estado = 5;
          receivedData = 0;
        }
      }
    }
  }
}


void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);

  // Inicializar pantalla
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(500);
  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Welcome Back");
  display.display();

  Serial.begin(9600);

  // Pines para botones
  pinMode(centerButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(centerButton), center_Interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(leftButton), left_Interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(rightButton), right_Interrupt, FALLING);

  // Tarea para menu
  xTaskCreate(vMenuTask, "MenuTask", 2048, NULL, 1, NULL);

  // Instanciar la clase Pet
  Pet mascota = Pet();

  // Crear colas y semáforos
  xGameResultQueue = xQueueCreate(5, sizeof(uint8_t));
  xMultipleGamesSemaphore = xSemaphoreCreateMutex();
  xDataMutex = xSemaphoreCreateMutex();

  // Crear tareas
  //xTaskCreate(vUITask, "UI Task", 1024, NULL, 1, NULL);
  xTaskCreate(vStateUpdateTask, "State Update Task", 1024, NULL, 1, NULL);
  xTaskCreate(vFeedingTask, "Feeding Task", 1024, NULL, 1, NULL);
  xTaskCreate(vWashTask, "Wash Task", 1024, NULL, 1, NULL);
  //xTaskCreate(vGameTask, "Game Task", 1024, NULL, 1, NULL);
  //xTaskCreate(vSleepTask, "Sleep Task", 1024, NULL, 1, NULL);
  //xTaskCreate(vPowerControlTask, "Power Control Task", 1024, NULL, 1, NULL);

  // Iniciar el planificador de FreeRTOS
  //vTaskStartScheduler();
}

void loop()
{

}

void vStateUpdateTask(void * pvParameters) {
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
  vTaskDelay(pdMS_TO_TICKS(10000)); // Esperar 30 segundos
  }
}

void vFeedingTask(void* pvParameters) {
  while (1) {
    // Verificar si hay una petición del usuario en la cola o a través de la bandera
    int receivedData = 0;
    if (xQueueReceive(comidaQueue, &receivedData, portMAX_DELAY)) {
      
      Serial.println("Alimentando a la mascota...");

      xSemaphoreTake(xDataMutex, portMAX_DELAY);
      if(mascota.hambre >= 0){
        mascota.updateHambre(-1);
      }
      xSemaphoreGive(xDataMutex);
    }

    // Esperar un tiempo antes de la próxima verificación
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void vWashTask(void* pvParameters) {
  while (1) {
    // Verificar si hay una petición del usuario en la cola o a través de la bandera
    int receivedData = 0;
    if (xQueueReceive(lavadoQueue, &receivedData, portMAX_DELAY)) {
      
      Serial.println("Limpiando a la mascota...");
      xSemaphoreTake(xDataMutex, portMAX_DELAY);
      sensors_event_t g, a, temp;
      mpu.getEvent(&g, &a, &temp);
      int contador2 = contadorMovimiento(a.acceleration.x, a.acceleration.y, a.acceleration.z, g.gyro.x, g.gyro.y, g.gyro.z, 0);
      if(contador2 == 10){
        if(mascota.higiene >= 0){
          mascota.updateHigiene(10);
        }
      }
      xSemaphoreGive(xDataMutex);
    }

    // Esperar un tiempo antes de la próxima verificación
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

int contadorMovimiento(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ, int contador) {
  // Agitar el tamagotchi para incrementar la limpieza
  while (contador < 10) {
    if (sqrt(accX * accX + accY * accY + accZ * accZ) > 3.5 && 
        sqrt(gyroX * gyroX + gyroY * gyroY + gyroZ * gyroZ) > 3.5) {
      contador += 1;
    }
  }
  return contador;
}