#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "CNCController3Axis.h"

// Offset untuk masing-masing WCS
float wcsOffsets[6][3] = {
  {0, 0, 0},    // Offset untuk G54 (WCS 1)
  {100, 100, 0}, // Offset untuk G55 (WCS 2)
  {200, 200, 0}, // Offset untuk G56 (WCS 3)
  {300, 300, 0}, // Offset untuk G57 (WCS 4)
  {400, 400, 0}, // Offset untuk G58 (WCS 5)
  {500, 500, 0}  // Offset untuk G59 (WCS 6)
};

CNCController3Axis::CNCController3Axis(int xStepPin, int xDirPin, int yStepPin, int yDirPin, int zStepPin, int zDirPin, int spindlePwmPin)
   : stepperX(AccelStepper::FULL2WIRE, xStepPin, xDirPin),
     stepperY(AccelStepper::FULL2WIRE, yStepPin, yDirPin),
     stepperZ(AccelStepper::FULL2WIRE, zStepPin, zDirPin),
     spindlePin(spindlePwmPin) {
}

void CNCController3Axis::setup() {
   stepperX.setMaxSpeed(1000);
   stepperX.setAcceleration(500);
   stepperY.setMaxSpeed(1000);
   stepperY.setAcceleration(500);
   stepperZ.setMaxSpeed(1000);
   stepperZ.setAcceleration(500);
   
   pinMode(spindlePin, OUTPUT);
}

void CNCController3Axis::loop() {
   stepperX.run();
   stepperY.run();
   stepperZ.run();
}

void CNCController3Axis::setMaxSpeed(float newSpeed) {
    stepperX.setMaxSpeed(newSpeed);
    stepperY.setMaxSpeed(newSpeed);
    stepperZ.setMaxSpeed(newSpeed);
}

// Mengubah nilai dalam satuan panjang menjadi langkah motor berdasarkan sumbu yang diberikan
long CNCController3Axis::convertToSteps(float value, char axis)
{
  float offset = 0;
  if (axis == 'X')
    offset = wcsOffsets[currentWCS - 1][0];
  else if (axis == 'Y')
    offset = wcsOffsets[currentWCS - 1][1];
  else if (axis == 'Z')
    offset = wcsOffsets[currentWCS - 1][2];

  return long(((useInches ? value * 25.4 : value) + offset) * stepsPerUnit);
}

//Konversi nilai panjang ke jumlah langkah motor
long CNCController3Axis::convertToSteps(float value)
{
  return long((useInches ? value * 25.4 : value) * stepsPerUnit);
}

