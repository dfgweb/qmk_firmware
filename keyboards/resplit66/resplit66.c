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

#include "_wait.h"
#include "board.h"
#include "gpio.h"
#include "hal.h"
#include "keycodes.h"
#include "matrix.h"
#include "oled_driver.h"
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

/* RGB matrix is an optional board placed under the main board. */
#ifdef RGB_MATRIX_ENABLE
/* Default function change OSPEEDR register and remove HIGH SPEED for MOSI! */
void ws2812_gpio_init(void) {}

void keyboard_post_init_kb(void) {
  /* Power-on RGB leds and wait for a bit before allowing RGB to continue. */
  gpio_write_pin_high(LINE_RGB_PWR);
  wait_us(10);
  /* Offload to the user func */
  keyboard_post_init_user();
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  uint8_t layer = get_highest_layer(layer_state);
  if (layer > 0) {
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
      for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
        uint8_t index = g_led_config.matrix_co[row][col];
        if (index >= led_min && index < led_max && index != NO_LED) {
            uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){col, row});
            if (kc <= KC_TRNS) continue;
            switch (kc) {
                case KC_F1 ... KC_F12: // red
                    rgb_matrix_set_color(index, 0xc0, 0, 0);
                    break;
                case KC_NUM_LOCK ... KC_KP_DOT: // green
                    rgb_matrix_set_color(index, 0, 0xc0, 0);
                    break;
                case KC_PRINT_SCREEN ... KC_UP: // blue
                    rgb_matrix_set_color(index, 0, 0, 0xc0);
                    break;
                case CONSUMER_KEYCODE_RANGE: // yellow
                    rgb_matrix_set_color(index, 0xc0, 0xc0, 0);
                    break;
                case MOUSE_KEYCODE_RANGE: // cyan
                    rgb_matrix_set_color(index, 0, 0xc0, 0xc0);
                    break;
                case RGB_MATRIX_KEYCODE_RANGE: // purple
                    rgb_matrix_set_color(index, 0xc0, 0, 0xc0);
                    break;
                default:
                    rgb_matrix_set_color(index, 0xc0, 0xc0, 0xc0);
            }
        }
      }
    }
  }
  return false;
}
#endif

#ifdef OLED_ENABLE
/* Default function change OSPEEDR register and remove HIGH SPEED for SCL/SDA! */
void i2c_init(void) {}

static void render_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
  };
  oled_write_P(qmk_logo, false);
}

/* Defines names for use in layer keycodes and the keymap */
enum layer_names { _BASE, _NAV, _FN};

static void render_status(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer: "), false);

  switch (get_highest_layer(layer_state)) {
  case _BASE:
    oled_write_P(PSTR("Default\n"), false);
    break;
  case _NAV:
    oled_write_P(PSTR("NAV\n"), false);
    break;
  case _FN:
    oled_write_P(PSTR("FN\n"), false);
    break;
  default:
    // Or use the write_ln shortcut over adding '\n' to the end of your string
    oled_write_ln_P(PSTR("Undefined"), false);
  }
  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
  oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
  oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_180;
    }
    return OLED_ROTATION_0;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_logo();  // Renders a static logo
        oled_scroll_left();  // Turns on scrolling
    }
    return false;
}
#endif
