#ifndef SENSORHELPER_H
#define SENSORHELPER_H

#include "Arduino.h"

class Measure {
  public:
    Measure();
    void include(float value);
    void clear();
    void resetAverage();
    uint32_t getCount();
    float getMax();
    float getMin();
    float getAverage();
    float getCurrent();
    float getTotal();
    void printMeasure();
  private:
    float _value;
    uint32_t _count;
    float _total;
    float _maxvalue;
    float _minvalue;
    float _average;
    bool _new_min_max; 
};

class SensorManager {
  public:
    SensorManager(uint32_t sampleDelayMS, uint32_t reportDelayMS);
    void setReportDelay(uint32_t delay);
    void setSampleDelay(uint32_t delay);
    bool sampleNow(uint32_t currentMS);
    bool reportNow(uint32_t currentMS);
  private:
    uint32_t _reportDelayMS;
    uint32_t _sampleDelayMS;
    uint32_t _lastReportMS;
    uint32_t _lastSampleMS;
};






#endif // SENSORHELPER_H