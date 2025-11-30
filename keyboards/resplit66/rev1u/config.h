/* I don't want gpio to be reconfigured each time. */
#define MATRIX_UNSELECT_DRIVE_HIGH

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
