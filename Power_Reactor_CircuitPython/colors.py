import math


def lerp(x, x0, x1, y0, y1):
    return y0+(x-x0)*((y1-y0)/(x1-x0))


gamma8 = bytearray(256)
for i in range(len(gamma8)):
    gamma8[i] = int(math.pow(i/255.0, 2.8)*255.0+0.5) & 0xFF


class Gradient:

    def __init__(self, primary=(0,0,0), secondary=(0,0,0)):
        self.primary = primary
        self.secondary = secondary

    def interpolate(self, x, x0=0.0, x1=1.0):
        return tuple(map(lambda c: gamma8[int(lerp(x, x0, x1, c[0], c[1]))],
                     zip(self.primary, self.secondary)))

    def interpolate_component(self, c, x, x0=0.0, x1=1.0):
        return gamma8[int(lerp(x, x0, x1, self.primary[c], self.secondary[c]))]


colors = [
  # Single color to black gradients:
  Gradient((255, 0, 0), (0, 0, 0)),        # Red
  Gradient((255, 127, 0), (0, 0, 0)),      # Orange
  Gradient((255, 255, 0), (0, 0, 0)),      # Yellow
  Gradient((127, 255, 0), (0, 0, 0)),      # Green-Yellow
  Gradient((0, 255, 0), (0, 0, 0)),        # Green
  Gradient((0, 255, 127), (0, 0, 0)),      # Green-Cyan
  Gradient((0, 255, 255), (0, 0, 0)),      # Cyan
  Gradient((0, 127, 255), (0, 0, 0)),      # Blue-Cyan
  Gradient((0, 0, 255), (0, 0, 0)),        # Blue
  Gradient((127, 0, 255), (0, 0, 0)),      # Blue-Magenta
  Gradient((255, 0, 255), (0, 0, 0)),      # Magenta
  Gradient((255, 0, 127), (0, 0, 0)),      # Red-Magenta
  # Complimentary colors:
  Gradient((255, 0, 0), (0, 255, 255)),    # Red - Cyan
  Gradient((255, 127, 0), (0, 127, 255)),  # Orange - Blue-Cyan
  Gradient((255, 255, 0), (0, 0, 255)),    # Yellow - Blue
  Gradient((127, 255, 0), (127, 0, 255)),  # Green-Yellow - Blue-Magenta
  Gradient((0, 255, 0), (255, 0, 255)),    # Green - Magenta
  Gradient((0, 255, 127), (255, 0, 127)),  # Green-Cyan - Red-Magenta
  # Secondary colors:
  Gradient((255, 0, 0), (0, 255, 0)),      # Red - Green
  Gradient((255, 127, 0), (0, 255, 127)),  # Orange - Green-Cyan
  Gradient((255, 255, 0), (0, 255, 255)),  # Yellow - Cyan
  Gradient((127, 255, 0), (0, 127, 255)),  # Green-Yellow - Blue-Cyan
  Gradient((0, 255, 0), (0, 0, 255)),      # Green - Blue
  Gradient((0, 255, 127), (127, 0, 255)),  # Green-Cyan - Blue-Magenta
  Gradient((0, 255, 255), (255, 0, 255)),  # Cyan - Magenta
  Gradient((0, 127, 255), (255, 0, 127)),  # Blue-Cyan - Red-Magenta
  Gradient((0, 0, 255), (255, 0, 0)),      # Blue - Red
  Gradient((127, 0, 255), (255, 127, 0)),  # Blue-Magenta - Orange
  Gradient((255, 0, 255), (255, 255, 0)),  # Magenta - Yellow
  Gradient((255, 0, 127), (127, 255, 0))   # Red-Magenta - Green-Yellow
]
