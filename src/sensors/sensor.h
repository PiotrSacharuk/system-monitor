#ifndef SENSOR_H
#define SENSOR_H

#include <string>

class Sensor {
  protected:
    std::string name;
    double current_value;
    double threshold;
    Sensor(const std::string& sensor_name, double alert_threshold);

  public:
    virtual ~Sensor() = default;

    virtual void fetch_data() = 0;

    virtual void display_status() const;

    std::string get_name() const { return name; }
    double get_value() const { return current_value; }
    bool is_healthy() const { return current_value < threshold; }
};

#endif