# ReloadedSplit66/rev1u

See parent directory for common documentation.

This directory use USART configuration for split communication through the debug port
where USART1 TX/RX signals are available without need to remap the GPIOS.

So you need to connect both side using a 4 wires Molex PicoBlade cable on the
J4 connector (**The debug port**).

The J2 USB-MicroB port can be used, but the GPIO in `board.h` need to be reconfigured
to send USART1 to J2 and I2C or GPIO to J4.

After remapping, the B6/B7 pin are used. A 4 wire cross-cable can be easily made.
