#include "Config.h"
#include <cstdlib>
#include <string>

bool Config::isProductionMode() {
    const char* env = std::getenv("PRODUCTION_MODE");
    return env && std::string(env) == "true";
}

int Config::monitoringCycles() {
    if (Config::isProductionMode) return -1;
    return 12; // Test mode
}