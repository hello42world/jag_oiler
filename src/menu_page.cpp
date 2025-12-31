#include "menu_page.h"
#include "menu_events.h"
#include "ui/events.h"


#define MAX_DROP_SIZE 10

#define FORM_MAIN_MENU        0
#define FORM_SETTINGS_MENU    1
#define FORM_DROP_SIZE_MENU   3


fds_t fds_data[] = 
MUI_FORM(0)
XMUI_MENU_HEADER("Menu")

MUI_DATA("GP", 
  MUI_10 "Prime pump|"
  MUI_1 "Settings"
)

XMUI_MENU_BTN0("GC") 
XMUI_MENU_BTN1("GC")

//----

MUI_FORM(1)
XMUI_MENU_HEADER("Settings")

MUI_DATA("GP", 
  MUI_3 "Drop size"
)
XMUI_MENU_BTN0("GC")

//XMUI_MENU_BTN1("ID")

//----

// MUI_FORM(2)
// XMUI_MENU_HEADER("Settings")
// MUI_LABEL(3, 31, "Drop size(1-20):")
// MUI_XY("IN", 106, 31)


//----

MUI_FORM(3)
XMUI_MENU_HEADER("Drop size")
MUI_STYLE(2)
MUI_XY("I0", 55, 35)



;  // fds_data



MenuPage::MenuPage(PageManager* pageManager, const Settings& settings) 
  : Page(pageManager)
  , settings_(settings)
  , stDropSize_{&settings_.dropSize, 1, MAX_DROP_SIZE}
{

  muifList_ = {
    MUIF_U8G2_FONT_STYLE(0, XMUI_DEFAULT_FONT),  
    MUIF_U8G2_FONT_STYLE(1, XMUI_HEADER_FONT),
    MUIF_U8G2_FONT_STYLE(2, XMUI_BIG_FONT),

    MUIF_RO("HR", ui::xmui_hrule),
    MUIF_U8G2_LABEL(),
    MUIF_RO("GP", mui_u8g2_goto_data),
    MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

    // XMUIF_DYNAMIC_MENU("ID", settingsMenu_),
    XMUIF_INT_VAR("I0", &stDropSize_),
  };


  mui_.begin(*pageManager->u8g2(), fds_data, muifList_.data(), muifList_.size());
}

bool MenuPage::handleEvent(const Event* event) {
  if (event->id == EventID::PageActivated) {
    mui_.gotoForm(/* form_id= */ 0, /* initial_cursor_position= */ 0);
  }
  else if (event->id == EventID::FullRedraw) {
    draw();    
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
    publishEvent(std::make_unique<ui::FullRedrawEvent>());
  } else {
    return false;
  }
  return true;
}

void MenuPage::draw() {
  mui_.draw();
}

void MenuPage::handleBtnHome() {
  if (mui_.getCurrentFormId() == FORM_MAIN_MENU) {
    publishEvent(std::make_unique<PageClosedEvent>(this));
  } else {
    if (mui_.getCurrentFormId() == FORM_DROP_SIZE_MENU) {
      publishEvent(std::make_unique<SettingsChangedEvent>(settings_));
    }
    mui_.restoreForm();
  }
}

void MenuPage::handleBtnSelect() {
  if (mui_.getCurrentFormId() == FORM_MAIN_MENU && mui_.getCurrentCursorFocusPosition() == 0) {
    publishEvent(std::make_unique<PageClosedEvent>(this, RES_PRIME_PUMP));
    return;
  }
  mui_.sendSelect();
  if (mui_.getCurrentFormId() == FORM_DROP_SIZE_MENU) {
    // Enter edit mode immediately.
    mui_.sendSelect();
  }
}
