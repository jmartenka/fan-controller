#include "display.h"
#include "config.h"

#include <font6x8.h>

#define CHAR_WIDTH 7.0
#define CHAR_HEIGHT 13.0


static const PROGMEM uint8_t bitmapTemp[] = {
  0b00000000,
  0b00000000,
  0b01100000,
  0b11111111,
  0b11111111,
  0b01100000,
  0b00000000,
};

static const PROGMEM uint8_t bitmapFan[3][7] = {
  {
    0b00110000,
    0b00110011,
    0b00011111,
    0b00011100,
    0b01111100,
    0b01100110,
    0b00000110,
  }, {
    0b00011100,
    0b00011000,
    0b01001011,
    0b01111111,
    0b01101001,
    0b00001100,
    0b00011100,
  }, {
    0b00000110,
    0b01100110,
    0b01111100,
    0b00011100,
    0b00011111,
    0b00110011,
    0b00110000,
  }
};

static const PROGMEM uint8_t bitmapDegree[] = {
  0b00000000,
  0b00000110,
  0b00001001,
  0b00001001,
  0b00000110,
};

static const PROGMEM uint8_t deltaIcon[] = {
  0b11000000,
  0b10110000,
  0b10001100,
  0b10000011,
  0b10001100,
  0b10110000,
  0b11000000,
};

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9);

Display::Display(Sensor* sensors, size_t sensorsLength, Fan* fans, size_t fansLength) {
  this->sensors = sensors;
  this->sensorsLength = sensorsLength;
  this->fans = fans;
  this->fansLength = fansLength;

  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();

  u8g2.drawStr(u8g2.getDisplayWidth() / 2 - 5 * CHAR_WIDTH, u8g2.getDisplayHeight(), "Booting...");
  u8g2.sendBuffer();

  lastState.rpm = new uint16_t[fansLength];
}

void Display::update(Mode mode) {
  uint8_t x = 0;
  uint8_t y = 9;

  if (mode != lastState.mode || init) {
    u8g2.setFont(u8g2_font_7x13_mf);

    x = u8g2.getDisplayWidth() / 2.0 - (5 + strlen(modeToStr(mode))) * CHAR_WIDTH / 2.0;
    u8g2.drawStr(x, y, "Mode:");
    u8g2.drawStr(x + 5 * CHAR_WIDTH, y, modeToStr(mode));
    u8g2.sendBuffer();

    lastState.mode = mode;
  }

  y += CHAR_HEIGHT;

  char buffer[24];

  for (uint8_t i = 0 ; i < sensorsLength ; i++) {
    auto& sensor = sensors[i];

    // Name
    u8g2.drawStr(9, y, sensor.def->name);

    // C
    u8g2.drawStr(u8g2.getDisplayWidth() - 2 * CHAR_WIDTH, y, " c");

    // Temp value
    dtostrf(sensor.smoothedTemp(), 6, 1, buffer);
    u8g2.drawStr(u8g2.getDisplayWidth() - (2 + 6) * CHAR_WIDTH, y, buffer);

    // Write to display
    u8g2.sendBuffer();

    y += CHAR_HEIGHT;

    for (uint8_t j = 0 ; j < fansLength ; j++) {
      auto& fan = fans[j];
      if (fan.def->sensorIndex == i) {
        // Name
        u8g2.drawStr(15, y, fan.def->name);

        // %
        u8g2.drawStr(u8g2.getDisplayWidth() - CHAR_WIDTH, y, "%");

        // TODO: Icon

        //Speed
        sprintf(buffer, "%3d", fan.currentSpeed());
        u8g2.drawStr(u8g2.getDisplayWidth() - (3 + 1) * CHAR_WIDTH, y, buffer);

        //RPM
        if (fan.hasTacho()) {
          auto rpm = fan.getRPM();
          auto rpmSave = rpm;
          if (init == false) {
            rpm = (rpm + lastState.rpm[j]) / 2;
          }
          lastState.rpm[j] = rpmSave;

          int digits = rpm == 0 ? 1 : (log10(rpm) + 1);

          if (digits <= 4)
            sprintf(buffer, "%4u", rpm);
          else
            sprintf(buffer, "%3uk", rpm / 1000ul);

          u8g2.drawStr(u8g2.getDisplayWidth() - (5 + 3 + 1) * CHAR_WIDTH, y, buffer);
        }

        y += CHAR_HEIGHT;
      }
    }
  }

  // TODO
  //  for (uint8_t i = 0 ; i < customLength ; i++) {
  //    auto& custom = customDefs[i];
  //    custom.value(buffer, sensors, fans);
  //    y += CHAR_HEIGHT;
  //  }

  cnt++;
  init = false;
}
