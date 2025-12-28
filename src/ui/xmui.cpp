#include "xmui.h"
#include <cstdarg>
#include <cstdio>

namespace ui {

// String printing buffer and function
char spbuf[64];

std::string sp(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vsnprintf(spbuf, sizeof(spbuf), format, args);
  va_end(args);
  return std::string(spbuf);
}

// Custom horizontal rule drawing
uint8_t xmui_hrule(mui_t *ui, uint8_t msg) {
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg) {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(u8g2, 0, mui_get_y(ui), u8g2_GetDisplayWidth(u8g2));
      break;
  }
  return 0;
}

// XMuiMenu implementation
XMuiMenu::XMuiMenu()
: items{}
, listDef{nullptr, this, getItem, getCnt} {
}

uint16_t XMuiMenu::getCnt(void *data) {
  XMuiMenu *menu = static_cast<XMuiMenu*>(data);
  return menu->items.size();
}

const char* XMuiMenu::getItem(void *data, uint16_t idx) {
  XMuiMenu *menu = static_cast<XMuiMenu*>(data);
  return menu->items[idx].c_str();
}

void xmuiUpdateDisplayArea(U8G2* u8g2, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint16_t x_prime_end = x + w;
    uint16_t y_prime_end = y + h;

    uint8_t x_tile = x / 8;
    uint8_t y_tile = y / 8;

    uint8_t x_tile_end = (x_prime_end + 7) / 8;
    uint8_t y_tile_end = (y_prime_end + 7) / 8;

    uint8_t w_tile = x_tile_end - x_tile;
    uint8_t h_tile = y_tile_end - y_tile;

    u8g2->updateDisplayArea(x_tile, y_tile, w_tile, h_tile);
}

} // namespace ui
