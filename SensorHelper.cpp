#include "Arduino.h"
#include "SensorHelper.h"

// Simple utility class to manage sensor readings and other environmental values that are
// gathered and monitored over time

Measure::Measure()
{
    _value = _total = _maxvalue = _minvalue = _average = 0.0;
    _count = 0;
    _new_min_max = true;
}

void Measure::include(float value)
{
  _count++; // started with 0
  _value = value;
  _total += value;
  if(_new_min_max == true) {
    _maxvalue = _minvalue = value;
    _new_min_max = false;
  }
  else {
    if(value > _maxvalue) _maxvalue = value;
    if(value < _minvalue) _minvalue = value;  
  }
  _average = _total / _count;
}

// clear() completely zeroes everything, including the min and max values
void Measure::clear()
{
    _value = _total = _average = 0;
    _count = 0;
    _maxvalue = _minvalue = 0.0;
    _new_min_max = true;
}

// resetAvg() clears the value, total, count and average but leaves the min
// and max values unmodified.  Use resetAvg() to reset the cumulative averaging
// behavior, e.g., to begin a new sampling interval, but leave the 
// longer term observed max/min values alone.
void Measure::resetAverage()
{
    _value = _total = _average = 0.0;
    _count = 0;
}

uint32_t Measure::getCount()
{
  return _count;
}

float Measure::getTotal()
{
  return _total;
}

float Measure::getMax()
{
  return _maxvalue;
}

float Measure::getMin()
{
  return _minvalue;
}

float Measure::getAverage()
{
  return _average;
}

float Measure::getCurrent()
{
  return _value;
}

void Measure::printMeasure()
{
  Serial.print("[#");
  Serial.print(_count);
  Serial.print("] ");
  Serial.print(_value,2);
  Serial.print(" (");
  Serial.print(_minvalue,2);
  Serial.print(",");
  Serial.print(_average,2);
  Serial.print(",");
  Serial.print(_maxvalue,2);
  Serial.println(")");
}


/*
 * Simple utility class to streamling handling timing of sensor interactions, allowing
 * sampling values on a pre-defined interval and separately reporting values (perhaps 
 * averaged across samples or otherwise processed as a collection). 
 */

 // Requires specifying the sample and reporting delays as part of constructing the object
SensorManager::SensorManager(uint32_t sampleDelayMS, uint32_t reportDelayMS)
{
  _lastReportMS = 0;
  _lastSampleMS = 0;
  _reportDelayMS = reportDelayMS;
  _sampleDelayMS = sampleDelayMS;
}

// Allows altering the reporting interval
void SensorManager::setReportDelay(uint32_t delay)
{
  _reportDelayMS = delay;
}

// Allows altering the sampling interval
void SensorManager::setSampleDelay(uint32_t delay)
{
  _sampleDelayMS = delay;
}

// Checks to see if it is time to sample the sensor. Returns TRUE if the specified
// sampling interval has passed, resetting the sample timer in the process, and FALSE if not.  
bool SensorManager::sampleNow(uint32_t currentMS) {
  if( (currentMS - _lastSampleMS) > _sampleDelayMS) {
    _lastSampleMS = currentMS;
    return(true);
  }
  else {
    return(false);
  }
}

// Checks to see if it is time to report values separately from sampling, allowing 
// operations across multiple samples (e.g., averaging). Returns TRUE if the specified
// reporting interval has passed, resetting the reporting timer in the process, and FALSE if not. 
bool SensorManager::reportNow(uint32_t currentMS)
{
  if( (currentMS - _lastReportMS) > _reportDelayMS) {
    _lastReportMS = currentMS;
    return(true);
  }
  else {
    return(false);
  }
}