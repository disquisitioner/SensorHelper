#include "SensorHelper.h"

// Define the delay intervals to be used for sampling the
// sensors and reporting their processed values. Note that
// both are in milliseconds.
const uint32_t SampleDelayMS = 10000;
const uint32_t ReportDelayMS = 60000;

// Create a Measure object for handling ongoing sensor readings.
// In this example we're simulating a temperature sensor
Measure Temperature;

// Create a SensorManager object to handle sensor sample and
// report timing, using the specified sample and report delays.
SensorManager sensors(SampleDelayMS, ReportDelayMS);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Sensor Helper example");

  // Seed the Arduino random number generator so we can
  // simulate having sensors attached.
  randomSeed(analogRead(0));
}

void loop() {
  float temperatureF;
  uint32_t currentMillis = millis();

  // If it's time to sample, do so
  if(sensors.sampleNow(currentMillis)) {
    // Put logic here to read whatever sensors are in use

    // Read the (simulated) temperature sensor
    temperatureF = simulatedTemperatureSensor();
    // Accumulate this reading in our Measure object to simplify
    // averaging, max & min tracking, etc.
    Temperature.include(temperatureF); 
    Serial.print("Sampling -- Sensor reading: ");
    Serial.print(temperatureF);
    Serial.println("F");
  }

  // If it's time to report, do so
  if(sensors.reportNow(currentMillis)){
    // Put logic here to report processed sensor readings to some host
    // (or store them in a database, etc.)

    // As an example, just print various measured information
    Serial.print("Reporting -- Average Temperature: ");
    Serial.print(Temperature.getAverage(),2);  // Average temperature since last report
    Serial.print(", Max: ");
    Serial.print(Temperature.getMax(),2);
    Serial.print(", Min: ");
    Serial.println(Temperature.getMin(),2);

    // Reset temperature sample averaging (but not min & max tracking). This produces
    // an average that's just across values sampled since the last report.  Don't reset 
    // the average if it should be across all sampled values.
    Temperature.resetAverage();

    // To reset max and min calculating (as well as average calculation) use
    // a Measure object's clear() function, e.g. Temperature.clear();
  }
  delay(1000);  // Simulate other things going on in the overall application...
}

// Simulate a temperature sensor through reasonable randomly generated values.
// Returns temperature in degrees Fahrenheit
float simulatedTemperatureSensor() {
  return(40.0 + random(0,650)/10.0);
}
