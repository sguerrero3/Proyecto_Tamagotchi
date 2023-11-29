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
    int felicidad = 20;
    int energia = 20;
    int suenio = 20;
    int higiene = 20;
    int salud = 20;
    int edad = 0;
    int peso = 20;

  // Constructor de la clase
  Pet() {}

  // Destructor de la clase
  ~Pet() {}

  // Función para actualizar el estado de la mascota por defecto
  void updateState() {
    if (hambre > 0){hambre -= 1;}
    if(felicidad > 0){felicidad -= 1;}
    if(energia > 0){energia -= 1;}
    if (higiene > 0){higiene -= 1;}
    if (salud > 0){salud -= 1;}
    edad += 1;

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


//Imagenes
const unsigned char PROGMEM zorra [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x28, 0x0E, 0x00, 0x24,
0x32, 0x00, 0x23, 0xC2, 0x00, 0x2C, 0x34, 0x00, 0x30, 0x0C, 0x00, 0x20, 0x04, 0x00, 0x24, 0x24,
0x00, 0x44, 0x22, 0x00, 0x41, 0x82, 0x00, 0x80, 0x01, 0x00, 0x60, 0x06, 0x00, 0x1F, 0xF8, 0x00,
0x04, 0x20, 0x1E, 0x08, 0x10, 0x62, 0x08, 0x10, 0xA2, 0x10, 0x09, 0x12, 0x14, 0x29, 0x0C, 0x14,
0x2A, 0x04, 0x14, 0x2A, 0x08, 0x24, 0x24, 0x30, 0x24, 0x24, 0xC0, 0x12, 0x4F, 0x00, 0x0F, 0xFC,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM background [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x80, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x47, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x28, 0x40, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x80, 0x10, 0x20, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00,
0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00,
0x00, 0x04, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x00,
0x00, 0x04, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00,
0x00, 0x04, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xCC, 0x00,
0x00, 0x06, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x38, 0x00,
0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,
0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,
0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x10, 0x00,
0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x83, 0x10, 0x00,
0x00, 0x00, 0x80, 0x00, 0x00, 0x03, 0xC0, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x00, 0xD0, 0x00,
0x00, 0x00, 0x80, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x03, 0xFC, 0x00, 0x30, 0x00,
0x00, 0x7F, 0xFC, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFE, 0x00, 0x00, 0x0C, 0x00,
0x0F, 0xC0, 0x07, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03,
0xF0, 0x00, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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
xQueueHandle juegoQueue = xQueueCreate(1, sizeof(int));
xQueueHandle casaQueue = xQueueCreate(1, sizeof(int));

Pet mascota;

// Definición de booleanos globales
bool requestFeeding = false;
int contadorMovimiento(float, float, float, float, float, float, int);

String opcionesMenu[] = {"< Menu >","<Casa>","<Comer>", "< Jugar >", "<Limpiar>", "<Dormir>"};
String opcionesJuego[] = {"Piedra","Papel","Tijera"};
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
    //else if (estado == 4)
    //{
    //  int dataToSend = 3;
    //  xQueueSend(juegoQueue, &dataToSend, portMAX_DELAY);
    //}
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
    else if(estado ==2){
      int dataToSend = 2;
      xQueueSend(casaQueue, &dataToSend, portMAX_DELAY);
    }
    else if(estado == 3){
      int dataToSend = 1;
      xQueueSend(comidaQueue, &dataToSend, portMAX_DELAY);
    }
    else if (estado == 4)
    {
      int dataToSend = 2;
      xQueueSend(juegoQueue, &dataToSend, portMAX_DELAY);
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
    else if(estado ==2){
      int dataToSend = 3;
      xQueueSend(casaQueue, &dataToSend, portMAX_DELAY);
    }
    else if (estado == 4)
    {
      int dataToSend = 4;
      xQueueSend(juegoQueue, &dataToSend, portMAX_DELAY);
    }
  }
  r_last_interrupt_time = r_interrupt_time;
}


//Deteccion movimineto
void vMovimientoTAsk(void *parameter){
  
  while(1){

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    if(abs(a.acceleration.x) >= 15 || abs(a.acceleration.y) >= 15 || abs(a.acceleration.z) >= 15){
      
      if (estado == 5){
        int dataToSend = 1;
        xQueueSend(lavadoQueue, &dataToSend, portMAX_DELAY);
      }
    }
    delay(200);
  }
}


//Deteccion movimineto
void vEstadisticas(void *parameter){
  
  while(1){

    if(estado!= 1 && estado !=0){

      display.drawLine(6, 4, 25, 4, SSD1306_BLACK);
      display.drawLine(6, 4, 5 + mascota.salud, 4, SSD1306_WHITE);

      display.drawLine(6, 8, 25, 8, SSD1306_BLACK);
      display.drawLine(6, 8, 5 + mascota.hambre, 8, SSD1306_WHITE);

      display.drawLine(6, 12, 25, 12, SSD1306_BLACK);
      display.drawLine(6, 12, 5 + mascota.energia, 12, SSD1306_WHITE);

      display.drawLine(6, 16, 25, 16, SSD1306_BLACK);
      display.drawLine(6, 16, 5 + mascota.higiene, 16, SSD1306_WHITE);

      display.drawLine(6, 20, 25, 20, SSD1306_BLACK);
      display.drawLine(6, 20, 5 + mascota.felicidad, 20, SSD1306_WHITE);

      display.display(); 
    }
    delay(5);
  }
}


//Task casa
void vCasaTask(void *parameter){

  int receivedData = 0;
  int pos = 50;

  while (1) {

    if (xQueueReceive(casaQueue, &receivedData, portMAX_DELAY))
    {

      if (receivedData == 1)
      {
        pos = 50;
      }
      else if (receivedData == 2){
        
        pos -= 5;
      }

      else if (receivedData == 3){ 
        pos += 5;
      }

      display.clearDisplay();
      display.drawBitmap(0,0,background, 128, 64, 1);
      display.drawBitmap(pos,25,zorra, 24, 32, 1);
      display.display();
      receivedData = 0;

    }

    delay(200);
  }
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
          int dataToSend = 1;
          xQueueSend(casaQueue, &dataToSend, portMAX_DELAY);
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
        else if (indexMenu == 3) {
          int dataToSend = 1;
          xQueueSend(juegoQueue, &dataToSend, portMAX_DELAY);
          estado = 4;
          receivedData = 0;
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
  xTaskCreate(vMovimientoTAsk, "MovimientoTask", 2048, NULL, 1, NULL);
  xTaskCreate(vCasaTask, "casaTask", 2048, NULL, 1, NULL);
  xTaskCreate(vEstadisticas, "estadisticasTask", 2048, NULL, 1, NULL);

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
  xTaskCreate(vGameTask, "Game Task", 1024, NULL, 1, NULL);
  //xTaskCreate(vSleepTask, "Sleep Task", 1024, NULL, 1, NULL);
  //xTaskCreate(vPowerControlTask, "Power Control Task", 1024, NULL, 1, NULL);

  // Iniciar el planificador de FreeRTOS
  //vTaskStartScheduler();

  mpu.begin();
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
    Serial.print("Higiene: "); Serial.println(mascota.higiene);
  xSemaphoreGive(xDataMutex);
  vTaskDelay(pdMS_TO_TICKS(2000)); 
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
        mascota.updateHambre(1);
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
      if(receivedData == 1){
        if(mascota.higiene <= 100){
          mascota.updateHigiene(1);
        }
      }
      xSemaphoreGive(xDataMutex);
    }
  }
}


void vGameTask(void* pvParameters) {
  int receivedData = 0;
  int indexJuego;
  int opcionUsuario;
  int opcionTamagotchi;
  int resultadoJuego;

  while (1) {
    // Verificar si hay una petición del usuario en la cola o a través de la bandera
    if (xQueueReceive(juegoQueue, &receivedData, portMAX_DELAY)) {

        if (receivedData == 1) {
          indexJuego = 0;
          display.clearDisplay();
          display.print(opcionesJuego[indexJuego]);
          display.display();
        }

        else if (receivedData == 2) {
          display.clearDisplay();
          display.print("Piedra");
          display.display();
          opcionUsuario = 0;
          delay(1000);
          display.clearDisplay();
          opcionTamagotchi = std::rand() % 3;
          if (opcionTamagotchi == 0) {
            display.print("Piedra - Empate");
            display.display();
          }
          else if (opcionTamagotchi == 1) {
            display.print("Papel - Perdiste");
            display.display();
          }
          else if (opcionTamagotchi == 2) {
            display.print("Tijera - Ganaste");
            display.display();
          }
        }

        else if (receivedData == 3) {
          display.clearDisplay();
          display.print("Papel");
          display.display();
          opcionUsuario = 1;
          delay(1000);
          display.clearDisplay();
          opcionTamagotchi = std::rand() % 3;
          if (opcionTamagotchi == 0) {
            display.print("Piedra - Ganaste");
            display.display();
          }
          else if (opcionTamagotchi == 1) {
            display.print("Papel - Empate");
            display.display();
          }
          else if (opcionTamagotchi == 2) {
            display.print("Tijera - Perdiste");
            display.display();
          }
        }

        else if (receivedData == 4) {
          display.clearDisplay();
          display.print("Tijera");
          display.display();
          opcionUsuario = 2;
          delay(1000);
          display.clearDisplay();
          opcionTamagotchi = std::rand() % 3;
          if (opcionTamagotchi == 0) {
            display.print("Piedra - Perdiste");
            display.display();
          }
          else if (opcionTamagotchi == 1) {
            display.print("Papel - Ganaste");
            display.display();
          }
          else if (opcionTamagotchi == 2) {
            display.print("Tijera - Empate");
            display.display();
          }
        }

      if (xQueueSend(xGameResultQueue, &resultadoJuego, portMAX_DELAY) == pdPASS) {

              // Realizar las acciones correspondientes según el resultado
              xSemaphoreTake(xDataMutex, portMAX_DELAY);
              switch (resultadoJuego) {
                  case 1:
                      // Usuario gana
                      if (mascota.felicidad <= 100) {
                          mascota.updateFelicidad(5);
                      }
                      break;
                  case 2:
                      // Máquina gana
                      if (mascota.felicidad >= 0) {
                          mascota.updateFelicidad(-5);
                      }
                      break;
                  default:
                      // Empate
                      break;

              }
              xSemaphoreGive(xDataMutex);
          }
      }
    // Pausa la tarea durante un breve periodo
    vTaskDelay(pdMS_TO_TICKS(5000));  // Pausa durante 5000 milisegundos (5 segundos)
  }
}

/*
    while (1) {
        // Esperar a que el semáforo indique que se puede jugar
        int receivedData = 0;
        int opcionUsuario;

        if (xQueueReceive(juegoQueue, &receivedData, portMAX_DELAY) && receivedData == 1) {
            display.clearDisplay();
            display.print("Juego");
            // Realizar el juego de piedra-papel-tijera
            if (receivedData == 2) {
              int opcionUsuario = 0;
              display.print("Piedra");
            }
            else if (receivedData == 3) {
              int opcionUsuario = 1;
              display.print("Papel");
            }
            else if (receivedData == 4) {
              int opcionUsuario = 2;
              display.print("Tijera");
            }
            int opcionTamagotchi = (rand() % 3);  // Elección automática
            display.print(opcionTamagotchi);

          // Piedra > Tijera, Tijera > Papel, Papel > Piedra
          uint8_t resultadoJuego;
          if ((opcionUsuario == 0 && opcionTamagotchi == 2) ||
              (opcionUsuario == 1 && opcionTamagotchi == 0) ||
              (opcionUsuario == 2 && opcionTamagotchi == 1)) {
              // Usuario gana
              resultadoJuego = 1;
          } else if (opcionUsuario == opcionTamagotchi) {
              // Empate
              resultadoJuego = 0;
          } else {
              // Máquina gana
              resultadoJuego = 2;
          }

          // Guardar el resultado en la cola
          int32_t resultado;
          resultado = resultadoJuego;
          if (xQueueSend(xGameResultQueue, &resultado, portMAX_DELAY) == pdPASS) {

              // Realizar las acciones correspondientes según el resultado
              xSemaphoreTake(xDataMutex, portMAX_DELAY);
              switch (resultadoJuego) {
                  case 1:
                      // Usuario gana
                      if (mascota.felicidad <= 100) {
                          mascota.updateFelicidad(10);
                      }
                      break;
                  case 2:
                      // Máquina gana
                      if (mascota.felicidad >= 0) {
                          mascota.updateFelicidad(-5);
                      }
                      break;
                  default:
                      // Empate
                      break;

              }
              xSemaphoreGive(xDataMutex);
          }
          xSemaphoreGive(xMultipleGamesSemaphore);
        }

        // Pausa la tarea durante un breve periodo
        vTaskDelay(pdMS_TO_TICKS(5000));  // Pausa durante 5000 milisegundos (5 segundos)
    }
}
*/