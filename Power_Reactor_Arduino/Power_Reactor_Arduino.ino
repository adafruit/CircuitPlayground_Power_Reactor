// Circuit Playground 32u4 'Power' Reactor Sketch
// Author: Tony DiCola
// License: MIT License (https://opensource.org/licenses/MIT)
#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SleepyDog.h>

#include "Animations.h"
#include "Colors.h"
#include "Reactor.h"


// Reactor configuration:
#define BRIGHTNESS            100   // NeoPixel brightness
#define COLOR_PERIOD_S        15    // Number of seconds before changing to random color.
#define ANIMATION_PERIOD_S    30    // Number of seconds before changing to random animation.

// NeoPixel ring configuration:
#define INNER_PIN     17  // Input pin of the inner neopixel ring (on circuit playground)
#define INNER_COUNT   10  // Number of neopixels in inner ring.
#define INNER_TYPE    NEO_GRB + NEO_KHZ800  // Type of neopixels in inner ring (see strandtest)

#define OUTER_PIN     3   // Input pin of the outer neopixel ring (around circuit playground)
#define OUTER_COUNT   24  // Number of neopixles in outer ring.
#define OUTER_TYPE    NEO_GRB + NEO_KHZ800  // Type of neopixels in outer ring (see strandtest)


// Globals:
const uint32_t colorPeriodMS = (uint32_t)COLOR_PERIOD_S * 1000L;
const uint32_t animationPeriodMS = (uint32_t)ANIMATION_PERIOD_S * 1000L;
Adafruit_NeoPixel inner = Adafruit_NeoPixel(INNER_COUNT, INNER_PIN, INNER_TYPE);
Adafruit_NeoPixel outer = Adafruit_NeoPixel(OUTER_COUNT, OUTER_PIN, OUTER_TYPE);
Clock clock;
Pixels pixels(&inner, &outer);

// Define animations:
Glow glow(0.66);
Chase chase(1.33);
HeartBeat beat(60);
Sparkle sparkle(pixels.numPixels(), 0.25, 1.25);
Animation* animations[] = { &glow, &chase, &beat, &sparkle };
#define ANIMATIONS_LENGTH (sizeof(animations)/sizeof(Animation*))
int currentAnimation = 0;
uint32_t elapsedAnimationMS = 0;

// Define colors:
Gradient colors[] = {
  // Single color to black gradients:
  Gradient(ColorGamma(255, 0, 0),   ColorGamma(0, 0, 0)),      // Red
  Gradient(ColorGamma(255, 127, 0), ColorGamma(0, 0, 0)),      // Orange
  Gradient(ColorGamma(255, 255, 0), ColorGamma(0, 0, 0)),      // Yellow
  Gradient(ColorGamma(127, 255, 0), ColorGamma(0, 0, 0)),      // Green-Yellow
  Gradient(ColorGamma(0, 255, 0),   ColorGamma(0, 0, 0)),      // Green
  Gradient(ColorGamma(0, 255, 127), ColorGamma(0, 0, 0)),      // Green-Cyan
  Gradient(ColorGamma(0, 255, 255), ColorGamma(0, 0, 0)),      // Cyan
  Gradient(ColorGamma(0, 127, 255), ColorGamma(0, 0, 0)),      // Blue-Cyan
  Gradient(ColorGamma(0, 0, 255),   ColorGamma(0, 0, 0)),      // Blue
  Gradient(ColorGamma(127, 0, 255), ColorGamma(0, 0, 0)),      // Blue-Magenta
  Gradient(ColorGamma(255, 0, 255), ColorGamma(0, 0, 0)),      // Magenta
  Gradient(ColorGamma(255, 0, 127), ColorGamma(0, 0, 0)),      // Red-Magenta
  // Complimentary colors:
  Gradient(ColorGamma(255, 0, 0),   ColorGamma(0, 255, 255)),    // Red - Cyan
  Gradient(ColorGamma(255, 127, 0), ColorGamma(0, 127, 255)),  // Orange - Blue-Cyan
  Gradient(ColorGamma(255, 255, 0), ColorGamma(0, 0, 255)),    // Yellow - Blue
  Gradient(ColorGamma(127, 255, 0), ColorGamma(127, 0, 255)),  // Green-Yellow - Blue-Magenta
  Gradient(ColorGamma(0, 255, 0),   ColorGamma(255, 0, 255)),  // Green - Magenta
  Gradient(ColorGamma(0, 255, 127), ColorGamma(255, 0, 127)),  // Green-Cyan - Red-Magenta
  // Secondary colors:
  Gradient(ColorGamma(255, 0, 0),   ColorGamma(0, 255, 0)),      // Red - Green
  Gradient(ColorGamma(255, 127, 0), ColorGamma(0, 255, 127)),  // Orange - Green-Cyan
  Gradient(ColorGamma(255, 255, 0), ColorGamma(0, 255, 255)),  // Yellow - Cyan
  Gradient(ColorGamma(127, 255, 0), ColorGamma(0, 127, 255)),  // Green-Yellow - Blue-Cyan
  Gradient(ColorGamma(0, 255, 0),   ColorGamma(0, 0, 255)),    // Green - Blue
  Gradient(ColorGamma(0, 255, 127), ColorGamma(127, 0, 255)),  // Green-Cyan - Blue-Magenta
  Gradient(ColorGamma(0, 255, 255), ColorGamma(255, 0, 255)),  // Cyan - Magenta
  Gradient(ColorGamma(0, 127, 255), ColorGamma(255, 0, 127)),  // Blue-Cyan - Red-Magenta
  Gradient(ColorGamma(0, 0, 255),   ColorGamma(255, 0, 0)),    // Blue - Red
  Gradient(ColorGamma(127, 0, 255), ColorGamma(255, 127, 0)),  // Blue-Magenta - Orange
  Gradient(ColorGamma(255, 0, 255), ColorGamma(255, 255, 0)),  // Magenta - Yellow
  Gradient(ColorGamma(255, 0, 127), ColorGamma(127, 255, 0)),  // Red-Magenta - Green-Yellow
};
int currentColor = 0;
uint32_t elapsedColorMS = 0;
#define COLORS_LENGTH (sizeof(colors)/sizeof(colors[0]))


