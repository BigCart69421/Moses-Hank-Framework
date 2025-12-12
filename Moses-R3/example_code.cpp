// Moses R2 code example
// Thank you to Arduino Documentation and Adafruit, as well as numerous open-source contributors fot the libraries
// and knowledge that enabled me to write this code

/* 
Written by Delta Aerospace, All Rights Reserved. Licensed under MIT License 2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BME280.h>

float ground = 0;
float altitude = 0;
float oldAltitude = 0;
float velocity = 0;
float change;

bool takeoff = false;
bool apogee = false;

int led = 3;
int buzzer = 8;

unsigned long oldTime = 0;
unsigned long currentTime = 0;
float deltaTime = 0;

unsigned long  lastAction = 0;

float altitudeLog[150] = {0};

int CSpin = 4;

int logIndex = 0;

Adafruit_BME280 bme;


void setup() {
    Serial.begin(9600);
    if (!SD.begin(CSpin)){
        Serial.println("SD Init Error, check wiring or if card installed!");
    }
    Serial.println("SD init success, moving to baro");
    if (!bme.begin()){
        Serial.println("Baro init failed, check wiring or PCB!");
    }
    Serial.println("Baro init success!");
    ground = bme.readAltitude();
    Serial.print("Computer Zeroed! Ground Altitude: ");
    Serial.print(ground);
    Serial.println("m");
    currentTime = millis();
    pinMode(led, OUTPUT);
    pinMode(buzzer, OUTPUT);
    tone(buzzer, 440, 500);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
}



void loop(){
    currentTime = millis();
    altitude = bme.readAltitude() - ground;
    change = altitude - oldAltitude;
    deltaTime = (currentTime - oldTime) / 1000;
    velocity = change / deltaTime;
    if (velocity > 1 || takeoff == true){
        takeoff = true;
        if (currentTime - lastAction >= 1000 && logIndex < 150) {
        altitudeLog[logIndex] = altitude;
        logIndex +=1;
        lastAction = currentTime;
    }
    if ((velocity < 2 && takeoff == true) || apogee == true) {
        File data;
        data = SD.open("flight.txt", FILE_WRITE);
        data.println("Moses R2 Flight Log - Delta Aerospace");
        for (int i = 0; i < logIndex i++ ){
            data.println(altitudeLog[i]);
        }
        data.close();
        while (true) {
            digitalWrite(led, HIGH);
            delay(500);
            digitalWrite(led, LOW);
            tone(buzzer, 440, 500);
        }
        oldTime = currentTime;
        oldAltitude = altitude;

    }
  }
}
    
  
