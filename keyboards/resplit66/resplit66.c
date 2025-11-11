/* Copyright 2025 David Girault
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "board.h"
#include "hal.h"
#include "matrix.h"
#include "quantum.h"
#include "split_util.h"
#include "usb_util.h"

/* I don't want gpio configuration altered at each loop.
 * This works with changes in matrix.c related to MATRIX_UNSELECT_DRIVE_HIGH.
 */
void matrix_init_pins(void) {}

/* V1 board has wrap to PH1 GPIO pin to select right side, but need pulldow because on
 * left side, the pin is floating. So activate pulldown while reading and deactivate it
 * afterward to avoid current leakage.
 */
static bool isRightSide(void)
{
  bool right;
  // Activate pulldown for unconnected side.
  gpio_set_pin_input_low(LINE_SIDE_SEL);
  wait_us(2);
  // Read the LINE_SIDE_SEL pin.
  right = gpio_read_pin(LINE_SIDE_SEL);
  // Now remove pulldown to avoid current leakage.
  gpio_set_pin_input(LINE_SIDE_SEL);
  return right;
}

bool is_keyboard_left_impl(void) {
  return !isRightSide();
}

bool is_keyboard_master_impl(void) {
  // USB-C connector is only on the left side.
  bool is_master = !isRightSide();
  // Avoid NO_USB_STARTUP_CHECK - Disable USB as the previous checks seem to enable it somehow
  if (!is_master) {
    usb_disconnect();
  }
  return is_master;
}

/* There is three leds per-side. */
void led_update_ports(led_t led_state)
{
  if (isLeftHand) {
    if (led_state.compose) {
      gpio_write_pin_high(LINE_LED1);
    } else {
      gpio_write_pin_low(LINE_LED1);
    }
  } else {
    if (led_state.caps_lock) {
      gpio_write_pin_high(LINE_LED3);
    } else {
      gpio_write_pin_low(LINE_LED3);
    }
    if (led_state.scroll_lock) {
      gpio_write_pin_high(LINE_LED2);
    } else {
      gpio_write_pin_low(LINE_LED2);
    }
    if (led_state.num_lock) {
      gpio_write_pin_high(LINE_LED1);
    } else {
      gpio_write_pin_low(LINE_LED1);
    }
  }
}
