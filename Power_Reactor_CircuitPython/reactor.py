import time


class Pixels:

    def __init__(self, inner, outer):
        self.inner = inner
        self.outer = outer
        self.n = self.inner.n + self.outer.n

    def fill(self, color):
        self.inner.fill(color)
        self.outer.fill(color)

    def write(self):
        self.inner.write()
        self.outer.write()

    def __setitem__(self, index, val):
        if index < 0 or index >= self.n:
            return
        if index < self.inner.n:
            self.inner[index] = val
        else:
            self.outer[index-self.inner.n] = val

    def __getitem__(self, index):
        if index < 0 or index >= self.n:
            return None
        if index < self.inner.n:
            return self.inner[index]
        else:
            return self.outer[index-self.inner.n]

    def set_pixel_rgb(self, index, red, green, blue):
        if index < 0 or index >= self.n:
            return None
        if index < self.inner.n:
            return self.inner.set_pixel_rgb(index, red, green, blue)
        else:
            return self.outer.set_pixel_rgb(index-self.inner.n, red, green, blue)


class Clock:

    def __init__(self):
        self.currentS = time.monotonic()
        self.currentMS = self.currentS*1000
        self.elapsedS = 0

    def update(self):
        currentS = time.monotonic()
        self.elapsedS = currentS - self.currentS
        self.currentS = currentS
        self.currentMS = self.currentS*1000
