#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "Colors.h"
#include "lerp.h"
#include "Reactor.h"


// Base class for all animations.
// Each animation must inherit from this and fill in the update function.
class Animation {
public:
  virtual ~Animation() {}

  virtual void update() = 0;
  
  Gradient* colors;
  Clock* clock;
  Pixels* pixels;
};


// Glow animation pulses smoothly from primary to secondary
// color and back at a specified frequency.
class Glow: public Animation {
public:
  Glow(float frequency):
    frequency(frequency)
  {}
  ~Glow() {}

  float frequency;

  virtual void update() {
    // Use sine wave to interpolate color of all pixels in smoothly
    // changing gradient between primary and secondary color.
    float t = sin(2.0*PI*frequency*clock->currentS);
    uint32_t color = colors->interpolate(t, -1.0, 1.0);
    pixels->fill(color);
  }
};


// Chase animates a pulse of primary to secondary color moving 
// around the pixels.  The higher the frequency the more pulses
// will be seen.
class Chase: public Animation {
public:
  Chase(float frequency):
    frequency(frequency)
  {}
  ~Chase() {}

  float frequency;

  virtual void update() {
    uint16_t n = pixels->numPixels();
    for (int i=0; i<n; ++i) {
      float phase = (float)i/(float)(n-1)*2.0*PI;
      float t = sin(2.0*PI*frequency*clock->currentS + phase);
      uint32_t color = colors->interpolate(t, -1.0, 1.0);
      pixels->setPixelColor(i, color);
    }
  }
};


// Heart beat animation uses exponential decay to flash and 
// gradually fade out the inner and outer pixel rings like
// a heart beat.  Can set the bpm beats per minute as desired.
class HeartBeat: public Animation {
public:
  HeartBeat(uint32_t bpm):
    bpm(bpm)
  {}
  ~HeartBeat() {}

  uint32_t bpm;

  virtual void update() {
    uint32_t period = 60000L/bpm;
    uint32_t to = (clock->currentMS % period);
    uint32_t ti = ((clock->currentMS + (period/8)) % period);
    float xo = pow(M_E, -(float)to/(float)(period/4));
    float xi = pow(M_E, -(float)ti/(float)(period/4));
    uint32_t coloro = colors->interpolate(xo, 1.0, 0.0);
    uint32_t colori = colors->interpolate(xi, 1.0, 0.0);
    pixels->fillOuter(coloro);
    pixels->fillInner(colori);
  }
};


// Sparkle animation will randomly light the pixels on the
// rings.  Each pixel is assigned a random sine wave frequency
// so it smoothly moves between primary and secondary color.
// Note you must call randomizeFrequencies before using to
// setup the number of pixels and the range of random frequencies.
class Sparkle: public Animation {
public:
  Sparkle(uint16_t n, float f0, float f1) {
    randomizeFrequencies(n, f0, f1);
  }
  ~Sparkle() {
    if (_frequencies != NULL) {
      delete[] _frequencies;
    }
  }

  void randomizeFrequencies(uint16_t n, float f0, float f1) {
    if (_frequencies != NULL) {
      delete[] _frequencies;
    }
    _frequencies = new float[n];
    _n = n;
    for (int i=0; i<n; ++i) {
      _frequencies[i] = lerp(random(4096), 0, 4095, f0, f1);
    }
  }

  virtual void update() {
    for (int i=0; i<pixels->numPixels(); ++i) {
      float t = sin(2.0*PI*_frequencies[i]*clock->currentS);
      uint32_t color = colors->interpolate(t, -1.0, 1.0);
      pixels->setPixelColor(i, color);
    }
  }
  
private:
  float* _frequencies;
  uint16_t _n;
};

#endif
