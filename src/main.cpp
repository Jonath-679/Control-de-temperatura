// Practica -> Control de temperatura
#include <Arduino.h>
#include <Wire.h> //I2C
#include <Adafruit_GFX.h> //Graficos
#include <Adafruit_SSD1306.h> //DisplayOled

// PINES
Adafruit_SSD1306 display(128, 64, &Wire, -1);
const int sensor = A0;
const int motor = 11;

// Variables globales
double temperatura;
float resistenciaSensor;
int potencia;
int pwm;

void setup() {
    // Configuracion general
    Serial.begin(9600);
    pinMode(sensor, INPUT);
    pinMode(motor, OUTPUT);

    // Configuracion del display | Direccion I2C = 0x3C
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Inicializa el display (*si falla, el programa "se detiene")
        Serial.println(F("Fallo al encontrar el display OLED"));
        for(;;); 
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
}

// Muestra los datos del sistema en el display
void mostrarDatos(double temperatura, float resistenciaSensor, int pwm, int potencia) {
    display.clearDisplay();
    // Temperatura
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temperatura, 1);
    display.print(" C");
    // Ohms
    display.setCursor(0, 16);
    display.print("Resistencia: ");
    display.print(resistenciaSensor, 1);
    display.print(" Ohms");
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
    // Calculo de temperatura en base al voltage
    temperatura = 27.77777778 * analogRead(sensor);

    // Proteccion de rango (0-100)
    if(temperatura < 0.0) temperatura = 0.0;
    else if(temperatura > 100.0) temperatura = 100.0;
    
    // Calculo de la potencia en base a discretizar la temp (paso 20).
    if(temperatura >= 98.0) potencia = 100;
    else if(temperatura >= 80.0) potencia = 80;
    else if(temperatura >= 60.0) potencia = 60;
    else if(temperatura >= 40.0) potencia = 40;
    else if(temperatura >= 20.0) potencia = 20;
    else potencia = 0;

    // Calculo de Resistencia y pwm
    resistenciaSensor = 100 + (0.385*int(temperatura));
    pwm = map(potencia, 0, 100, 0, 255);

    // Enviar datos al display
    mostrarDatos(temperatura, resistenciaSensor, pwm, potencia);

    // Enviar pwm al motor
    analogWrite(motor, pwm);
    delay(100);
}

// El .hex se encuentra en: .pio/build/nanoatmega328new/firmware.hex | previamente el codigo nececita compilarse
// Si da error al cargar el codigo al arduino, utilizar el bootloader old (quitando las extenciones de "new" en el archivo .platformio.ini)

// Temperatura | Ohms     | Volts
//-------------------------------
// 0           | 100      | 0V
// 1           | 100.385  | 0.036V
// 100         | 138.5    | 3.7V
// 100+        | 138.5+   | 3.7V

