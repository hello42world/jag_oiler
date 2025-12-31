#pragma once

#include <cstdint>

class NVStorage {
public:
  NVStorage();
  ~NVStorage();

  bool setup();
  void set(const char* name, int32_t val);
  bool get(const char* name, int32_t* val, int32_t defaultValue);

private:
  void* handle_;
  bool initialized_;
};
