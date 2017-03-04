#ifndef LERP_H
#define LERP_H

// Linear interpolation of a value x within range x0, x1 to value
// y within range y0, y1.
inline float lerp(float x, float x0, float x1, float y0, float y1) {
  return y0+(x-x0)*((y1-y0)/(x1-x0));
}

#endif
