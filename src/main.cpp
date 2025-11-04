#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ui.h"
#include "motor.h"



/*
muif_t muif_list[] = {  
  MUIF_VARIABLE("BN", NULL, mui_u8g2_btn_exit_wm_fi)
};


fds_t fds_data[] = 
MUI_FORM(1)
MUI_XYT("BN", 64, 30, " Select Me ")
;
*/

UI ui;
Motor motor;


void setup() {
   // WiFi.mode(WIFI_OFF);

  // Initialize serial communication
  Serial.begin(115200);  // Use a standard baud rate

  ui.setup();
//  motor.setup();

#if 0
  pinMode(PIN_BTN_UP, INPUT_PULLUP);

  u8g2.begin(/* menu_select_pin= */ PIN_BTN_SEL, 
    /* menu_next_pin= */ U8X8_PIN_NONE, /* menu_prev_pin= */ U8X8_PIN_NONE, 
    /* menu_up_pin= */ PIN_BTN_UP, /* menu_down_pin= */ PIN_BTN_DOWN, 
    /* menu_home_pin= */ U8X8_PIN_NONE);  
  mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
#endif


}

void mui_loop(void);

const int TURN = 1104; 

static Motor::State g_lastState = Motor::IDLE;
void loop() {
  int8_t event = ui.getButtonEvent();
  if (event != 0) {
    if (event == U8X8_MSG_GPIO_MENU_SELECT) {
      Serial.println("Begin portion");
      motor.beginPortion(TURN / 2);
      g_lastState = motor.getState();
    }
  }

  if (g_lastState == Motor::RUNNING_PORTION) {
    ui.setProgress(motor.getPortionProgress());
    g_lastState = motor.getState();
  }
  
  ui.loop();
  motor.loop();
}


#if 0
uint8_t is_redraw = 1;
void mui_loop(void) {
  u8g2.setFont(u8g2_font_helvR08_tr);
  if ( mui.isFormActive() )
  {
    /* menu is active: draw the menu */
    if ( is_redraw ) {
      u8g2.firstPage();
      do {
          mui.draw();
      } while( u8g2.nextPage() );
      is_redraw = 0;
    }

    /* handle events */
    switch(u8g2.getMenuEvent()) {
      case U8X8_MSG_GPIO_MENU_SELECT:
        Serial.println("U8X8_MSG_GPIO_MENU_SELECT");
        mui.sendSelect();
        is_redraw = 1;
        break;
    }
  } else {  
    /* menu not active: show something else */
    u8g2.firstPage();
    do {
        u8g2.setCursor(0,20);
        u8g2.print(millis());
    } while( u8g2.nextPage() );
  } /* mui.isFormActive() */
}
#endif