void setup() {
  // Initialize random number generator with light sensor value.
  randomSeed(analogRead(0));
  // Initialize other parts of board (serial output, circuit playground library).
  Serial.begin(115200);
  CircuitPlayground.begin();
  // Initialize neopixel rings and clear them.
  inner.begin();
  outer.begin();
  inner.show();
  outer.show();
  // Adjust pixel brightness.
  pixels.setBrightness(BRIGHTNESS);
  // Initialize animations.
  for (int i=0; i<ANIMATIONS_LENGTH; ++i) {
    animations[i]->pixels = &pixels;
    animations[i]->clock = &clock;
  }
}

void loop() {
  // Check if slide switch is on the left (false) and go to sleep.
  while (!CircuitPlayground.slideSwitch()) {
    // Turn off the pixels, then go into deep sleep for a second.
    pixels.fill(0);
    pixels.show();
    Watchdog.sleep(1000);
  }
  
  // Grab an initial button state.
  bool leftFirst = CircuitPlayground.leftButton();
  bool rightFirst = CircuitPlayground.rightButton();
  
  // Update clock and then animation (making sure to update to the current color
  // for the current animation).
  clock.update();
  animations[currentAnimation]->colors = &colors[currentColor];
  animations[currentAnimation]->update();
  pixels.show();
  
  // Grab a second button state and look for button releases.
  delay(10);  // Small delay for debouncing.
  bool leftSecond = CircuitPlayground.leftButton();
  bool rightSecond = CircuitPlayground.rightButton();
  
  // Increment animation when left button is released.
  if (leftFirst && !leftSecond) {
    currentAnimation = (currentAnimation + 1) % ANIMATIONS_LENGTH;
    // Reset time to next animation/color after button press.
    elapsedColorMS = 0;
    elapsedAnimationMS = 0;
  }
  
  // Increment color when right button is released.
  if (rightFirst && !rightSecond) {
    currentColor = (currentColor + 1) % COLORS_LENGTH;
    // Reset time to next animation/color after button press.
    elapsedColorMS = 0;
    elapsedAnimationMS = 0;
  }
  
  // Update elapsed time for current color and animation.
  elapsedColorMS += clock.deltaMS;
  elapsedAnimationMS += clock.deltaMS;
  
  // Change to random color after specified period.
  if (elapsedColorMS >= colorPeriodMS) {
    currentColor = random(COLORS_LENGTH);
    elapsedColorMS = 0;
  }
  
  // Change to random animation after specified period.
  if (elapsedAnimationMS >= animationPeriodMS) {
    currentAnimation = random(ANIMATIONS_LENGTH);
    elapsedAnimationMS = 0;
  }
}

