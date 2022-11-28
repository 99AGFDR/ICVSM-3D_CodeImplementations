#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Variable Designation of Variables
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  // Call Calculation and Power ON Functions
  setupMPU();

  // Power ON the LCD,
  lcd.init();
  // Power ON the blacklight
  lcd.backlight();
}

void loop() {
  recordAccelRegisters();
  printData();
  delay(100);

  lcd.clear();// clearn previous values from screen
  lcdDisplay(
    // to print X:
    0, // character 0
    0, // line 0
    "X:",

    // to print AngleX
    2, // character 2
    0, // line 0
    gForceX
  );

  lcdDisplay(
    // to print Y:
    13, // character 13
    0, // line 0
    "Y:",

    // to print AngleY
    0, // character 0
    1, // line 1
    gForceY
  );

  lcdDisplay(
    // to print Z:
    7, // character 7
    1, // line 1
    "Z:",

    // to print AngleZ
    9, // character 9
    1, // line 0
    gForceZ
  );
  delay(100);
}// loop end

void lcdDisplay(int tc, int tr, String title, int vc, int vr, float value)
{
  // Robojax.com MPU6050 Demo
  lcd.setCursor (tc, tr); //
  lcd.print(title);

  lcd.setCursor (vc, vr); //
  lcd.print(value);

}

//Function to Check and Change Sensor Settings
void setupMPU() {
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 333.3 RPM
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5)
  Wire.write(0b00010000); //Setting the accel to +/- 8g
  Wire.endTransmission();
}

// Function To Store Accelerometer Readings
void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (3B - 40)
  while (Wire.available() < 6);
  accelX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

// ON-Board Computation and conversion of the LSB-binary to g-force of the data
void processAccelData() {
  gForceX = accelX / 4096.0;
  gForceY = accelY / 4096.0;
  gForceZ = accelZ / 4096.0;
}

// Function To Store Rotational Readings
void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Gyro Registers (43 - 48)
  while (Wire.available() < 6);
  gyroX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

// ON-Board computation and conversion of the LSB-Binary to RPM-force of the data
void processGyroData() {
  rotX = gyroX / 131;
  rotY = gyroY / 131;
  rotZ = gyroZ / 131;
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}
