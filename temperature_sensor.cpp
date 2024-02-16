#include <BME280I2C.h>
#include <Wire.h>


BME280I2C bme; 

extern float temp;
extern float hum;
extern float pres;

void setup_temperature_sensor() {
    Wire.begin();

    while (!bme.begin()) {
        Serial.println("Could not find BME280 sensor!");
        delay(200);
    }

    switch (bme.chipModel()) {
        case BME280::ChipModel_BME280:
            Serial.println("Found BME280 sensor! Success.");
            break;
        case BME280::ChipModel_BMP280:
            Serial.println("Found BMP280 sensor! No Humidity available.");
            break;
        default:
            Serial.println("Found UNKNOWN sensor! Error!");
    }
}

void get_temperature() {
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_hPa);

    bme.read(pres, temp, hum, tempUnit, presUnit);

    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print("°C");
    Serial.print("    Humidity: ");
    Serial.print(hum);
    Serial.print("% RH");
    Serial.print("    Pressure: ");
    Serial.print(pres);
    Serial.println(" hPa");
}

