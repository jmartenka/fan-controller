#ifndef __INCLUDED_FAN__
#define __INCLUDED_FAN__

#include <Arduino.h>
#include "sensor.h"


enum class Mode
{
	Auto,
	Low,
	High,
	Full,
};
const char* modeToStr(Mode mode);

// This can handle 2 tachometers
#define TACHOMETER_MAX 2
extern volatile uint16_t tachoCounters[TACHOMETER_MAX];
extern void (*onTachoCbs[TACHOMETER_MAX])();
extern uint8_t onTachoCbsCnt;


struct SpeedCurvePoint {
	double temp;
	uint8_t speed;// 0 -> 100
};

struct FanDef{
	const char* name;

	uint8_t pin;
	uint8_t tachoPin;

	Sensor* sensor;

	SpeedCurvePoint* speedCurve;
	uint8_t speedCurveLength;
};


class Fan {
public:
	Fan(){};

	Fan(const FanDef* _def);


	uint8_t currentSpeed() const {
		return speed;
	}

	void setSpeed(uint8_t speed);
	void setModeSpeed(Mode mode);

	bool hasTacho() const {
		return def->tachoPin != (uint8_t)-1;
	}
	void resetTacho(){
		*tachoCounter = 0;
		tachoTimer = millis();
	}
	uint16_t getRPM() const;



	const FanDef* def = nullptr;
private:
	unsigned long tachoTimer;
	volatile uint16_t* tachoCounter;

	uint8_t speed, minSpeed, maxSpeed;

	uint8_t calcSpeed(double temp) const;
};




#endif