#include "nv_storage.h"
#include <nvs_flash.h>
#include <nvs.h>
#include <Arduino.h>

NVStorage::NVStorage() 
  : handle_(nullptr)
  , initialized_(false) {
}

NVStorage::~NVStorage() {
  if (handle_) {
    nvs_close(static_cast<nvs_handle_t>(reinterpret_cast<uintptr_t>(handle_)));
  }
}

bool NVStorage::setup() {
  if (initialized_) {
    return true;
  }

  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  
  if (err != ESP_OK) {
    Serial.printf("NVStorage: Failed to initialize NVS: %s\n", esp_err_to_name(err));
    return false;
  }

  // Open NVS namespace
  nvs_handle_t handle;
  err = nvs_open("storage", NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    Serial.printf("NVStorage: Failed to open NVS namespace: %s\n", esp_err_to_name(err));
    return false;
  }

  handle_ = reinterpret_cast<void*>(static_cast<uintptr_t>(handle));
  initialized_ = true;
  return true;
}

void NVStorage::set(const char* name, int32_t val) {
  if (!initialized_) {
    Serial.println("NVStorage: Not initialized, cannot set value");
    return;
  }

  nvs_handle_t handle = static_cast<nvs_handle_t>(reinterpret_cast<uintptr_t>(handle_));
  esp_err_t err = nvs_set_i32(handle, name, val);
  
  if (err != ESP_OK) {
    Serial.printf("NVStorage: Failed to set '%s': %s\n", name, esp_err_to_name(err));
    return;
  }

  // Commit written value
  err = nvs_commit(handle);
  if (err != ESP_OK) {
    Serial.printf("NVStorage: Failed to commit '%s': %s\n", name, esp_err_to_name(err));
  }
}

bool NVStorage::get(const char* name, int32_t* val, int32_t defaultValue) {
  if (!initialized_) {
    Serial.println("NVStorage: Not initialized, returning default value");
    *val = defaultValue;
    return false;
  }

  nvs_handle_t handle = static_cast<nvs_handle_t>(reinterpret_cast<uintptr_t>(handle_));
  int32_t value;
  esp_err_t err = nvs_get_i32(handle, name, &value);
  
  if (err == ESP_ERR_NVS_NOT_FOUND) {
    *val = defaultValue;
    return false;
  } else if (err != ESP_OK) {
    Serial.printf("NVStorage: Failed to get '%s': %s, using default value %d\n", 
                  name, esp_err_to_name(err), defaultValue);
    *val = defaultValue;
    return false;
  }

  *val = value;
  return true;
}
