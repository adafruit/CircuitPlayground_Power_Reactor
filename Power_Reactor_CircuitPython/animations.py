import math
import urandom


# Globals that all animations can use:
clock  = None
pixels = None
color  = None


class Fade:

    def __init__(self, frequency=1.0):
        self.frequency = frequency

    def update(self):
        x = math.sin(2.0*math.pi*self.frequency*clock.currentS)
        pixels.fill(color.interpolate(x, -1.0, 1.0))


class Chase:

    def __init__(self, frequency=1.0):
        self.frequency = frequency

    def update(self):
        phase_scale = 2.0*math.pi/pixels.n
        sin_scale = 2.0*math.pi*self.frequency
        for i in range(pixels.n):
            x = math.sin(sin_scale*clock.currentS + (phase_scale*i))
            pixels.set_pixel_rgb(i, color.interpolate_component(0, x, -1.0, 1.0),
                                    color.interpolate_component(1, x, -1.0, 1.0),
                                    color.interpolate_component(2, x, -1.0, 1.0))

class Sparkle:

    def __init__(self, n, min_freq=0.5, max_freq=1.5):
        self.frequencies = list(map(lambda x: urandom.uniform(min_freq, max_freq),
                                    range(n)))

    def update(self):
        for i in range(pixels.n):
            x = math.sin(2.0*math.pi*self.frequencies[i]*clock.currentS)
            pixels.set_pixel_rgb(i, color.interpolate_component(0, x, -1.0, 1.0),
                                    color.interpolate_component(1, x, -1.0, 1.0),
                                    color.interpolate_component(2, x, -1.0, 1.0))

class HeartBeat:

    def __init__(self, bpm=60):
        self.bpm = bpm
        self.period = 60.0/bpm
        self.quarter_period = self.period/4.0
        self.beat_phase = self.period/8.0

    def update(self):
        t = clock.currentS % self.period
        x = math.pow(math.e, -t/self.quarter_period)
        pixels.outer.fill(color.interpolate(x, 1.0, 0.0))
        t = (clock.currentS + self.beat_phase) % self.period
        x = math.pow(math.e, -t/self.quarter_period)
        pixels.inner.fill(color.interpolate(x, 1.0, 0.0))
