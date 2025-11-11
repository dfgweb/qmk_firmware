/* Copyright 2020 QMK
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

#pragma once

#include_next <mcuconf.h>

#undef STM32_HSI16_ENABLED
#define STM32_HSI16_ENABLED FALSE

#undef STM32_HSI48_ENABLED
#define STM32_HSI48_ENABLED FALSE

#undef STM32_LSI_ENABLED
#define STM32_LSI_ENABLED TRUE

#undef STM32_HSE_ENABLED
#define STM32_HSE_ENABLED TRUE

#undef STM32_PLLSRC
#define STM32_PLLSRC STM32_PLLSRC_HSE

#undef STM32_PLLM_VALUE
#define STM32_PLLM_VALUE 2  // 4Mhz input

#undef STM32_PLLN_VALUE
#define STM32_PLLN_VALUE 80 // VCO = 320Mhz

#undef STM32_PLLPDIV_VALUE
#define STM32_PLLPDIV_VALUE 10 // 32Mhz

#undef STM32_PLLP_VALUE
#define STM32_PLLP_VALUE 7  // NA.

#undef STM32_PLLQ_VALUE
#define STM32_PLLQ_VALUE 4  // 80Mhz

#undef STM32_PLLR_VALUE
#define STM32_PLLR_VALUE 4  // 80Mhz

#undef STM32_PLLSAI1N_VALUE
#define STM32_PLLSAI1N_VALUE 72 // VCO = 288Mhz

#undef STM32_PLLSAI1PDIV_VALUE
#define STM32_PLLSAI1PDIV_VALUE 6  // 48Mhz

#undef STM32_PLLSAI1P_VALUE
#define STM32_PLLSAI1P_VALUE 7  // NA.

#undef STM32_PLLSAI1Q_VALUE
#define STM32_PLLSAI1Q_VALUE 6  // 48Mhz

#undef STM32_PLLSAI1R_VALUE
#define STM32_PLLSAI1R_VALUE 6  // 48Mhz

#undef STM32_CLK48SEL
#define STM32_CLK48SEL STM32_CLK48SEL_PLLSAI1

#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1 TRUE
