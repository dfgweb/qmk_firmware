/* I don't want gpio to be reconfigured each time. */
#define MATRIX_UNSELECT_DRIVE_HIGH
/* Reduce delay to 4 cycles (50ns)  when clock is 80Mhz. */
#define GPIO_INPUT_PIN_DELAY (CPU_CLOCK / 1000000L / 20)

/* Mandatory parameters for serial_uart.c */
#define SERIAL_USART_FULL_DUPLEX
/* Optional parameter to be faster than default in serial_usart.h */
#define SERIAL_USART_SPEED 921600
/* We use the debug port USART on rev1 */
#define UART_TX_PIN A9
#define UART_RX_PIN A10

/* For RGB matrix using qmk ws2812 driver */
#define WS2812_SPI_DRIVER SPID2
#define WS2812_SPI_DIVISOR 32

/* For OLED using qmk default driver */
#define I2C_DRIVER I2CD2
#define I2C1_SCL_PIN B10
#define I2C1_SDA_PIN B11

/* From STM32CubeMX project used for Test FW:
I2C1.I2C_Speed_Mode=I2C_Fast
I2C1.IPParameters=I2C_Speed_Mode,Timing
I2C1.Timing=0x00702991
I2C2.IPParameters=Timing
I2C2.Timing=0x10909CEC
*/
/* Use FastMode */
#define I2C1_TIMINGR_PRESC  0x00u // <<28
#define I2C1_TIMINGR_SCLDEL 0x07u // <<20
#define I2C1_TIMINGR_SDADEL 0x00u // <<16
#define I2C1_TIMINGR_SCLH   0x29u // << 8
#define I2C1_TIMINGR_SCLL   0x91u // << 0

#define OLED_DISPLAY_128X64
