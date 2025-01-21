#include "CNCController3Axis.h"


// Create an instance of CNCController3Axis
CNCController3Axis cncController(X_STEP_PIN, X_DIR_PIN, Y_STEP_PIN, Y_DIR_PIN, Z_STEP_PIN, Z_DIR_PIN, SPINDLE_PWM_PIN);

void setup() {
  // Initialize the CNC Controller
  cncController.setup();
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  // Check if there is a new G-code command from Serial input
  if (Serial.available()) {
    // Read the G-code from Serial input
    String gcode = Serial.readString();
    cncController.processGcode(gcode.c_str());  // Process the G-code command
  }

  // Call the loop method to update motor positions
  cncController.loop();
}
