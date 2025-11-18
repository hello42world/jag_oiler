#include "menu_page.h"
#include "ui/events.h"


uint8_t number_input = 2;       // variable where the user can input a number between 0 and 9
uint8_t exit_code = 0;   

muif_t muif_list[] = {
  /* normal text style */
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),
  
  /* Leave the menu system */
  MUIF_VARIABLE("LV",&exit_code, mui_u8g2_btn_exit_wm_fi),
  
  /* input for a number between 0 to 9 */
  MUIF_U8G2_U8_MIN_MAX("IN", &number_input, 0, 9, mui_u8g2_u8_min_max_wm_mse_pi),
  
  /* MUI_LABEL is used to place fixed text on the screeen */
  MUIF_LABEL(mui_u8g2_draw_text)
};


fds_t fds_data[] MUI_PROGMEM = 
  MUI_FORM(1)
  MUI_STYLE(0)
  MUI_LABEL(5,12, "Countdown Time")
  MUI_LABEL(5,30, "Seconds:")
  MUI_XY("IN",60, 30)
  MUI_XYT("LV",64, 59, " OK ")
;


MenuPage::MenuPage(U8G2* u8g2) 
  : Page(u8g2) {
  mui_.begin(*u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
}

void MenuPage::loop(const Event* event) {
  if (event->id == EventID::FullRedraw) {
    muiRedraw();    
  } else if (event->id == EventID::Button) {
    const ui::ButtonEvent* buttonEvent = static_cast<const ui::ButtonEvent*>(event);
    switch (buttonEvent->button) {
      case U8X8_MSG_GPIO_MENU_SELECT:
        mui_.sendSelect();
        break;
      case U8X8_MSG_GPIO_MENU_NEXT:
        mui_.nextField();
        break;
      case U8X8_MSG_GPIO_MENU_PREV:
        mui_.prevField();
        break;
      default:
        break;
    }
    muiRedraw();
  }
}

void MenuPage::muiRedraw() {
  u8g2_->firstPage();
  do {
    mui_.draw();
  } while( u8g2_->nextPage() );
}
