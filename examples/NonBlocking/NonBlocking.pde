#include <Arduino.h>
#include <OneWire.h>
#include <SingleDS18B20.h>

// Setup an oneWire instance to communicate with the Dallas sensor
OneWire oneWirePin7(7);

// Setup a SingleDS18B20 instance and pass our oneWire reference.

SingleDS18B20 sensor(&oneWirePin7);

// Declare variables

unsigned long LastSensorUpdate;
bool sensorOk;
int16_t tempRaw;
float tempC;

// Helper function to call a function at a certain interval

void doFunctionAtInterval(void (*callBackFunction)(), unsigned long *lastEvent,
		unsigned long Interval) {
	unsigned long now = millis();
	if ((now - *lastEvent) >= Interval) {
		*lastEvent = now;
		callBackFunction();
	}
}

// Helper function to initialize the sensor and start the conversion
// Useful when multiple sensors are used

bool initDS18B20(SingleDS18B20* sensor) {
	if (sensor->setResolution(SingleDS18B20::res12bit)) {
		return sensor->convert();
	} else
		return false;
}

//The setup function is called once at startup of the sketch
void setup() {
	// start serial port
	Serial.begin(9600);
	Serial.println(F("Single DS18B20 Library Demo non blocking"));
	//initialize the sensor
	sensorOk = initDS18B20(&sensor);
	LastSensorUpdate = millis();
	delay(1000);
}

// The loop function is called in an endless loop
void loop() {
	doFunctionAtInterval(readSensor, &LastSensorUpdate, 1000);
}

void readSensor() {
	if (sensorOk && sensor.read()) {
		tempRaw = sensor.getTempAsRaw();
		tempC = sensor.getTempAsC();
		sensorOk = sensor.convert();
	} else {
		sensorOk = initDS18B20(&sensor); // Oops, we lost a sensor, try to reconnect
	}

// output the values

	if (sensorOk) {
		Serial.print(F("Raw value  : "));
		Serial.println(tempRaw, DEC);
		Serial.print(F("temperature: "));
		Serial.println(tempC, 4);
	} else {
		Serial.println(F("No sensor"));
	}
}

