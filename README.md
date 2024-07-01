# Sensor Helper Library

Over many years developing Arduino sketches I've found myself often working on a project that needed to read and act on on the value of various connected sensors. I soon noticed I was writing the same code each time.  My `loop()` function invariably included sections that sampled sensors on a periodic basis, accumulated those values in some way -- often calculating a running average -- and then seperately acted on those values on a different periodic basis, sometimes using the averages to drive other devices and sometimes to report the averages to an external service (e.g. HomeAssistant or an InfluxDB for storage), e.g.:

```C
loop()
{
    ...
    if( (millis() - timeLastSampledMS) > sampleIntervalMS) {
        // Read sensors and compute running averages,
        // perhaps track max and min reported values, etc...
        // Also, reset time of last sample.
    }
    if( (millis() - timeLastReportedMS) > reportIntervalMS) {
        // Use computed values to manage other devices, and/or
        // report averages, max/min, etc. to external services
        // Also, reset average calculation parameters and
        // time of last report.
    }
    ...
}
```
This library is an attempt to streamline those aspects of a sketch through to helpful object classes.

One, the `Measure` object, handles accumulating values read from sensors. Each sampled value can be included in the accumulation, with running maximum value, minimum value, and average automatically determined along with a count of number of values included.  The other, the `SensorManager` object class handles the timing comparisons needed to determine when to sample sensors and also to report values.   Neither class depends on the other, but I've found them very useful together.

# Sensor Helper Object Classes
The **Measure** object class provides the following public functions. Note that all sensor values to be handled are processed as floating point numbers, which may not correspond to values returned from those sensor but should work for all cases and keeps things simple.

`Measure()`
Constructor, which creates an instance of the class and requires no arguments.

`void include(float value)`
Adds numeric `value` to the accumulation.  

`float getMax()`
Returns the maximum value included in the Measure thus far.  Maximum value tracking can be reset with the `clear()` function (see below).

`float getMin()`
Returns the minimum value included in the Measure thus far.  Minimum tracking can be reset with the `clear()` function (see below).

`float getAverage()`
Returns the average of all values included in the Measure thus far.  Average tracking can be reset with either the `resetAverage()` function, which just resets running average calculation, or the `clear()` function (see below).

`float getCurrent()`
Returns the most recent value included in the Measure accumulation.

`float getTotal()`
Returns the running sum of all values included in the Measure thus far.

`uint32_t getCount()`
Returns the number of values included in the Measure since it was created or most recently cleared.

`void resetAverage()`
Resets the running average calculation by clearing the count, total, and average values as well as the most recently included (current) value.

`void clear()`
Clears average, maximum, minimum, total, and count metrics for the Measure, effectively returning it to its state at initial creation.

## SensorManager
The **SensorManager** object class provides the following public functions.

`SensorManager(uint32_t sampleDelayMS, uint32_t reportDelayMS)`
Constructor, which creates an instance that can be used to manage reading  sensor values at the specified delay interval, and reporting them at a (probably different) delay interval. Both delay intervals are specified in milliseconds.

`bool sampleNow(uint32_t currentMS)`
Uses the current time in milliseconds, as returned from the `millis()` function to determine whether it is time to carry out another sampling effort.  The SensorManager object keeps track of the time of last sampling, using it and the sample interval to decide whether it is time to carry out another round of sampling.  Returns `true` if it is time to sample again, and `false` otherwise

`bool reportNow(uint32_t currentMS)`
Uses the current time in milliseconds, as returned from the `millis()` function to determine whether it is time to carry out another reporting effort.  The SensorManager object keeps track of the time of last report, using it and the reporting interval to decide whether it is time to carry out another round of sampling. Returns `true` if it is time to report again, and `false` otherwise.

`void setSampleDelay(uint32_t delay)`
Modifies the sample delay interval, allowing it to be different from what was specified when the SensorManager object was created.  The `delay` specified should be in milliseconds.

`void setReportDelay(uint32_t delay)`
Modifies the reporting delay interval, allowing it to be different from what was specified when the SensorManager object was created.  The `delay` specified should be in milliseconds.

# Examples
See the `examples` folder for application use of these utility object classes in working Arduino sketches.
