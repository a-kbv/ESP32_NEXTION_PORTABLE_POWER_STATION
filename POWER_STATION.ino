/**
 * @file POWER_STATION.ino
 * @author Atanas Korabov (atanaskorabov@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
//---- LIBRARIES ----//
#include <math.h>
#include <Preferences.h>

Preferences preferences;


//---- DEFINES ----//

#define INPUT_VOLTAGE_SENSE_PIN 33
#define OUTPUT_VOLTAGE_SENSE_PIN 35
#define INPUT_CURRENT_SENSE_PIN 32
#define OUTPUT_CURRENT_SENSE_PIN 15
#define ledPin 2

//---- CONSTANTS ----//
double mVPerAmp = 66; // use 66 for 30A Module
double ACSoffset = 1532;
float  R1_VOLTAGE = 68000; //68K
float  R2_VOLTAGE = 6800; // 6.8K
float outputEnergy = 0;
float inputEnergy = 0;
unsigned long previousMillis = 0;
unsigned long interval = 100;
unsigned long currentTime = 0;
unsigned long lastTime = 0;


//---- FUNCTIONS ----//
double returnCurrentValue(int pinNumber)
{
  double tmp = 0;
  double avg = 0;
  double ADCVoltage = 0;
  double amps = 0;
  for (int z = 0; z < 150; z++)
  {
    tmp = tmp + analogRead(pinNumber);
  }
  avg = tmp / 150;
  ADCVoltage = ((avg / 4095.0) * 3300); // Gets you mV
  amps = ((ADCVoltage - ACSoffset) / mVPerAmp);
  return amps;
}

double returnVoltageValue(int pinNumber)
{
  double tmp = 0;
  double ADCVoltage = 0;
  double inputVoltage = 0;
  double avg = 0;
  for (int i = 0; i < 150; i++)
  {
    tmp = tmp + analogRead(pinNumber);
  }
  avg = tmp / 150;
  ADCVoltage = ((avg * 3.3) / (4095)) + 0.138;
  inputVoltage = ADCVoltage / (R2_VOLTAGE / (R1_VOLTAGE + R2_VOLTAGE)); // formula for calculating voltage in i.e. GND
  return inputVoltage;
}

int calculateAndDisplayBatteryPercentage(float threeCellVoltage) {
  float voltage = threeCellVoltage/3;
  
  float capacityArray[122] = {100.00, 99.90, 99.59, 99.39, 99.29, 98.98, 98.78, 98.07, 97.46, 96.44, 95.53, 94.61, 93.50, 92.68, 91.67, 90.65, 89.53, 88.62, 87.70, 86.79, 85.87, 84.65, 83.74, 82.72, 82.01, 81.10, 80.28, 79.17, 78.25, 77.64, 76.83, 75.81, 74.59, 73.68, 72.66, 71.44, 70.53, 69.72, 68.90, 67.99, 67.17, 66.36, 65.35, 64.63, 63.72, 62.70, 61.69, 60.77, 59.96, 59.04, 57.83, 56.50, 55.49, 54.47, 53.56, 52.34, 51.32, 50.30, 49.29, 48.48, 47.66, 46.85, 45.93, 44.92, 43.60, 42.38, 41.16, 40.14, 39.13, 38.01, 36.79, 35.77, 34.55, 33.33, 32.01, 30.69, 29.88, 29.17, 28.35, 27.24, 26.22, 25.30, 24.49, 23.88, 22.87, 21.85, 20.83, 19.72, 18.90, 18.09, 17.28, 16.46, 15.55, 14.53, 13.82, 13.21, 12.50, 11.99, 11.28, 10.57, 10.06, 9.25, 8.94, 8.54, 8.23, 7.83, 7.42, 7.11, 6.71, 6.50, 6.10, 5.79, 5.59, 5.28, 4.98, 4.67, 4.27, 3.86, 3.66, 3.35, 2.85, 2.34};
  float voltageArray[122] = {4.20, 4.17, 4.15, 4.13, 4.10, 4.08, 4.05, 4.03, 4.02, 4.01, 3.99, 3.98, 3.97, 3.97, 3.96, 3.95, 3.94, 3.94, 3.93, 3.93, 3.92, 3.91, 3.90, 3.90, 3.89, 3.87, 3.86, 3.85, 3.84, 3.83, 3.83, 3.81, 3.80, 3.79, 3.78, 3.77, 3.76, 3.75, 3.74, 3.73, 3.72, 3.71, 3.70, 3.69, 3.68, 3.67, 3.66, 3.65, 3.64, 3.63, 3.61, 3.60, 3.59, 3.58, 3.56, 3.55, 3.54, 3.53, 3.53, 3.51, 3.51, 3.50, 3.50, 3.49, 3.48, 3.47, 3.47, 3.47, 3.46, 3.45, 3.44, 3.44, 3.43, 3.42, 3.42, 3.41, 3.40, 3.39, 3.38, 3.37, 3.36, 3.35, 3.35, 3.34, 3.33, 3.32, 3.31, 3.29, 3.28, 3.26, 3.25, 3.24, 3.22, 3.20, 3.18, 3.16, 3.14, 3.11, 3.10, 3.07, 3.05, 3.02, 3.00, 2.98, 2.95, 2.92, 2.90, 2.88, 2.86, 2.84, 2.81, 2.78, 2.75, 2.72, 2.69, 2.66, 2.63, 2.60, 2.58, 2.55, 2.53, 2.51};
  int colorArray[12] = {2016, 2024, 34784, 34800, 49136, 65520, 65504, 65216, 64704, 64128, 63488, 63488};
  int closestIndex = 0;
  float closestDiff = 100.0;
  
  for (int i = 0; i < 123; i++) {
    float diff = abs(voltage - voltageArray[i]);
    if (diff < closestDiff) {
      closestDiff = diff;
      closestIndex = i;
    }
  } 

  int capacity = capacityArray[closestIndex];

  nextionSendIntegerData(capacity, "j0.val");

  nextionSendIntegerData(colorArray[closestIndex / 10], "j0.pco");

  nextionSendIntegerAsStringData(capacity, "t3.txt");
}

void nextionSendFloatData(float data, char *selector)
{
  Serial2.print(selector); Serial2.print("="); Serial2.print(data);
  Serial2.write(0xff); Serial2.write(0xff); Serial2.write(0xff);
}

void nextionSendFloatAsStringData(float data, char *selector)
{
  Serial2.print(selector);
  Serial2.print("="); Serial2.print("\""); Serial2.print(data); Serial2.print("\"");
  Serial2.write(0xff); Serial2.write(0xff); Serial2.write(0xff);
}

void nextionSendIntegerData(int data, char *selector)
{
  Serial2.print(selector); Serial2.print("="); Serial2.print(data);
  Serial2.write(0xff); Serial2.write(0xff); Serial2.write(0xff);
}
void nextionSendIntegerAsStringData(int data, char *selector)
{
  Serial2.print(selector); 
  Serial2.print("="); Serial2.print("\""); Serial2.print(data); Serial2.print("\"");
  Serial2.write(0xff); Serial2.write(0xff); Serial2.write(0xff);
}

void setup() {
//---- SETUP ----//
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);
  
//EXAMPLE SAVE DATA
  preferences.begin("my-app", false);
  unsigned int counter = preferences.getUInt("counter", 0);
  counter++;
  preferences.putUInt("counter", counter);
  preferences.end();
  //EXAMPLE SAVE DATA
}

void loop() {

unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; 
    //READ SENSOR DATA
    float inputVoltage = abs(returnVoltageValue(INPUT_VOLTAGE_SENSE_PIN)) ;
    float inputCurrent = abs(returnCurrentValue(INPUT_CURRENT_SENSE_PIN)) ;
    float outputVoltage = abs(returnVoltageValue(OUTPUT_VOLTAGE_SENSE_PIN)) ;
    float outputCurrent =  abs((returnCurrentValue(OUTPUT_CURRENT_SENSE_PIN))) ;

    outputVoltage = outputVoltage + 10.00;
    inputVoltage = inputVoltage + 10.00;

    float outputPower = abs(outputCurrent * outputVoltage);
    float inputPower = abs(inputCurrent * inputVoltage);

    lastTime = currentTime;
    currentTime = millis();
    outputEnergy = outputEnergy + outputPower * (( currentTime - lastTime ) / 3600000.0 );
    inputEnergy = inputEnergy + inputPower * (( currentTime - lastTime ) / 3600000.0 );
    
  
    //DISPLAY SENSOR DATA
    nextionSendFloatAsStringData(outputCurrent, "t2.txt"); // OUTPUT CURRENT
    nextionSendFloatAsStringData(outputVoltage, "t1.txt"); // OUTPUT VOLTAGE
    nextionSendFloatAsStringData(inputCurrent, "t5.txt"); // INPUT CURRENT
    nextionSendFloatAsStringData(inputVoltage, "t4.txt"); // INPUT VOLTAGE
    nextionSendFloatAsStringData(outputPower,"t0.txt"); //OUTPUT POWER
    nextionSendFloatAsStringData(inputPower,"t6.txt"); //INPUT POWER
    nextionSendFloatAsStringData(outputEnergy,"t7.txt"); //OUTPUT ENERGY
    nextionSendFloatAsStringData(inputEnergy,"t8.txt"); //INPUT ENERGY
    calculateAndDisplayBatteryPercentage(outputVoltage);
  }
 
//EXAMPLE READ NEXTION DATA
  String Data_From_Display = "";
   if(Serial2.available())
   {
    String Data_From_Display = "";
    delay(100);
    while(Serial2.available())
    {
      Data_From_Display += char(Serial2.read());
    }
      if( Data_From_Display == "on")
       { digitalWrite(2,HIGH);}
      else if( Data_From_Display == "off")
       { digitalWrite(2,LOW); }
   }
//EXAMPLE READ NEXTION DATA

}
