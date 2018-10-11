# Main Controller

The main controller is built on top of a Raspberry Pi 3 +

## Setup I2C

raspi-config

## Test I2C

i2ctools:
1/ i2cdetect
2/ i2cset
   i2cset -y 1 0x08 123




## PS4 Controller

ds4drv

https://github.com/chrippa/ds4drv

Read input from /dev/input

sudo apt-get install joystick