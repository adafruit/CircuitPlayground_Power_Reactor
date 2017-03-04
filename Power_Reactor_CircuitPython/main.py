# Circuit Playground SAMD21/Express CircuitPython 'Power' Reactor Sketch
# Author: Tony DiCola
# License: MIT License (https://opensource.org/licenses/MIT)
import time
import urandom

import board
import nativeio

import circuitplayground
import neopixel

import animations
import colors
import reactor


# Reactor configuration:
COLOR_PERIOD_S       = 15    # Number of seconds before changing to random color.
ANIMATION_PERIOD_S   = 30    # Number of seconds before changing to random animation.

# NeoPixel ring configuration:
# (inner ring is already defined by circuitplayground.py)
OUTER_PIN    = board.SCL   # Input pin of the outer neopixel ring.
OUTER_COUNT  = 24          # Number of neopixles in outer ring.


# Globals:
outer = neopixel.NeoPixel(OUTER_PIN, OUTER_COUNT)
clock = reactor.Clock()
pixels = reactor.Pixels(circuitplayground.pixels, outer)

# Define animations:
_animations = [animations.Fade(0.66),
               animations.Chase(0.66),
               animations.HeartBeat(60),
               animations.Sparkle(pixels.n, 0.25, 1.25)]
currentAnimation = 0
elapsedAnimation = 0.0

# Colors are defined in colors.py for space efficiency (packed into .mpy file).
currentColor = 0
elapsedColor = 0.0

# Setup code:
# Initialize random number generator with light sensor value:
with nativeio.AnalogIn(board.LIGHT) as light:
    urandom.seed(light.value)
# Clear all the pixels.
pixels.fill((0,0,0))
pixels.write()
# Setup animation globals:
animations.clock = clock
animations.pixels = pixels

# Main loop:
while True:
    # Check if slide switch is off and turn off all the pixels.
    if not circuitplayground.slide_switch.value:
        # Turn off pixels and sleep (ideally this would deep sleep the CPU
        # but for now it just spins in a loop internally).
        pixels.fill((0,0,0))
        pixels.write()
        time.sleep(0.01)
        continue

    # Get initial button state.
    left_first = circuitplayground.left_button.value
    right_first = circuitplayground.right_button.value

    # Update clock and current animation (being careful to update the current
    # color for the animation).
    clock.update()
    animations.color = colors.colors[currentColor]
    _animations[currentAnimation].update()
    pixels.write()

    # Grab a second button state and look for button releases.
    left_second = circuitplayground.left_button.value
    right_second = circuitplayground.right_button.value

    # Increment animation when left button released.
    if left_first and not left_second:
        currentAnimation = (currentAnimation + 1) % len(_animations)
        # Reset time to next animation/color after button press.
        elapsedAnimation = 0.0
        elapsedColor = 0.0

    # Increment color when right button released.
    if right_first and not right_second:
        currentColor = (currentColor + 1) % len(colors.colors)
        # Reset time to next animation/color after button press.
        elapsedAnimation = 0.0
        elapsedColor = 0.0

    # Update elapsed time for current color and animation.
    elapsedColor += clock.elapsedS
    elapsedAnimation += clock.elapsedS

    # Change to random color after specified period.
    if elapsedColor >= COLOR_PERIOD_S:
        currentColor = urandom.randint(0, len(colors.colors)-1)
        elapsedColor = 0.0

    # Change to random animation after specified period.
    if elapsedAnimation >= ANIMATION_PERIOD_S:
        currentAnimation = urandom.randint(0, len(_animations)-1)
        elapsedAnimation = 0.0
