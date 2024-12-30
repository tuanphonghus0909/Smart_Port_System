#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <HX711.h>
#include <Ticker.h>
#include "secrets/wifi.h"
#include "wifi_connect.h"
#include "ca_cert.h"
#include "MQTT.h"
#include "secrets/mqtt.h"

#define WATER_SENSOR_PIN 36U
#define RGB_LED_RED 32U
#define RGB_LED_GREEN 25U
#define RGB_LED_BLUE 26U
#define SERVO_PIN 33
#define SERVO_IN_PIN 12
#define SERVO_OUT_PIN 13
#define TCRT5000_IN_PIN 34U
#define TCRT5000_OUT_PIN 39U
#define TRIG1_PIN 5U
#define ECHO1_PIN 18U
#define TRIG2_PIN 16U
#define ECHO2_PIN 17U
#define TRIG3_PIN 14U
#define ECHO3_PIN 27U
#define HX711_DOUT 4U
#define HX711_SCK 15U

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MAX_VALUE 2500.0

TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_2);
WiFiClientSecure tlsClient;
PubSubClient mqttClient(tlsClient);
Servo servo; 
Servo servoIn;
Servo servoOut;
HX711 scale;
Ticker waterLevelTicker, parkingTicker, weightTicker, irTicker;

const char *water_status_topic = "esp32/water_status";
const char *water_status1_topic = "esp32/water_status1";
const char *weight_status_topic = "esp32/weight_status";
const char *parking_status_topic = "esp32/parking_status";
const char *servo_control_topic = "esp32/servo_control";
const char *reset_counts_topic = "esp32/reset_counts";
const char *cars_in_topic = "esp32/cars_in";
const char *cars_out_topic = "esp32/cars_out";
const char *subscribe_topics[] = {reset_counts_topic, servo_control_topic};

int parkingSlots = 3; 
float weight = 0;
int carsInToday = 0; 
int carsOutToday = 0;
const float CALIBRATION_FACTOR = -1023.36; 
bool isIRDetecting = false;
unsigned long irDetectStart = 0;
unsigned long analogDetectStart = 0; // IR sử dụng AO
bool isAnalogDetecting = false; // IR sử dụng AO

void setRGBColor(uint8_t red, uint8_t green, uint8_t blue) {
    analogWrite(RGB_LED_RED, red);
    analogWrite(RGB_LED_GREEN, green);
    analogWrite(RGB_LED_BLUE, blue);
}

long measureDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}

void checkWeight() {
    if (scale.is_ready()) {
        weight = scale.get_units(5); 
        char weightStr[10]; 
        dtostrf(weight, 6, 1, weightStr);
        if (weight > 30) {
            setRGBColor(0, 255, 255);
        } else {
            setRGBColor(255, 0, 255);
        }
        mqttClient.publish(weight_status_topic, String(weight).c_str(), false);

        display2.clearDisplay();
        display2.setTextSize(2);
        display2.setCursor((SCREEN_WIDTH - 6 * 2 * 3) / 2, (SCREEN_HEIGHT - 16) / 2);
        display2.print(weight, 1);
        display2.setTextSize(1);
        display2.setCursor((SCREEN_WIDTH - 6 * 1 * 3) / 2, (SCREEN_HEIGHT - 16) / 2 + 16);
        
        if (weight == 1) {
            display2.print(" ton");
        } else {
            display2.print(" tons");
        }
        
        display2.display();
    }
}

void checkWaterLevel() {
    int waterLevel = analogRead(WATER_SENSOR_PIN);
    int valueWL = (int)((float)waterLevel / MAX_VALUE * 100);
    
    mqttClient.publish(water_status_topic, String(valueWL).c_str(), false);
    mqttClient.publish(water_status1_topic, String(waterLevel).c_str(), false);
}

void checkParking() {
    int occupiedSlots = 0;

    for (int i = 0; i < 3; i++) {
        int trigPin = (i == 0) ? TRIG1_PIN : (i == 1) ? TRIG2_PIN : TRIG3_PIN;
        int echoPin = (i == 0) ? ECHO1_PIN : (i == 1) ? ECHO2_PIN : ECHO3_PIN;

        long distance = measureDistance(trigPin, echoPin);

        if (distance >= 2 && distance <= 5) {
            occupiedSlots++;
        }
    }

    parkingSlots = 3 - occupiedSlots;

    mqttClient.publish(parking_status_topic, String(parkingSlots).c_str(), false);

    display1.clearDisplay();
    display1.setTextSize(2);
    display1.setCursor((SCREEN_WIDTH - 6 * 2 * 3) / 2, (SCREEN_HEIGHT - 16) / 2);
    display1.print(parkingSlots);  
    display1.setTextSize(1);
    display1.setCursor((SCREEN_WIDTH - 6 * 1 * 6) / 2, (SCREEN_HEIGHT - 16) / 2 + 16);
    if (parkingSlots == 1) {
        display1.print(" slot"); 
    } else {
        display1.print(" slots"); 
    }
    display1.display();
}

// void checkIRSensor() {
//     bool carDetectedIn = digitalRead(TCRT5000_IN_PIN) == LOW;
//     bool carDetectedOut = digitalRead(TCRT5000_OUT_PIN) == LOW;
    
