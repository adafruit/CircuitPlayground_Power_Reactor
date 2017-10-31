import board
import digitalio 

import neopixel


pixels = neopixel.NeoPixel(board.NEOPIXEL, 10)
left_button = digitalio.DigitalInOut(board.BUTTON_A)
left_button.switch_to_input(pull=digitalio.Pull.DOWN)

right_button = digitalio.DigitalInOut(board.BUTTON_B)
right_button.switch_to_input(pull=digitalio.Pull.DOWN)

slide_switch = digitalio.DigitalInOut(board.SLIDE_SWITCH)
slide_switch.switch_to_input(pull=digitalio.Pull.UP)
