// Moses R2 code example
// Thank you to Arduino Documentation and Adafruit, as well as numerous open-source contributors fot the libraries
// and kowledge that enabled me to write this code


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BMP085.h>

float ground = 0;
float zeroed = 0;
float altitude = 0;
float oldAltitude = 0;
float velocity = 0;
float change;

unsigned long oldTime = 0;
unsigned long currentTime = 0;
float deltaTime = 0;

unsigned long  lastAction = 0;

float altitudeLog[250] = {0};

int CSpin = 4;

int index = 0;

Adafruit_BMP085 bmp;


void setup() {
    Serial.begin(9600);
    if (!SD.begin(CSpin)){
        Serial.println("SD Init Error, check wiring or if card installed!");
    }
    Serial.println("SD init success, moving to baro");
    if (!bmp.begin()){
        Serial.println("Baro init failed, check wiring or PCB!");
    }
    Serial.println("Baro init success!");
    ground = bmp.readAltitude();
    Serial.print("Computer Zeroed! Ground Altitude: ");
    Serial.print(ground);
    Serial.println("m");
    currentTime = millis();
}

void apogee() {
    
}

void flying() {
    currentTime = millis();
    deltaTime = (currentTime - oldTime) / 1000;
    altitude = bmp.readAltitude() - ground;
    change = altitude - oldAltitude;
    velocity = change / deltaTime;
    if (velocity > 2) {
        apogee();
    }
    if (currentTime - lastAction >= 1000 && index < 250) {
        altitudeLog[index] = altitude;
        index +=1;
        lastAction = currentTime;
    }
    oldTime = currentTime;
    oldAltitude = altitude;
}

void loop() {
    currentTime = millis();
    altitude = bmp.readAltitude() - ground;
    change = altitude - oldAltitude;
    deltaTime = (currentTime - oldTime) / 1000;
    velocity = change / deltaTime;
    if (velocity > 1) {
        flying();
    }
    oldTime = currentTime;
    oldAltitude = altitude;
}