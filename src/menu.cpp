#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define BUZZER_PIN 33
#define centerButton 18
#define leftButton 5
#define rightButton 19
#define WIRE Wire

String opcionesMenu[] = {"< Menu >", "<Casa>","< Jugar >", "<Limpiar>", "<Guardar>"};
xQueueHandle menuQueue = xQueueCreate(1, sizeof(int));
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &WIRE);
int estado = 0;

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
        if (indexMenu == 0){indexMenu = 3;}
        else{indexMenu -= 1;}
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(opcionesMenu[indexMenu % 4]);
        display.display();
        receivedData = 0;
      }
      else if (receivedData == 3)
      {
        if (indexMenu == 3){indexMenu = 0;}
        else{indexMenu += 1;}
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(opcionesMenu[indexMenu % 4]);
        display.display();
        receivedData = 0;
      }
      else if (receivedData == 4)
      {
        Serial.print(indexMenu);
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
          display.print("Task Jugar");
          display.display();
          estado = 3;
          receivedData = 0;
        }
        else if(indexMenu == 3){
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Task Limpiar");
          display.display();
          estado = 4;
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
  display.print("Este NO es el menu");
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
}

void loop()
{
  delay(500);
}