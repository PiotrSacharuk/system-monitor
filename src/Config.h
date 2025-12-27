#ifndef CONFIG_H
#define CONFIG_H

class Config {
public:
    static bool isProductionMode();
    static int monitoringCycles();
};

#endif