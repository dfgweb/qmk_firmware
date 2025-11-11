# ReloadedSplit66/rev1i

See parent directory for common documentation.

This directory use I2C configuration for split communication using the USB-MicroB port.

It use a custom transport.c to allow multiple I2C bus usage (split comm & oled diplay).

Usage is deprecated since the used pull-ups in the only known keyboard to be made are too
weak to support correctly the Fast Mode I2C. This revision should be not used as base design
for another keyboard. It's a POC.

Make example for this keyboard (after setting up your build environment):

    make resplit66/rev1i:default

Flashing example for this keyboard:

    make resplit66/rev1i:default:flash
