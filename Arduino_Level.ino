//Imports necessary libraries
#include "Wire.h"
#include <MPU6050_light.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

//Defines connections to MPU-6050, HMC883L, and LED display modules
MPU6050 mpu(Wire);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
unsigned long timer = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Defines pins for x-axis LEDs
int LED_red_x_0 = 9;
int LED_yellow_x_0 = 10;
int LED_green_x_0 = 11;
int LED_yellow_x_1 = 12;
int LED_red_x_1 = 13;

//Defines pins for y-axis LEDs
int LED_red_y_0 = 8;
int LED_yellow_y_0 = 7;
int LED_green_y_0 = 6;
int LED_yellow_y_1 = 5;
int LED_red_y_1 = 4;
 
void setup() {
  //Starts wires for connections
  Wire.begin();

  //Turns display on
  lcd.init();
  lcd.backlight();
  lcd.begin(16,2);

  //Starts connection to MPU6050 module
  byte status = mpu.begin();
  while (status != 0) { 
    //Waits if the MPU-6050 is not connected properly
    }

  //Show message while the sensor is calibrating
  lcd.setCursor(0,0);
  lcd.print("Calibrando...");
  delay(1000);
  lcd.clear();

  //Calibration of MPU6050
  mpu.calcOffsets();

  //Shows message when the sensor has been succesfully calibrated
  lcd.setCursor(0,0);
  lcd.print("Listo!");
  delay(1000);  
  lcd.clear();

  //Begins tranmission to HMC5883L
  Wire.beginTransmission(0x1E);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();

  //Defines pin mode of both x and y axis LEDs as outputs
  pinMode(LED_red_x_0, OUTPUT);
  pinMode(LED_yellow_x_0, OUTPUT);
  pinMode(LED_green_x_0, OUTPUT);
  pinMode(LED_yellow_x_1, OUTPUT);
  pinMode(LED_red_x_1, OUTPUT);

  pinMode(LED_red_y_0, OUTPUT);
  pinMode(LED_yellow_y_0, OUTPUT);
  pinMode(LED_green_y_0, OUTPUT);
  pinMode(LED_yellow_y_1, OUTPUT);
  pinMode(LED_red_y_1, OUTPUT);
   
 }
