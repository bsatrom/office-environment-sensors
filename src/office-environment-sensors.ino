/******************************************************************************
  Office Environment Sensors Example (Rosie Hub Compartible)
  Brandon Satrom
  Original Creation Date: July 26, 2016
  Updated March 23th, 2018

  This sketch measures the temperature, humidity, and barometric pressure and
  logs those readings via MQTT to a node-red instance running as a Smart Home
  controller. The controller then forwards that information to an InfluxDB
  instance for collection and vizualization.

  The Shield library used in this example can be found here:
  https://github.com/sparkfun/SparkFun_Photon_Weather_Shield_Particle_Library

  The MQTT library can be found here:
  https://github.com/hirotakaster/MQTT

  Hardware Connections:
	This sketch was written specifically for the Photon Weather Shield,
	which connects the HTU21D and MPL3115A2 to the I2C bus by default.
  If you have an HTU21D and/or an MPL3115A2 breakout,	use the following
  hardware setup:
      HTU21D ------------- Photon
      (-) ------------------- GND
      (+) ------------------- 3.3V (VCC)
       CL ------------------- D1/SCL
       DA ------------------- D0/SDA

    MPL3115A2 ------------- Photon
      GND ------------------- GND
      VCC ------------------- 3.3V (VCC)
      SCL ------------------ D1/SCL
      SDA ------------------ D0/SDA

  Development environment specifics:
  	IDE: Particle Dev
  	Hardware Platform: Particle Photon
                       SparkFun Photon Weather Shield
*******************************************************************************/
#include "SparkFun_Photon_Weather_Shield_Library.h"

double humidity = 0;
double tempf = 0;
double pascals = 0;
float baroTemp = 0;
double pressue = pascals/100;

long lastPrint = 0;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and
//MPL3115A2 barometric sensor
Weather sensor;

//---------------------------------------------------------------
void setup()
{
    Serial.begin(9600);   // open serial over USB at 9600 baud

    Particle.variable("temp", tempf);
    Particle.variable("humidity", humidity);
    Particle.variable("pressure", pressue);

    sensor.begin();
    sensor.setModeBarometer();//Set to Barometer Mode
    sensor.setOversampleRate(7); // Set Oversample rate
    sensor.enableEventFlags(); //Necessary register calls to enble temp, baro and alt
}
//---------------------------------------------------------------

void loop()
{
      if(millis() - lastPrint > 30000)
      {
        // Record when you published
        lastPrint = millis();

        getWeather();
        printInfo(); //Print to Serial
      }
}
//---------------------------------------------------------------
void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = sensor.getTempF();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()

  //Measure the Barometer temperature in F from the MPL3115A2
  baroTemp = sensor.readBaroTempF();

  //Measure Pressure from the MPL3115A2
  pascals = sensor.readPressure();

  //If in altitude mode, you can get a reading in feet with this line:
  //float altf = sensor.readAltitudeFt();
}
//---------------------------------------------------------------

void printInfo()
{
//This function prints the weather data out to the default Serial Port

  Serial.print("Temp:");
  Serial.print(tempf);
  Serial.print("F, ");

  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.print("%, ");

  Serial.print("Baro_Temp:");
  Serial.print(baroTemp);
  Serial.print("F, ");

  Serial.print("Pressure:");
  Serial.print(pascals/100);
  Serial.println("hPa");
}
