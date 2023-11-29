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
    if(hambre < 20){hambre += value;}
  }

  void updateFelicidad(int value) {
    if(felicidad < 20){felicidad += value;}
  }

  void updateEnergia(int value) {
    if(energia < 20){energia += value;}
  }

  void updateSuenio(int value) {
    if(suenio < 20){suenio += value;}
  }

  void updateHigiene(int value) {
    if(higiene < 20){higiene += value;}
  }

  void updateSalud(int value) {
    if(salud < 20){salud += value;}
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

const unsigned char PROGMEM comida []  = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x70, 0x00,
0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x70, 0x00,
0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x70, 0x00,
0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x80, 0x00,
0x00, 0x1C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x80, 0x00,
0x00, 0x1C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xCF, 0xF0, 0x00,
0x00, 0x1C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xCC, 0x30, 0x00,
0x00, 0xE0, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x0C, 0x00,
0x00, 0xE0, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x0C, 0x00,
0x00, 0xE0, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0xE0, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0xE0, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0xE0, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x1C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x1C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x1C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x1C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x03, 0x80, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x03, 0x80, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x03, 0x80, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x00, 0x7F, 0xE3, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x00, 0x7F, 0xE3, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00,
0x00, 0x00, 0x7F, 0xE3, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x0C, 0x00,
0x00, 0x00, 0x00, 0x1C, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x0C, 0x00,
0x00, 0x00, 0x00, 0x1C, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xCF, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x1C, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xCF, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM SOAP [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x03, 0x80, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0xE6, 0x00, 0x00, 0x04, 0x40, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0xE6, 0x00, 0x00, 0x09, 0xA0, 0x00,
0x07, 0xFF, 0xFF, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x66, 0x00, 0x00, 0x08, 0xA0, 0x00,
0x04, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x66, 0x00, 0x00, 0x08, 0x20, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x04, 0x40, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x03, 0x80, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x70,
0x04, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x88,
0x07, 0xFF, 0xFF, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0xC0, 0x01, 0x3C,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x06, 0x03, 0xC0, 0x01, 0x0C,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x78, 0x00, 0x3C, 0x01, 0x04,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4D, 0x00, 0x00, 0x78, 0x00, 0x3C, 0x00, 0x88,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x00, 0x07, 0x80, 0x00, 0x03, 0x00, 0x70,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x1F, 0x80, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x18, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x03, 0xE0, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0xF8, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x06, 0x00, 0x01, 0x04, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x06, 0x00, 0x02, 0xC2, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 0x00, 0x04, 0x91, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 0x00, 0x04, 0x11, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x19, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x09, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0xF8, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x40, 0x00, 0x07, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x00, 0x18, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x80, 0x00, 0x20, 0x08, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x40, 0x84, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xC4, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x22, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x32, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x22, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x62, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x44, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x84, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x08, 0x00, 0x00
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
xQueueHandle comidaQueue = xQueueCreate(4, sizeof(int));
xQueueHandle lavadoQueue = xQueueCreate(4, sizeof(int));
xQueueHandle casaQueue = xQueueCreate(1, sizeof(int));

Pet mascota;

// Definición de booleanos globales
bool requestFeeding = false;
int contadorMovimiento(float, float, float, float, float, float, int);

String opcionesMenu[] = {"< Menu >","<Casa>","<Comer>", "< Jugar >", "<Limpiar>", "<Dormir>"};
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
    else if(estado ==2){
      int dataToSend = 2;
      xQueueSend(casaQueue, &dataToSend, portMAX_DELAY);
    }
    else if(estado == 3){
      int dataToSend = 2;
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
    else if(estado ==2){
      int dataToSend = 3;
      xQueueSend(casaQueue, &dataToSend, portMAX_DELAY);
    }
    else if(estado == 3){
      int dataToSend = 3;
      xQueueSend(comidaQueue, &dataToSend, portMAX_DELAY);
    }
  }
  r_last_interrupt_time = r_interrupt_time;
}


//Deteccion movimineto
void vMovimientoTAsk(void *parameter){
  
  while(1){

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    Serial.println(a.acceleration.x);

    if(abs(a.acceleration.x) >= 11 || abs(a.acceleration.y) >= 11 || abs(a.acceleration.z) >= 11){
      
      if (estado == 5){
        int dataToSend = 2;
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

void vFeedingTask(void* pvParameters) {

  int receivedData = 0;

  while (1) {
    // Verificar si hay una petición del usuario en la cola o a través de la bandera
    if (xQueueReceive(comidaQueue, &receivedData, portMAX_DELAY)) {
      
      if(receivedData == 1){
        display.clearDisplay();
        display.drawBitmap(0,0,comida, 128, 64, 1);
        display.drawBitmap(50,25,zorra, 24, 32, 1);
        display.display();
      }

      else if(receivedData == 2){
        Serial.println("Alimentando a la mascota 1...");
        if(xSemaphoreTake(xDataMutex, portMAX_DELAY)){
          mascota.updateHambre(1);
          xSemaphoreGive(xDataMutex);
          display.fillCircle(18, 38, 2, SSD1306_WHITE);
          display.display();
          delay(200);
          display.fillCircle(18, 38, 2, SSD1306_BLACK);
          display.display();
        }
      }
      else if(receivedData == 3){
        Serial.println("Alimentando a la mascota 2...");

        if(xSemaphoreTake(xDataMutex, portMAX_DELAY)){
          mascota.updateHambre(1);
          xSemaphoreGive(xDataMutex);
          display.fillCircle(108, 38, 2, SSD1306_WHITE);
          display.display();
          delay(200);
          display.fillCircle(108, 38, 2, SSD1306_BLACK);
          display.display();
        }
      }

      receivedData = 0;
      
    }
    // Esperar un tiempo antes de la próxima verificación
    delay(100);
}}

//Task casa
void vCasaTask(void *parameter){
  int receivedData = 0;
  int pos = 50;
  while (1) {
    if (xQueueReceive(casaQueue, &receivedData, portMAX_DELAY)){
      if (receivedData == 1){pos = 50;}
      else if (receivedData == 2){pos -= 5;}
      else if (receivedData == 3){pos += 5;}
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
          int dataToSend = 1;
          xQueueSend(comidaQueue, &dataToSend, portMAX_DELAY);
          estado = 3;
          receivedData = 0;
        }
        else if (indexMenu == 4) {
          int dataToSend = 1;
          xQueueSend(lavadoQueue, &dataToSend, portMAX_DELAY);
          estado = 5;
          receivedData = 0;
        }
        else if (indexMenu == 5) {
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Task Dormir");
          display.display();
          estado = 6;
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
  display.print("Welcome :)");
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
  xTaskCreate(vFeedingTask, "Feeding Task", 2048, NULL, 1, NULL);
  xTaskCreate(vWashTask, "Wash Task", 2048, NULL, 1, NULL);
  //xTaskCreate(vGameTask, "Game Task", 1024, NULL, 1, NULL);
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

void vWashTask(void* pvParameters) {
  int receivedData = 0;
  
  while (1) {
    // Verificar si hay una petición del usuario en la cola o a través de la bandera
    if (xQueueReceive(lavadoQueue, &receivedData, portMAX_DELAY)) {
      
      if(receivedData == 1){
        display.clearDisplay();
        display.drawBitmap(0,0,SOAP, 128, 64, 1);
        display.drawBitmap(10,25,zorra, 24, 32, 1);
        display.display();
        receivedData = 0;
      }
      else if (receivedData == 2){
        if(xSemaphoreTake(xDataMutex, portMAX_DELAY)){
          Serial.println("Limpiando a la mascota...");
          mascota.updateHigiene(1);
          xSemaphoreGive(xDataMutex);
        }
      }
    }   
  }
}
