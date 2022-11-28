 /*
 * 
 * This code is basic usage MPU-6050 Accelerometer and Gyroscope
 * Running this code you will get angles only.
 * The Angles at X, Y and Z are displayed on LCD1602-I2C display module
 * 
 * Library and code have been taken from:
 * https://github.com/tockn/MPU6050_tockn
 * 
 * Updated by Ahmad Shamshiri on July 05, 2018 at 22:19 in Ajax, Ontario, Canada
 * for Robojax.com
 * Get this code from Robojax.com
 * Watch video instruction for this code at:https://youtu.be/uIz6WIis4dc
 * 
 * You will need to watch 2 videos before following the instruction in this video:
 * 1-MPU6050 Introduction video and code: https://youtu.be/uhh7ik02aDc
 * 2-LCD1602 with I2C module video and code https://youtu.be/q9YC_GVHy5A
 */
#include <Wire.h>

MPU6050 mpu6050(Wire);

#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // initialize the LCD, 
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();   
}

void loop() {

  mpu6050.update();
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());
  
  lcd.clear();// clearn previous values from screen
  lcdDisplay(
             // to print X:
             0, // character 0  
             0, // line 0
             "X:", 

             // to print AngleX
             2, // character 2
             0, // line 0
             mpu6050.getAngleX() 
             );

  lcdDisplay(
             // to print Y:
             13, // character 13 
             0, // line 0
             "Y:", 

             // to print AngleY
             0, // character 0
             1, // line 1
             mpu6050.getAngleY() 
             );  

  lcdDisplay(
             // to print Z:
             7, // character 7 
             1, // line 1
             "Z:", 

             // to print AngleZ
             9, // character 9
             1, // line 0
             mpu6050.getAngleZ() 
             );                         
   delay(100);
}// loop end

/*
 * lcdDisplay(int tc, int tr, String title, int vc, int vr, float value)
 * displays value and title on LCD1602
 * How to use:
 * If you wan to diaplay: "Voltage: 13.56mV" starting from first character
 * on second row.
 * use:
 * lcdDisplay(0, 1, "Voltage: ", 13.56)
 *   
 * tc  is character number  (0)
 * tr is row in the lcd (1)
 * title is the text (Voltage:)
 * vc value for character 
 * vr value for  row or line
 * value is the value (13.56)
 */
void lcdDisplay(int tc, int tr, String title, int vc, int vr, float value)
{
   // Robojax.com MPU6050 Demo
   lcd.setCursor (tc,tr); //
   lcd.print(title);
   
   lcd.setCursor (vc,vr); //
   lcd.print(value);
 
}