void CNCController3Axis::processGcode(const char *gcode) {
 Serial.print("");
 Serial.println(gcode);

 if (strncmp(gcode, "G90", 3) == 0)
 {
   absoluteMode = false;
   Serial.println("Absolute Positioning.");
 }
 else if (strncmp(gcode, "G91", 3) == 0)
 {
   absoluteMode = true;
   Serial.println("Incremental Positioning.");
 }
 else if (strncmp(gcode, "G94", 3) == 0)
 {
   unitsPerMinute = true;
   Serial.println("feedrate per Minute.");
 }
 else if (strncmp(gcode, "G20", 3) == 0)
 {
   useInches = false;
   stepsPerUnit = STEPS_PER_REV / 0.0787401575; 
   Serial.println("Set Units to Inches.");
 }
 else if (strncmp(gcode, "G21", 3) == 0)
 {
   useInches = true;
   stepsPerUnit = STEPS_PER_REV / 50.8; 
   Serial.println("Set Units to Millimeters.");
 }
 else if (strncmp(gcode, "G28", 3) == 0)
 {
   stepperX.moveTo(0); 
   stepperY.moveTo(0); 
   stepperZ.moveTo(0); 

   stepperX.runToPosition();
   stepperY.runToPosition();
   stepperZ.runToPosition();

   Serial.println("Return to Home Position");
 }
 else if (strncmp(gcode, "G17", 3) == 0)  
 {
   Serial.println("Select XY Plane.");
 }
 else if (strncmp(gcode, "G18", 3) == 0)  
 {
   Serial.println("Select XZ Plane.");
 }
 else if (strncmp(gcode, "G19", 3) == 0)  
 {
   Serial.println("Select YZ Plane.");
 }
 else if (strncmp(gcode, "G54", 3) == 0)
 {
   currentWCS = WCS_G54;
   Serial.println("Work Coordinate System 1.");
 }
 else if (strncmp(gcode, "G55", 3) == 0)
 {
   currentWCS = WCS_G55;
   Serial.println("Work Coordinate System 2.");
 }
 else if (strncmp(gcode, "G56", 3) == 0)
 {
   currentWCS = WCS_G56;
   Serial.println("Work Coordinate System 3.");
 }
 else if (strncmp(gcode, "G57", 3) == 0)
 {
   currentWCS = WCS_G57;
   Serial.println("Work Coordinate System 4.");
 }
 else if (strncmp(gcode, "G58", 3) == 0)
 {
   currentWCS = WCS_G58;
   Serial.println("Work Coordinate System 5.");
 }
 else if (strncmp(gcode, "G59", 3) == 0)
 {
   currentWCS = WCS_G59;
   Serial.println("Work Coordinate System 6.");
 }
 else if (strncmp(gcode, "G41", 3) == 0)
 {
   toolRadiusCompensation = getValue(gcode, 'D');
   radiusCompensationLeft = true;
   radiusCompensationRight = false;
   Serial.print("Cutter Radius Compensation Left: ");
   Serial.println(toolRadiusCompensation);
 }
 else if (strncmp(gcode, "G42", 3) == 0)
 {
   toolRadiusCompensation = getValue(gcode, 'D');
   radiusCompensationRight = true;
   radiusCompensationLeft = false;
   Serial.print("Cutter Radius Compensation Right: ");
   Serial.println(toolRadiusCompensation);
 }
 else if (strncmp(gcode, "G40", 3) == 0)
 {
   toolRadiusCompensation = 0;
   radiusCompensationLeft = false;
   radiusCompensationRight = false;
   Serial.println("Cancel Cutter Radius Compensation.");
 }
 else if (strncmp(gcode, "G43", 3) == 0)
 {
   toolLengthOffset = getValue(gcode, 'H');
   Serial.print("Apply Tool Length Compensation: ");
   Serial.println(toolLengthOffset);
 }
 else if (strncmp(gcode, "G49", 3) == 0)
 {
   toolLengthOffset = 0;
   Serial.println("Cancel Tool Length Compensation.");
 }
 else if (strncmp(gcode, "G0", 2) == 0)
 {
   float x = getValue(gcode, 'X');
   float y = getValue(gcode, 'Y');
   float z = getValue(gcode, 'Z');
   moveTo(x, y, z);
 }
 else if (strncmp(gcode, "G1", 2) == 0)
 {
   float x = getValue(gcode, 'X');
   float y = getValue(gcode, 'Y');
   float z = getValue(gcode, 'Z');
   float f = getValue(gcode, 'F'); 
   linearMoveTo(x, y, z, f);
 }
 else if (strncmp(gcode, "G2", 2) == 0)
 {
   float x = getValue(gcode, 'X');
   float y = getValue(gcode, 'Y');
   float i = getValue(gcode, 'I'); 
   float j = getValue(gcode, 'J'); 
   float f = getValue(gcode, 'F');
   clockwiseArcMoveTo(x, y, i, j, f);
 }
 else if (strncmp(gcode, "G3", 2) == 0)
 {
   float x = getValue(gcode, 'X');
   float y = getValue(gcode, 'Y');
   float i = getValue(gcode, 'I'); 
   float j = getValue(gcode, 'J'); 
   float f = getValue(gcode, 'F'); 
   counterclockwiseArcMoveTo(x, y, i, j, f);
 }
 else if (strncmp(gcode, "F", 1) == 0)
 {
   float newSpeed = getValue(gcode, 'F');
   setMaxSpeed(newSpeed);
 }
 else if (strncmp(gcode, "M3", 2) == 0)
 {
   float speed = getValue(gcode, 'S');
   setSpindleSpeed(speed);
   Serial.print("Spindle ON Clockwise: ");
   Serial.println(speed);
 }
 else if (strncmp(gcode, "M4", 2) == 0)
 {
   float speed = getValue(gcode, 'S');
   setSpindleSpeed(-speed);
   Serial.print("Spindle ON Counterclockwise: ");
   Serial.println(speed);
 }
 else if (strncmp(gcode, "M5", 2) == 0)
 {
   setSpindleSpeed(0);
   Serial.println("Spindle Stop");
 }
}

