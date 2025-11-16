#pragma once
#include "event.h"
#include <U8g2lib.h>

class Page {
public:
  Page(U8G2* u8g2) : u8g2_(u8g2) {}
  
  virtual void loop(const Event* event) = 0;
protected:
  U8G2 *u8g2_;
};