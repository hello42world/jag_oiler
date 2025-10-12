#include "motor.h" 

 /**
 * Author Teemu Mäntykallio
 * Initializes the library and runs the stepper
 * motor in alternating directions.
 */

//#include <TMCStepper.h>
//#include <SoftwareSerial.h>

/* ESP8266 Pin configuration
//#define EN_PIN           D2 // Enable
#define STEP_PIN         D1 // Step
#define SW_RX     D7     // Software serial RX (connects to TMC2209 PDN_UART)
#define SW_TX     D8      // Software serial TX (connects to TMC2209 PDN_UART via 1kΩ)
*/ 

#define STEP_PIN 0
#define EN_PIN  1

#define SW_RX     3
#define SW_TX     4


#define R_SENSE   0.11f   // Sense resistor on the TMC2209 module (e.g. 0.11Ω)
#define DRIVER_ADDRESS 0b00  // TMC2209 UART address (00 by default for standalone)


// SoftwareSerial swSerial(SW_RX, SW_TX); // Software serial for TMC2209 UART communication

// TMC2209Stepper driver(SW_RX, SW_TX, R_SENSE, DRIVER_ADDRESS); 
// TMC2209Stepper driver(&swSerial, R_SENSE, DRIVER_ADDRESS); 
//TMC2209Stepper driver(&Serial, R_SENSE, DRIVER_ADDRESS);

uint8_t motor_setup() {
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  digitalWrite(EN_PIN, LOW);     
/*
//  swSerial.begin(115200); // Initialize software serial for TMC2209
  // Initialize TMC2209 UART communication
  driver.beginSerial(115200);   // Initialize software serial at 115200 baud for TMC2209
  // driver.begin();               // Initialize driver (loads configuration)

  // Configure TMC2209 settings via UART
  driver.toff(5);              // Enable driver by setting toff > 0 (turns on power stages):contentReference[oaicite:16]{index=16}
  driver.rms_current(150);     // Set motor RMS current to 150 mA (limit for your 200 mA motor):contentReference[oaicite:17]{index=17}
  driver.microsteps(16);       // Set 16 microstep mode (will interpolate to 256 internally)
  driver.en_spreadCycle(false);// Use StealthChop (quiet mode) instead of SpreadCycle:contentReference[oaicite:18]{index=18}
  driver.pwm_autoscale(true);  // Enable automatic current scaling (required for StealthChop)

  Serial.printf("TMC2209 version: %d\n", driver.version());

  return driver.test_connection();
*/
  return 0;
}

bool shaft = false;


void motor_loop() {
 for(int i=0;i<1000;i++){
   digitalWrite(STEP_PIN, HIGH);
   delayMicroseconds(500);  
   digitalWrite(STEP_PIN, LOW);
   delayMicroseconds(500);  
 }
}