void CNCController3Axis::moveTo(float x, float y, float z) {
 long targetX = absoluteMode ? convertToSteps(x, 'X') + wcsOffsets[currentWCS - 1][0] : convertToSteps(x, 'X') + stepperX.currentPosition() + wcsOffsets[currentWCS - 1][0];
 long targetY = absoluteMode ? convertToSteps(y, 'Y') + wcsOffsets[currentWCS - 1][1] : convertToSteps(y, 'Y') + stepperY.currentPosition() + wcsOffsets[currentWCS - 1][1];
 long targetZ = absoluteMode ? convertToSteps(z, 'Z') - convertToSteps(toolLengthOffset) + wcsOffsets[currentWCS - 1][2] : convertToSteps(z, 'Z') - convertToSteps(toolLengthOffset) + stepperZ.currentPosition() + wcsOffsets[currentWCS - 1][2];

   if (radiusCompensationLeft) {
       // Jika G41 aktif, geser jalur ke kiri berdasarkan radius alat
       targetX -= convertToSteps(toolRadiusCompensation, 'X'); 
       targetY -= convertToSteps(toolRadiusCompensation, 'Y');
   } 
   else if (radiusCompensationRight) {
       // Jika G42 aktif, geser jalur ke kanan berdasarkan radius alat
       targetX += convertToSteps(toolRadiusCompensation, 'X'); 
       targetY += convertToSteps(toolRadiusCompensation, 'Y');
   }

 stepperX.moveTo(targetX);
 stepperY.moveTo(targetY);
 stepperZ.moveTo(targetZ);

 Serial.print("X=");
 Serial.print(x);
 Serial.print(" Y=");
 Serial.print(y);
 Serial.print(" Z=");
 Serial.println(z);
}

void CNCController3Axis::linearMoveTo(float x, float y, float z, float feedrate) {
 stepperX.setMaxSpeed(feedrate);
 stepperY.setMaxSpeed(feedrate);
 stepperZ.setMaxSpeed(feedrate);

 long targetX = absoluteMode ? convertToSteps(x) + wcsOffsets[currentWCS - 1][0] : convertToSteps(x) + stepperX.currentPosition() + wcsOffsets[currentWCS - 1][0];
 long targetY = absoluteMode ? convertToSteps(y) + wcsOffsets[currentWCS - 1][1] : convertToSteps(y) + stepperY.currentPosition() + wcsOffsets[currentWCS - 1][1];
 long targetZ = absoluteMode ? (convertToSteps(z) - convertToSteps(toolLengthOffset)) + wcsOffsets[currentWCS - 1][2] : (convertToSteps(z) - convertToSteps(toolLengthOffset)) + stepperZ.currentPosition() + wcsOffsets[currentWCS - 1][2];

   if (radiusCompensationLeft) {
       // Jika G41 aktif, geser jalur ke kiri berdasarkan radius alat
       targetX -= convertToSteps(toolRadiusCompensation, 'X'); 
       targetY -= convertToSteps(toolRadiusCompensation, 'Y');
   } 
   else if (radiusCompensationRight) {
       // Jika G42 aktif, geser jalur ke kanan berdasarkan radius alat
       targetX += convertToSteps(toolRadiusCompensation, 'X'); 
       targetY += convertToSteps(toolRadiusCompensation, 'Y');
   }

 stepperX.moveTo(targetX);
 stepperY.moveTo(targetY);
 stepperZ.moveTo(targetZ);

 Serial.print("X=");
 Serial.print(x);
 Serial.print(" Y=");
 Serial.print(y);
 Serial.print(" Z=");
 Serial.print(z);
 Serial.print(" With feedrate ");
 Serial.println(feedrate);
}

