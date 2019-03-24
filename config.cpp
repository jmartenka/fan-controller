#include "config.h"

const uint8_t buttonPin = 20;
const uint8_t ledPin = LED_BUILTIN   ;//Blink when in low speed mode
const uint8_t buzzerPin = 12; // beep beep beep. Must be analog or PWM compatible. -1 to disable beeping.

// List of temperature sensors
const SensorDef sensorDefs[] = {
  {
name: "Ambient", // Sensor name used when sending its temperature to serial
    pin: 2, // Sensor pin. Must be analog

    maxTemp: 50.0, // Max temperature, that will make the buzzer beep as warning. Set to NAN to disable
    sensorFailTemp: 50.0, // What temperature will be reported when the sensor goes away
type: SENSOR_DHT22, // DHT22
  },
  //  {
  //		name: "Ambient",
  //		pin: 23,
  //		maxTemp: NAN,
  //    type: SENSOR_ANALOG,
  //	},
};
const uint8_t sensorsLength = sizeof(sensorDefs) / sizeof(SensorDef);

// This is an example curve for speed control
static SpeedCurve defaultCurve = {
  //  temp °C => speed %
  { 30.0, 7 }, // Everything below will be 7%
  { 40.0, 70 },
  { 50.0, 100 },
};

static uint8_t defaultCurveLength = sizeof(defaultCurve) / sizeof(SpeedCurvePoint);

// List of fans
const FanDef fanDefs[] = {
  {
    name: "Fan0", // Fan name used when sending its speed to serial
    pin: 7, // Fan pin. Must be PWM compatible
    tachoPin: 3, // Tachometer fan pin. This pin MUST support interruptions
    sensorIndex: 0, // Associated sensor index in `sensorDefs` list. Will be used to calculate fan speed with speedCurve.
    speedCurve: defaultCurve, // Curve used to calculate fan speed in Auto mode
    speedCurveLength: defaultCurveLength, // Number of points on speedCurve
  },
  //	{
  //		name: "Fan1",
  //		pin: 25,
  //		tachoPin: 22,
  //		sensorIndex: 0,
  //		speedCurve: defaultCurve,
  //		speedCurveLength: defaultCurveLength,
  //	},
  //	{
  //		name: "Fan2",
  //		pin: 26,
  //		tachoPin: -1,
  //		sensorIndex: 1,
  //		speedCurve: SpeedCurve{
  //			{ 30.0, 30 },
  //			{ 40.0, 70 },
  //			{ 45.0, 100 },
  //		},
  //		speedCurveLength: 3,
  //	},
};
const uint8_t fansLength = sizeof(fanDefs) / sizeof(FanDef);

static const PROGMEM uint8_t deltaIcon[] = {
  0b00000000,
  0b01100000,
  0b01011000,
  0b01000110,
  0b01011000,
  0b01100000,
  0b00000000,
};

const CustomDef customDefs[] = {
  {
    name: "Delta",
    icon: deltaIcon,
    value: [](char* buffer, const Sensor * sensors, const Fan * fans) {
      auto val = sensors[0].smoothedTemp() - sensors[1].smoothedTemp();
      dtostrf(val, 5, 1, buffer);
    },
  },
};

const uint8_t customLength = sizeof(customDefs) / sizeof(CustomDef);

// Is SSD1306 128x64 display connected?
const bool hasDisplay = true;
