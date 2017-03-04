import board
import nativeio

import neopixel


pixels = neopixel.NeoPixel(board.NEOPIXEL, 10)
left_button = nativeio.DigitalInOut(board.LEFT_BUTTON)
left_button.switch_to_input(pull=nativeio.DigitalInOut.Pull.DOWN)

right_button = nativeio.DigitalInOut(board.RIGHT_BUTTON)
right_button.switch_to_input(pull=nativeio.DigitalInOut.Pull.DOWN)

slide_switch = nativeio.DigitalInOut(board.SLIDE_SWITCH)
slide_switch.switch_to_input(pull=nativeio.DigitalInOut.Pull.UP)
