#ifndef REACTOR_H
#define REACTOR_H

// Clock provides a simple abstraction over the current
// and elapsed time in the sketch.  Useful for animations
// to all be in sync without computing the time themselves.
class Clock {
public:
  Clock()
  {
    currentMS = millis();
    currentS = currentMS / 1000.0;
    deltaMS = 0;
  }

  void update() {
    uint32_t next = millis();
    deltaMS = next - currentMS;
    currentMS = next;
    currentS = currentMS / 1000.0;
  }

  uint32_t currentMS;
  float currentS;
  uint32_t deltaMS;
};

// Pixels provides an abstraction over the reactor's two pixel
// rings, an inner ring and outer ring.  Allows individual control
// of each ring through the inner/outer members and global control
// through the NeoPixel-like interface.
class Pixels {
public:
  Pixels(Adafruit_NeoPixel* inner, Adafruit_NeoPixel* outer):
    inner(inner), outer(outer)
  {}
  ~Pixels() {}

  Adafruit_NeoPixel* inner;
  Adafruit_NeoPixel* outer;

  // Set all the inner pixel ring pixels to specified color.
  void fillInner(uint32_t color) {
    for (int i=0; i < inner->numPixels(); ++i) {
      inner->setPixelColor(i, color);
    }
  }

  // Set all the outer pixel ring pixels to specified color.
  void fillOuter(uint32_t color) {
    for (int i=0; i < outer->numPixels(); ++i) {
      outer->setPixelColor(i, color);
    }
  }

  // Set all the pixels to specified color.
  void fill(uint32_t color) {
    fillInner(color);
    fillOuter(color);
  }

  // Show both pixel rings (like NeoPixel library).
  void show() {
    inner->show();
    outer->show();
  }

  // Return total number of pixels (like NeoPixel library).
  uint16_t numPixels() {
    return inner->numPixels() + outer->numPixels();
  }

  // Set all pixel brightness (like NeoPixel library).
  void setBrightness(uint8_t brightness) {
    inner->setBrightness(brightness);
    outer->setBrightness(brightness);
  }

  // Set color of a pixel (like NeoPixel library).
  void setPixelColor(uint16_t n, uint32_t color) {
    if (n >= numPixels()) {
      return;
    }
    else if (n < inner->numPixels()) {
      inner->setPixelColor(n, color);
    }
    else {
      outer->setPixelColor(n-inner->numPixels(), color);
    }
  }
};

#endif
