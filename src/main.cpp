// Practica -> Control de temperatura
#include <Arduino.h>
#include <Wire.h> //I2C
#include <Adafruit_GFX.h> //Graficos
#include <Adafruit_SSD1306.h> //DisplayOled

// PINES
Adafruit_SSD1306 display(128, 64, &Wire, -1);
const int sensor = A0; //potenciometro
const int motor = 11;

// Variables globales
float temperatura = 0;
int potencia = 0; //0-100%
int pwm = 0;

void setup() {
    // Configuracion general
    Serial.begin(9600);
    pinMode(sensor, INPUT);
    pinMode(motor, OUTPUT);

    // Configuracion del display | La direccion I2C es 0x3C (o si no, 0x3D)
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Inicializa el display (*si falla, el programa "se detiene")
        Serial.println(F("Fallo al encontrar el display OLED"));
        for(;;); 
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
}

// Muestra los datos del sistema en el display
void mostrarDatos(float temperatura, float ohms, int pwm, int potencia) {
    display.clearDisplay();
    // Temperatura
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temperatura, 1);
    display.print(" C");
    // Ohms
    display.setCursor(0, 16);
    display.print("Sensor: ");
    display.print(ohms, 1);
    display.print(" Ohm");
    // PWM (0, 255)
    display.setCursor(0, 32);
    display.print("PWM: ");
    display.print(pwm);
    display.print(" / 255");
    // Potencia (0-100%)
    display.setCursor(0, 48);
    display.print("Potencia: ");
    display.print(potencia);
    display.print("%");
    //transfiere la info de la memoria (buffer) al display (fisico)
    display.display(); 
}

void loop() {
    // Se simula el sensor con una relacion lineal | 0V = 0- grados | 5v = 100+ Grados
    temperatura = map(analogRead(sensor), 0, 1023, 0, 100);
    pwm = map(temperatura, 0, 100, 0, 255);
    potencia = temperatura; //ya que la temperatura va de 0 a 100 (en este caso)
    mostrarDatos(temperatura, 0.0, pwm, potencia);
    analogWrite(motor, pwm);
    delay(100);
}

/*
NOATAS:
- El .hex que necesita el arduino del proteus se encuentra en: .pio/build/nanoatmega328new/firmware.hex | previamente el codigo nececita compilarse
- Si da error al cargar el codigo al arduino, utilizar el bootloader old (quitando las extenciones de "new" en el archivo .platformio.ini)
*/

