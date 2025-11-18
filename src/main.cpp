#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "app.h"



/*
muif_t muif_list[] = {  
  MUIF_VARIABLE("BN", NULL, mui_u8g2_btn_exit_wm_fi)
};


fds_t fds_data[] = 
MUI_FORM(1)
MUI_XYT("BN", 64, 30, " Select Me ")
;
*/

App app;


void setup() {
   // WiFi.mode(WIFI_OFF);

  // Initialize serial communication
  Serial.begin(115200);  // Use a standard baud rate

  app.setup();
}

void mui_loop(void);


void loop() {
  app.loop();
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