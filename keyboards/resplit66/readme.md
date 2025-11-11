# ReSplit66

![resplit66](https://gitlab.com/dgirault/resplit66/-/raw/7dbd4c0fc2389f283133b7b86e7c1f5aad2f50f1/img/freecad.png?inline=true)

ReSplit66 is a split orthogonal keyboard with 66 keys build with reversible PCBs:

- Each side use the same PCBs where keys, diodes, leds connectors can be places in both faces.
- Each side has two plates, the mainboard and the optionnal RGB leds boards.
- A wire jumper on mainboard is used to set left or right side.
- Master/slave connection can use:
  - [deprecated] I2C+GPIO using a 5 wires cable with two USB microB connectors (**This is not OTG cable**)
  - USART using 4 wires Molex PicoBlade cable. This is the configuration used in `rev1u`.
  - A 4 wires cable with two USB microB connectors after USART1 is remapped to B6/B7 pins.

Look in rev1i/rev1u sub-directory for specific details on using I2C or UART split communication.

* Keyboard Maintainer: [David Girault](https://github.com/dfgweb)
* Hardware Supported: [RESPLIT66_V1](https://gitlab.com/dgirault/resplit66)
* Hardware Availability: DIY

Make example for this keyboard (after setting up your build environment):

    make resplit66/rev1u:default

Flashing example for this keyboard:

    make resplit66/rev1u:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **BOOT_SW**: Hold button on the side of the keyboard while plug in the keyboard
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
* **JTAG**: Use the available JTAG port with the following script:

```shell
arm-none-eabi-gdb -nx --batch \
  -ex 'target extended-remote /dev/ttyACM0' \
  -ex 'monitor tpwr ena' \
  -ex 'monitor swdp_scan' \
  -ex 'attach 1' \
  -ex 'load' \
  -ex 'compare-sections' \
  -ex 'monitor tpwr dis' \
  -ex 'kill' \
  $1
```

Then, on both side:
```shell
./bmpflash.sh .build/resplit66_rev1u_default.elf
```
