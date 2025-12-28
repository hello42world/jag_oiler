#include "menu_page.h"
#include "menu_events.h"
#include "ui/events.h"


#define MAX_DROP_SIZE 20

fds_t fds_data[] = 
MUI_FORM(0)
XMUI_MENU_HEADER("Menu")

MUI_DATA("GP", 
  MUI_10 "Prime pump|"
  MUI_2 "Settings"
)

XMUI_MENU_BTN0("GC") 
XMUI_MENU_BTN1("GC")

//----

//MUI_FORM(1)
//XMUI_MENU_HEADER("Settings")
//XMUI_MENU_BTN0("ID")
//XMUI_MENU_BTN1("ID")

//----

MUI_FORM(2)
XMUI_MENU_HEADER("Settings")
MUI_LABEL(5, 27, "Drop size (1-20):")
MUI_XY("IN", 100, 27)
// MUI_XYT("LV", 64, 59, " OK ")

;



MenuPage::MenuPage(U8G2* u8g2, EventBus* eventBus, const Settings& settings) 
  : Page(u8g2, eventBus)
  , settings_(settings)
  , stDropSize_{&settings_.dropSize, 1, MAX_DROP_SIZE}
   {

  muifList_ = {
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),  
    MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),

    MUIF_RO("HR", ui::xmui_hrule),
    MUIF_U8G2_LABEL(),
    MUIF_RO("GP",mui_u8g2_goto_data),
    MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

    // XMUIF_DYNAMIC_MENU("ID", settingsMenu_),

    XMUIF_INT_VAR("IN", &stDropSize_),
  };


  mui_.begin(*u8g2, fds_data, muifList_.data(), muifList_.size());
  mui_.gotoForm(/* form_id= */ 0, /* initial_cursor_position= */ 0);
}

bool MenuPage::handleEvent(const Event* event) {
  if (event->id == EventID::FullRedraw) {
    muiRedraw();    
  } else if (event->id == EventID::Button) {
    const ui::ButtonEvent* buttonEvent = static_cast<const ui::ButtonEvent*>(event);
    switch (buttonEvent->button) {
      case U8X8_MSG_GPIO_MENU_SELECT:
        handleBtnSelect();
        break;
      case U8X8_MSG_GPIO_MENU_NEXT:
        mui_.nextField();
        break;
      case U8X8_MSG_GPIO_MENU_PREV:
        mui_.prevField();
        break;
      case U8X8_MSG_GPIO_MENU_HOME:
        handleBtnHome();
        break;
    }
    muiRedraw();
  } else {
    return false;
  }
  return true;
}

void MenuPage::muiRedraw() {
  u8g2_->firstPage();
  do {
    mui_.draw();
  } while( u8g2_->nextPage() );
}

void MenuPage::handleBtnHome() {
  if (mui_.getCurrentFormId() == 0) {
    publishEvent(std::make_unique<PageClosedEvent>(this));
  } else {
    if (mui_.getCurrentFormId() == 2) {
      publishEvent(std::make_unique<SettingsChangedEvent>(settings_));
    }
    mui_.restoreForm();
  }
}

void MenuPage::handleBtnSelect() {
  if (mui_.getCurrentFormId() == 0 && mui_.getCurrentCursorFocusPosition() == 0) {
    publishEvent(std::make_unique<PageClosedEvent>(this, RES_PRIME_PUMP));
    return;
  }
  mui_.sendSelect();
}