void CNCController3Axis::clockwiseArcMoveTo(float x, float y, float i, float j, float feedrate){
 stepperX.setMaxSpeed(feedrate);
 stepperY.setMaxSpeed(feedrate);

 long centerX = convertToSteps(i) + wcsOffsets[currentWCS - 1][0];
 long centerY = convertToSteps(j) + wcsOffsets[currentWCS - 1][1];
 long targetX = convertToSteps(x) + wcsOffsets[currentWCS - 1][0];
 long targetY = convertToSteps(y) + wcsOffsets[currentWCS - 1][1];

 float radius = sqrt(pow(centerX - targetX, 2) + pow(centerY - targetY, 2));
 float startAngle = atan2(centerY - stepperY.currentPosition(), stepperX.currentPosition() - centerX);
 float endAngle = atan2(targetY - centerY, targetX - centerX);
 
 if (endAngle < startAngle) {
   endAngle += 2 * PI;
 }

 float angleIncrement = (feedrate / radius) * 0.01; // Adjustment for angle increment
 
 // Gerakan searah jarum jam
 for (float angle = startAngle; angle <= endAngle; angle += angleIncrement)
 {
   float newX = centerX + radius * cos(angle);
   float newY = centerY + radius * sin(angle);

   stepperX.moveTo(newX);
   stepperY.moveTo(newY);
   
   while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0)
   {
     stepperX.run();
     stepperY.run();
   }
 }

 stepperX.moveTo(targetX);
 stepperY.moveTo(targetY);

 while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0)
 {
   stepperX.run();
   stepperY.run();
 }

 Serial.print("X=");
 Serial.print(x);
 Serial.print(" Y=");
 Serial.print(y);
 Serial.print(" I=");
 Serial.print(i);
 Serial.print(" J=");
 Serial.print(j);
 Serial.print(" With feedrate ");
 Serial.println(feedrate);
}

void CNCController3Axis::counterclockwiseArcMoveTo(float x, float y, float i, float j, float feedrate) {
 stepperX.setMaxSpeed(feedrate);
 stepperY.setMaxSpeed(feedrate);

 long centerX = convertToSteps(i) + wcsOffsets[currentWCS - 1][0];
 long centerY = convertToSteps(j) + wcsOffsets[currentWCS - 1][1];
 long targetX = convertToSteps(x) + wcsOffsets[currentWCS - 1][0];
 long targetY = convertToSteps(y) + wcsOffsets[currentWCS - 1][1];

 float radius = sqrt(pow(centerX - targetX, 2) + pow(centerY - targetY, 2));
 float startAngle = atan2(centerY - stepperY.currentPosition(), stepperX.currentPosition() - centerX);
 float endAngle = atan2(targetY - centerY, targetX - centerX);

 if (startAngle < endAngle) {
   startAngle += 2 * PI;
 }

 float angleIncrement = (feedrate / radius) * 0.01; // Adjustment for angle increment
 
 // Gerakan berlawanan arah jarum jam
 for (float angle = startAngle; angle >= endAngle; angle -= angleIncrement)
 {
   float newX = centerX + radius * cos(angle);
   float newY = centerY + radius * sin(angle);

   stepperX.moveTo(newX);
   stepperY.moveTo(newY);

   while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0)
   {
     stepperX.run();
     stepperY.run();
   }
 }

 stepperX.moveTo(targetX);
 stepperY.moveTo(targetY);

 while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0)
 {
   stepperX.run();
   stepperY.run();
 }

 Serial.print("X=");
 Serial.print(x);
 Serial.print(" Y=");
 Serial.print(y);
 Serial.print(" I=");
 Serial.print(i);
 Serial.print(" J=");
 Serial.print(j);
 Serial.print(" With feedrate ");
 Serial.println(feedrate);
}

void CNCController3Axis::setSpindleSpeed(float speed) {
 int pwmValue = map(abs(speed), 0, 100, 0, 255);
 analogWrite(0, pwmValue);
 if (speed < 0)
 {
   // Putar spindle berlawanan arah jarum jam
   digitalWrite(SPINDLE_PWM_PIN, LOW);
 }
 else
 {
   // Putar spindle searah jarum jam
   digitalWrite(SPINDLE_PWM_PIN, HIGH);
 }
 Serial.print("Spindle Speed : ");
 Serial.println(speed);
}

float CNCController3Axis::getValue(const char *gcode, char key) {
 const char *ptr = strchr(gcode, key);
 if (ptr)
 {
   return atof(ptr + 1);
 }
 return 0;
}
