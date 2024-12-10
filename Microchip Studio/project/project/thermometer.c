/******************************************************************************
* Include files
******************************************************************************/
#include "thermometer.h"
#include <avr/io.h>
#include <inttypes.h>
#include <avr/delay.h>

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
void set_input(void);
void set_output(uint8_t output);
void reset_presence_pulse(void);
void write_slot(uint8_t bit);
uint8_t read_slot(void);
void write_8_bit(uint8_t data);

/******************************************************************************
* Local Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         void set_input(void)
* Description:		sets PB0 to input
* Input:			-
* Output:			-
* Notes:			- 
******************************************************************************/
void set_input(void) {
	DDRB = (0<<PB0);
	PORTB = (0<<PB0);
}

/******************************************************************************
* Function:         void set_output(uint8_t output);
* Description:		sets PB0 to output
* Input:			uint8_t output -> HIGH = 1; LOW = 0
* Output:			-
* Notes:			- 
******************************************************************************/
void set_output(uint8_t output) {
	DDRB = (1<<PB0);
	if(output == HIGH) PORTB = (1<<PB0);
	else if(output == LOW) PORTB = (0<<PB0);
}

/******************************************************************************
* Function:         void reset_presence_pulse(void)
* Description:		master initiates a reset pulse to the 1-wire communication
					then slave initiates a presence pulse to the 1-wire communication
* Input:			-
* Output:			-
* Notes:			timing is explained in DS18S20 datasheet p. 13
******************************************************************************/
void reset_presence_pulse(void) {
	uint8_t presence_succeded = FALSE;
	while(!presence_succeded) {
		set_output(LOW);
		_delay_us(500);
		set_input();
		_delay_us(70);
		if((PINB & (1<<PB0)) == 0) presence_succeded = TRUE;
		_delay_us(430);
	}
}

/******************************************************************************
* Function:         void write_slot(uint8_t bit);
* Description:		1-wire communication write slot
* Input:			uint8_t bit -> the bit we want to transmit; either 0 or 1
* Output:			-
* Notes:			timing is explained in DS18S20 datasheet pp. 13-14
******************************************************************************/
void write_slot(uint8_t bit) {
	// the slot must be at least 60us long
	// there must be at least 1us between write slots
	// to write 1 -> master must release the bus within 15us
	// to write 0 -> master must continue to hold the bus LOW
	
	set_output(LOW);
	if(bit == 1) {
		_delay_us(5);
		set_input();
		_delay_us(75);
	} else if(bit == 0) {
		_delay_us(80);
		set_input();
	}
	_delay_us(2);
}

/******************************************************************************
* Function:         uint8_t read_slot(void);
* Description:		1-wire communication read slot
* Input:			-
* Output:			uint8_t -> the bit we read; either 0 or 1
* Notes:			timing is explained in DS18S20 datasheet pp. 14-15
******************************************************************************/
uint8_t read_slot(void) {
	// the slot must be at least 60us long
	// there must be at least 1us between write slots
	// slot is initiated by pulling the bus LOW for at least 1us
	// output data is valid for 15us after the read slot has been initiated
	// we read the data at the 12th us after the read slot's initiation
	
	set_output(LOW);
	_delay_us(2);
	set_input();
	_delay_us(10);
	if((PINB & (1<<PB0)) == 0) return 0;
	else if((PINB & (1<<PB0)) == (1<<PB0)) return 1;
	_delay_us(70);
}

/******************************************************************************
* Function:         void write_8_bit(uint8_t data);
* Description:		writes 8-bit data bitwise to the 1-wire bus
* Input:			uint8_t data -> the data we want to write (at most 8-bit!)
* Output:			-
* Notes:			writing starts with the LEAST SIGNIFICANT BIT first
******************************************************************************/
void write_8_bit(uint8_t data) {
	for(uint8_t i=0; i<8; i++) {
		if((data & (1<<i)) == 0) write_slot(0);
		else if((data & (1<<i)) == (1<<i)) write_slot(1);
	}
}

/******************************************************************************
* Global Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         int16_t read_temperature(void);
* Description:		reads the temperature data from the DS18S20
* Input:			-
* Output:			int16_t -> the data read in signed two's complement format
* Notes:			flow chart explained in DS18S20 datasheet pp. 16-19
******************************************************************************/
int16_t read_temperature(void) {
	int16_t temperature = 0;
	
	// Temperature conversion
	reset_presence_pulse();
	write_8_bit(SKIP_ROM);
	write_8_bit(CONVERT_T);
	while(!read_slot());
	
	// Read data
	reset_presence_pulse();
	write_8_bit(SKIP_ROM);
	write_8_bit(READ_SCRATCHPAD);
	for(uint8_t i=0; i<16; i++) {
		temperature |= (read_slot()<<i);
	}
	
	// End of communication
	reset_presence_pulse();
	
	return temperature;
}


/******************************************************************************
* Interrupt Routines
******************************************************************************/






