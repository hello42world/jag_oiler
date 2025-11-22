#pragma once

#include <cstdint>

// Initialize ADC for battery voltage reading on GPIO5
// Returns true on success, false on failure
bool batteryInit();

// Read battery voltage in millivolts
// Returns voltage in mV, or -1 on error
int32_t batteryReadVoltage();
