#include <Arduino.h>
#include <OneWire.h>
#include <SingleDS18B20.h>

// Setup an oneWire instance to communicate with the Dallas sensor
OneWire oneWirePin7(7);

// Setup a SingleDS18B20 instance and pass our oneWire reference.
SingleDS18B20 sensor(&oneWirePin7);

//The setup function is called once at startup of the sketch
void setup() {
	// start serial port
	Serial.begin(9600);
	Serial.println("Single DS18B20 Library Demo blocking");
}

// The loop function is called in an endless loop
void loop() {
  // set the resolution
  sensor.setResolution(SingleDS18B20::res12bit);
  
  // start the conversion
  sensor.convert();
  delay(1000);
  // done

  // read the sensor
  sensor.read();

  // output the temperature
	Serial.print("Temperature: ");
	Serial.println(sensor.getTempAsC(), 2);
}