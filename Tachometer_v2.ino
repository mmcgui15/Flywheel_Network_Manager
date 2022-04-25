/*
Arduino Hall Effect Sensor Project
by Arvind Sanjeev
Please check out  http://diyhacking.com for the tutorial of this project.
DIY Hacking
*/


 volatile byte revolutions;
 unsigned int rpm;
 unsigned long timeold;
 void setup()
 {
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(2), magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
   revolutions = 0;
   rpm = 0;
   timeold = 0;
 }
 void loop()//Measure RPM
 {
   if (revolutions >= 10) {             //add delay() or raise revolution count if rpms are too high and cause sensor errors
     rpm = 60000/(millis() - timeold)*revolutions;
     timeold = millis();
     revolutions = 0;
     Serial.println(rpm,DEC);
   }
 }
 void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   revolutions++;
   //Serial.println("detect");
 }
