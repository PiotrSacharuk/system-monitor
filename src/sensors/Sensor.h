#ifndef SENSOR_H
#define SENSOR_H

#include <string>

class Sensor {
protected:
    std::string name;
    double currentValue;
    double threshold;
    Sensor(const std::string& sensorName, double alertThreshold);

public:
    virtual ~Sensor() = default;

    virtual void fetchData() = 0;
    
    virtual void displayStatus() const;

    std::string getName() const { return name; }
    double getValue() const { return currentValue; }
    bool isHealthy() const { return currentValue < threshold; }
};

#endif