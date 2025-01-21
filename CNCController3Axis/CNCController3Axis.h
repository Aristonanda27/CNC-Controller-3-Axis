#ifndef CNC_CONTROLLER_3_AXIS_H
#define CNC_CONTROLLER_3_AXIS_H
#include <AccelStepper.h>
// Pin konfigurasi untuk stepper motor sumbu X, Y, dan Z
#define X_STEP_PIN 19
#define X_DIR_PIN 21
#define X_ENABLE_PIN 4 //untuk mengaktifkan motor stepper
#define Y_STEP_PIN 22
#define Y_DIR_PIN 23
#define Y_ENABLE_PIN 4
#define Z_STEP_PIN 25
#define Z_DIR_PIN 26
#define Z_ENABLE_PIN 4
#define STEPS_PER_REV 1600 // Jumlah langkah per revolusi motor
#define SPINDLE_PWM_PIN 18 // Pin PWM untuk spindle

// Definisi Work Coordinate Systems (WCS)
#define WCS_G54 1
#define WCS_G55 2
#define WCS_G56 3
#define WCS_G57 4
#define WCS_G58 5
#define WCS_G59 6
#define BUFFER_SIZE 64

extern float wcsOffsets[6][3];
// // Offset untuk masing-masing WCS
// float wcsOffsets[6][3] = {
//   {0, 0, 0},    // Offset untuk G54 (WCS 1)
//   {100, 100, 0}, // Offset untuk G55 (WCS 2)
//   {200, 200, 0}, // Offset untuk G56 (WCS 3)
//   {300, 300, 0}, // Offset untuk G57 (WCS 4)
//   {400, 400, 0}, // Offset untuk G58 (WCS 5)
//   {500, 500, 0}  // Offset untuk G59 (WCS 6)
// };

class CNCController3Axis {
public:
  CNCController3Axis(int xStepPin, int xDirPin, int yStepPin, int yDirPin, int zStepPin, int zDirPin, int spindlePwmPin);
  void setup();
  void loop();
  void processGcode(const char *gcode);
  void moveTo(float x, float y, float z);
  void linearMoveTo(float x, float y, float z, float feedrate);
  void clockwiseArcMoveTo(float x, float y, float i, float j, float feedrate);
  void counterclockwiseArcMoveTo(float x, float y, float i, float j, float feedrate);
  void setSpindleSpeed(float speed);
  float getValue(const char *gcode, char key);
  long convertToSteps(float value, char axis);
  long convertToSteps(float value);
  void setMaxSpeed(float newSpeed);



private:
  AccelStepper stepperX, stepperY, stepperZ;
  int spindlePin;
  float stepsPerUnit;
   // // Buffer untuk menyimpan G-code dari serial input
  
  char gcodeBuffer[BUFFER_SIZE];
  int gcodeIndex = 0;

  // Mode operasi CNC
  int currentWCS = WCS_G54; // Sistem koordinat aktif
  bool absoluteMode = false;  // Mode posisi absolut atau inkremental
  bool unitsPerMinute = true; // Mode kecepatan: satuan per menit
  bool useInches = true;      // Unit pengukuran: inci (true) atau milimeter (false)
  float toolLengthOffset = 0; // Offset panjang alat
  float toolRadiusCompensation = 0; // Kompensasi radius alat
  bool radiusCompensationLeft = false;  // Kompensasi radius ke kiri
  bool radiusCompensationRight = false; // Kompensasi radius ke kanan




};

#endif
