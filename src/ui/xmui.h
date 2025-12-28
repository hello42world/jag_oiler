#pragma once

#include <U8g2lib.h>
#include <MUIU8g2.h>


#include <vector>
#include <string>
#include <cstdint>


// Menu header macro
#define XMUI_MENU_HEADER(caption) \
  MUI_STYLE(1) \
  MUI_LABEL(5,10, caption) \
  MUI_XY("HR", 0, 13) \
  MUI_STYLE(0)

// Menu button position macros
#define XMUI_MENU_BTN0(id) MUI_XYA(id, 5, 25, 0)
#define XMUI_MENU_BTN1(id) MUI_XYA(id, 5, 37, 1)

#define XMUIF_DYNAMIC_MENU(id, menu) \
  { id[0], id[1], MUIF_CFLAG_IS_CURSOR_SELECTABLE, 0, &menu.listDef, mui_u8g2_u16_list_goto_w1_pi }

#define XMUIF_INT_VAR(id, varstruct) \
  {id[0], id[1], MUIF_CFLAG_IS_CURSOR_SELECTABLE, 0, varstruct, mui_u8g2_u8_min_max_wm_mud_pi}

namespace ui {

// Custom horizontal rule function
uint8_t xmui_hrule(mui_t *ui, uint8_t msg);

// String printing helper
std::string sp(const char* format, ...);

// Update display area (partial refresh)
void xmuiUpdateDisplayArea(U8G2* u8g2, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

// Dynamic menu structure
struct XMuiMenu {
  XMuiMenu();
  std::vector<std::string> items;
  mui_u8g2_list_struct listDef;
  
  static uint16_t getCnt(void *data);
  static const char* getItem(void *data, uint16_t idx);
};

} // namespace ui
