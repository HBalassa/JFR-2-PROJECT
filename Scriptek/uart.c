/******************************************************************************
* Include files
******************************************************************************/
#include "uart.h"
#include <inttypes.h>
#include <avr/io.h>
/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/


/******************************************************************************
* External Variables
******************************************************************************/


/******************************************************************************
* Local Function Declarations
******************************************************************************/


/******************************************************************************
* Local Function Definitions
******************************************************************************/


/******************************************************************************
* Global Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         void uart0_init(uint16_t baud)
* Description:      UART0 felkonfigurálása
* Input:
* Output:
* Notes:
******************************************************************************/
void uart0_init(uint16_t baud)
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (baud>>8);
	UBRR0L = (unsigned char) baud;
	/* Set frame format: 8data, no parity & 1 stop bits */
	UCSR0C = (0<<UMSEL0) | (0<<UPM0) | (1<<USBS0) | (1<<UCSZ1) | (1<<UCSZ0);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

/******************************************************************************
* Function:         void uart_0_transmit(char data)
* Description:      karakter küldése uarton
* Input:
* Output:
* Notes:
******************************************************************************/
void uart_0_transmit(char data)
{
	/* Wait for empty transmit buffer */
	while ( ! ( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/******************************************************************************
* Function:        void uart_0_write_string(char *c)
* Description:     karaktersorozat kiküldése
* Input:
* Output:
* Notes:
******************************************************************************/
void uart_0_write_string(char *c)
{
	char *p = c;
	while(*p != 0) uart_0_transmit(*p++);
}