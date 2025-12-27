#include "battery.h"
#include <Arduino.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>

static adc_oneshot_unit_handle_t adc1_handle = nullptr;
static adc_cali_handle_t adc1_cali_handle = nullptr;
static bool initialized = false;

/*
  GPIOX → ADC1_CHANNEL_X 
  X is 0 to 4 
*/

const adc_channel_t BATTERY_ADC_CHANNEL = ADC_CHANNEL_0; // GPIO0 on ESP32-C3


bool batteryInit() {
  // If already initialized, return success
  if (initialized && adc1_handle != nullptr) {
    return true;
  }
  
  // Configure ADC1 for GPIO0 (ADC1_CHANNEL_0 on ESP32-C3)
  adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT_1,
  };
  
  esp_err_t ret = adc_oneshot_new_unit(&init_config, &adc1_handle);
  if (ret != ESP_OK) {
    Serial.printf("ADC unit init failed: 0x%x\n", ret);
    return false;
  }
  
  // Configure channel with ADC_ATTEN_DB_6 for 0-2200mV range (suitable for max 2.1V)
  adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
  };
  
  ret = adc_oneshot_config_channel(adc1_handle, BATTERY_ADC_CHANNEL, &config);
  if (ret != ESP_OK) {
    Serial.printf("ADC channel config failed: 0x%x\n", ret);
    return false;
  }

  // Initialize calibration
  // Try curve fitting first (preferred for ESP32-C3)
  bool calibration_done = false;
  
  adc_cali_curve_fitting_config_t cali_config_curve = {
    .unit_id = ADC_UNIT_1,
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
  };
  
  ret = adc_cali_create_scheme_curve_fitting(&cali_config_curve, &adc1_cali_handle);
  if (ret == ESP_OK) {
    Serial.println("ADC calibration: curve fitting");
    calibration_done = true;
  }

  if (!calibration_done) {
    // Calibration not available, will use raw values
    Serial.println("ADC calibration not available (will use raw values)");
    adc1_cali_handle = nullptr;
  }
  
  // Wait for ADC to stabilize
  delay(10);
  
  // Verify ADC is working by reading a test value
  int testRead = 0;
  ret = adc_oneshot_read(adc1_handle, BATTERY_ADC_CHANNEL, &testRead);
  if (ret != ESP_OK) {
    Serial.printf("ADC test read failed: 0x%x\n", ret);
    return false;
  }


  initialized = true;
  Serial.println("Battery ADC initialized successfully");
  return true;
}

int32_t batteryReadVoltage() {
  if (!initialized || !adc1_handle) {
    return -1;
  }
  
  // Read raw ADC value (0-4095 for 12-bit)
  int adcRaw = 0;
  if (adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &adcRaw) != ESP_OK) {
    return -1;
  }
  
  // Use calibration if available
  if (adc1_cali_handle) {
    int voltage_mv = 0;
    if (adc_cali_raw_to_voltage(adc1_cali_handle, adcRaw, &voltage_mv) == ESP_OK) {
      return (voltage_mv * 200) / 100;
    }
  }
  
  // Fallback: convert raw value to voltage manually
  // ADC_ATTEN_DB_6 gives 0-2200mV range for 12-bit (0-4095)
  // voltage_mv = (adcRaw * 2200) / 4095
  // Then apply voltage divider: * 200 / 100
  int32_t voltage_mv = (adcRaw * 2200) / 4095;
  return (voltage_mv * 200) / 100;
}