void loop() {
  //Updates the MPU6050
  mpu.update();

  //Gets and displays data every 300 ms
  if ((millis() - timer) > 300) { 
    //Updates data from HMC5883L module
    sensors_event_t event;
    mag.getEvent(&event);

    //Calculates the heading of the device
    float heading = atan2(event.magnetic.y, event.magnetic.x);

    //Corrects for the current declination angle of the earth
    float declination = 0.22;
    heading += declination;
    if (heading < 0) {
      heading += (2 * M_PI);
     }
    if (heading > (2 * M_PI)) {
      heading -= (2 * M_PI);
     }

    //Calculates the heading of the device in degrees
    float headingDegrees = heading * (180 / M_PI);

    String coordinate = "";

    //Sets the coordinate to be shown on screen depending on the current heading
    if (headingDegrees > 338 || headingDegrees < 22 ) {
      coordinate = "N";
     }
    if (headingDegrees > 22 && headingDegrees < 66 ) {
      coordinate = "NE";
     }
    if (headingDegrees > 66 && headingDegrees < 111 ) {
      coordinate = "E";
     }
    if (headingDegrees > 111 && headingDegrees < 156 ) {
      coordinate = "SE";
     }
    if (headingDegrees > 156 && headingDegrees < 201 ) {
      coordinate = "S";
     }
    if (headingDegrees > 201 && headingDegrees < 246 ) {
      coordinate = "SW";
     }
    if (headingDegrees > 246 && headingDegrees < 291 ) {
      coordinate = "W";
     }
    if (headingDegrees > 291 && headingDegrees < 338 ) {
      coordinate = "NW";
     }

    //Inclination angles for both x and y axis
    float angleX = mpu.getAngleX();
    float angleY = mpu.getAngleY();

    //Clears previous data and sets the new one to be shown on the screen
    lcd.clear();

    //Inclination on x-axis
    lcd.setCursor(0,0);
    lcd.print("X:");
    lcd.print(angleX);

    //Inclination on y-axis
    lcd.setCursor(0,1);
    lcd.print("Y:");
    lcd.print(angleY);

    //Heading in degrees
    lcd.setCursor(9,0);
    lcd.print("H:");
    lcd.print(headingDegrees);

    //Coordinate in the cardinal system
    lcd.setCursor(9,1);
    lcd.print("C:");
    lcd.print(coordinate);

    //Turns on the corresponding red LED if the inclination on the x-axis is
    //less than -5.01 degrees
    if (angleX < -5.01) {
    digitalWrite(LED_red_x_0, HIGH);
    digitalWrite(LED_yellow_x_0, LOW);
    digitalWrite(LED_green_x_0, LOW);
    digitalWrite(LED_yellow_x_1, LOW);
    digitalWrite(LED_red_x_1, LOW);
    }

    //Turns on the corresponding yellow LED if the inclination on the x-axis is
    //any value between -5.00 and -1.01 degrees
    else if ((angleX > -5.00) && (angleX < -1.01)) {
    digitalWrite(LED_red_x_0, LOW);
    digitalWrite(LED_yellow_x_0, HIGH);
    digitalWrite(LED_green_x_0, LOW);
    digitalWrite(LED_yellow_x_1, LOW);
    digitalWrite(LED_red_x_1, LOW);
    }

    //Turns on the corresponding green LED if the inclination on the x-axis is
    //any value between -1.00 and 1.00 degrees
    else if ((angleX < 1.00) && (angleX > -1.00)) {
    digitalWrite(LED_red_x_0, LOW);
    digitalWrite(LED_yellow_x_0, LOW);
    digitalWrite(LED_green_x_0, HIGH);
    digitalWrite(LED_yellow_x_1, LOW);
    digitalWrite(LED_red_x_1, LOW);
    }

    //Turns on the corresponding yellow LED if the inclination on the x-axis is
    //any value between 1.01 and 5.00 degrees
    else if ((angleX > 1.01) && (angleX < 5.00)) {
    digitalWrite(LED_red_x_0, LOW);
    digitalWrite(LED_yellow_x_0, LOW);
    digitalWrite(LED_green_x_0, LOW);
    digitalWrite(LED_yellow_x_1, HIGH);
    digitalWrite(LED_red_x_1, LOW);
    }

    //Turns on the corresponding red LED if the inclination on the x-axis is
    //greater than 5.01 degrees
    else if (angleX > 5.01) { 
    digitalWrite(LED_red_x_0, LOW);
    digitalWrite(LED_yellow_x_0, LOW);
    digitalWrite(LED_green_x_0, LOW);
    digitalWrite(LED_yellow_x_1, LOW);
    digitalWrite(LED_red_x_1, HIGH);
    }

    //Turns on the corresponding red LED if the inclination on the y-axis is
    //less than -5.01 degrees
    if (angleY < -5.01) {
    digitalWrite(LED_red_y_0, HIGH);
    digitalWrite(LED_yellow_y_0, LOW);
    digitalWrite(LED_green_y_0, LOW);
    digitalWrite(LED_yellow_y_1, LOW);
    digitalWrite(LED_red_y_1, LOW);
    }

    //Turns on the corresponding yellow LED if the inclination on the y-axis is
    //any value between -5.00 and -1.01 degrees
    else if ((angleY > -5.00) && (angleY < -1.01)) {
    digitalWrite(LED_red_y_0, LOW);
    digitalWrite(LED_yellow_y_0, HIGH);
    digitalWrite(LED_green_y_0, LOW);
    digitalWrite(LED_yellow_y_1, LOW);
    digitalWrite(LED_red_y_1, LOW);
    }

    //Turns on the corresponding green LED if the inclination on the y-axis is
    //any value between -1.00 and 1.00 degrees
    else if ((angleY < 1.00) && (angleY > -1.00)) {
    digitalWrite(LED_red_y_0, LOW);
    digitalWrite(LED_yellow_y_0, LOW);
    digitalWrite(LED_green_y_0, HIGH);
    digitalWrite(LED_yellow_y_1, LOW);
    digitalWrite(LED_red_y_1, LOW);
    }

    //Turns on the corresponding yellow LED if the inclination on the y-axis is
    //any value between 1.01 and 5.00 degrees
    else if ((angleY > 1.01) && (angleY < 5.00)) {
    digitalWrite(LED_red_y_0, LOW);
    digitalWrite(LED_yellow_y_0, LOW);
    digitalWrite(LED_green_y_0, LOW);
    digitalWrite(LED_yellow_y_1, HIGH);
    digitalWrite(LED_red_y_1, LOW);
    }

    //Turns on the corresponding red LED if the inclination on the y-axis is
    //greater than 5.01 degrees
    else if (angleY > 5.01) { 
    digitalWrite(LED_red_y_0, LOW);
    digitalWrite(LED_yellow_y_0, LOW);
    digitalWrite(LED_green_y_0, LOW);
    digitalWrite(LED_yellow_y_1, LOW);
    digitalWrite(LED_red_y_1, HIGH);
    }

    //Resets the timer to show new data
     timer = millis();
   }
 }