//     if (carDetectedIn) {
//         if (!isIRDetecting) {
//             irDetectStart = millis();
//             isIRDetecting = true;
//         } else if (millis() - irDetectStart >= 2000) {
//             carsInToday++; 
//             mqttClient.publish(cars_in_topic, String(carsInToday).c_str(), false);
//             isIRDetecting = false; 
//         }
//     } else {
//         isIRDetecting = false;
//     }

//     if (carDetectedOut) {
//         if (!isIRDetecting) {
//             irDetectStart = millis();
//             isIRDetecting = true;
//         } else if (millis() - irDetectStart >= 2000) {
//             carsOutToday++; 
//             mqttClient.publish(cars_out_topic, String(carsOutToday).c_str(), false);
//             isIRDetecting = false;
//         }
//     } else {
//         isIRDetecting = false;
//     }
// }

void checkTRAnalogSensor() {
    int carDetectedIn = analogRead(TCRT5000_IN_PIN) ;
    int carDetectedOut = analogRead(TCRT5000_OUT_PIN) ;

    if (carDetectedIn < 4095) {
        if (!isAnalogDetecting) {
            analogDetectStart = millis();
            isAnalogDetecting = true;
        } else if (millis() - analogDetectStart >= 2000) {
            
            carsInToday++; 
            mqttClient.publish(cars_in_topic, String(carsInToday).c_str(), false);
            isAnalogDetecting = false; 
        }
    } else {
        isAnalogDetecting = false; 
    }

    if (carDetectedOut < 4095) {
        if (!isAnalogDetecting) {
            analogDetectStart = millis();
            isAnalogDetecting = true;
        } else if (millis() - analogDetectStart >= 2000) {
            
            carsInToday++; 
            mqttClient.publish(cars_in_topic, String(carsInToday).c_str(), false);
            isAnalogDetecting = false; 
        }
    } else {
        isAnalogDetecting = false; 
    }
}

void mqttCallback(char *topic, uint8_t *payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    if (strcmp(topic, servo_control_topic) == 0) {
        if (strcmp(message, "Open In") == 0) {
            servoIn.write(90);
            Serial.println("Servo In: Opened");
        } else if (strcmp(message, "Close In") == 0) {
            servoIn.write(0);
            Serial.println("Servo In: Closed");
        } else if (strcmp(message, "Open Out") == 0) {
            servoOut.write(90);
            Serial.println("Servo Out: Opened");
        } else if (strcmp(message, "Close Out") == 0) {
            servoOut.write(0);
            Serial.println("Servo Out: Closed");
        }else if (strcmp(message, "Permission") == 0) {
            servo.write(180); 
            Serial.println("Servo: Permission granted, rotating to 180 degrees");
        }else if (strcmp(message, "Stop") == 0) {
            servo.write(0); 
            Serial.println("Servo: Stopped, returning to 0 degrees");
        }
    }else if (strcmp(topic, reset_counts_topic) == 0) { 
        if (strcmp(message, "RESET") == 0) {
            carsInToday = 0;
            carsOutToday = 0;
            //mqttClient.publish("esp32/confirmation", "Counts reset", false);
        }
    }
}

void setup() {
    Serial.begin(115200);
    analogSetAttenuation(ADC_11db);

    setup_wifi(WiFiSecrets::ssid, WiFiSecrets::pass);
    tlsClient.setCACert(ca_cert);
    mqttClient.setServer(HiveMQ::broker, HiveMQ::port);
    mqttClient.setCallback(mqttCallback);

    pinMode(WATER_SENSOR_PIN, INPUT);
    pinMode(RGB_LED_RED, OUTPUT);
    pinMode(RGB_LED_GREEN, OUTPUT);
    pinMode(RGB_LED_BLUE, OUTPUT);
    pinMode(TCRT5000_IN_PIN, INPUT);
    pinMode(TCRT5000_OUT_PIN, INPUT);
    pinMode(TRIG1_PIN, OUTPUT);
    pinMode(ECHO1_PIN, INPUT);
    pinMode(TRIG2_PIN, OUTPUT);
    pinMode(ECHO2_PIN, INPUT);
    pinMode(TRIG3_PIN, OUTPUT);
    pinMode(ECHO3_PIN, INPUT);

    servo.attach(SERVO_PIN);
    servoIn.attach(SERVO_IN_PIN);
    servoOut.attach(SERVO_OUT_PIN);
    scale.begin(HX711_DOUT, HX711_SCK);

    scale.set_scale(CALIBRATION_FACTOR);
    scale.tare(); 

    I2C_1.begin(21, 22);
    I2C_2.begin(19, 23);
    if (!display1.begin(SSD1306_PAGEADDR, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed for display 1"));
        for (;;);
    }
    if (!display2.begin(SSD1306_PAGEADDR, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed for display 2"));
        for (;;);
    }

    waterLevelTicker.attach(1.0, checkWaterLevel);
    parkingTicker.attach(1.0, checkParking);
    weightTicker.attach(1.0,checkWeight);
    irTicker.attach(0.2,checkTRAnalogSensor);
    //irTicker.attach(0.2,checkIRSensor);
}

void loop() {
    MQTT::reconnect(mqttClient, WiFi.macAddress().c_str(), HiveMQ::username, HiveMQ::password, subscribe_topics, 2);
    mqttClient.loop();
}