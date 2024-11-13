/******************************************************************************
* Include files
******************************************************************************/
#include "twi.h"
#include <avr/io.h>
#include <inttypes.h>
#include <math.h>

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
void twi_error(uint8_t);
uint8_t decimal_to_bcd(uint8_t);
uint8_t calc_day_of_week(uint16_t, uint8_t, uint8_t);

/******************************************************************************
* Local Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         void twi_error(uint8_t twsr_register);
* Description:		error routine if not the correct staus code happens
* Input:			TWSR register value
* Output:			-
* Notes:			-
******************************************************************************/
void twi_error(uint8_t twsr_register) {
	uint8_t status_code = twsr_register & 0xF8;

	switch(status_code) {
		case START:
		break;
		case REP_START:
		break;
		case MT_SLA_ACK:
		break;
		case MT_SLA_NACK:
		break;
		case MT_DATA_ACK:
		break;
		case MT_DATA_NACK:
		break;
		case ARB_LOST:
		break;
	}
}

/******************************************************************************
* Function:         uint8_t decimal_to_bcd(uint8_t dec_num);
* Description:		converts a decimal number to bcd format
* Input:			decimal number (must be between 0...99)
* Output:			the bcd format of the decimal number
* Notes:			-
******************************************************************************/
uint8_t decimal_to_bcd(uint8_t dec_num) {
	// to convert from dec to bcd, just simply convert every digit to a 4-bit binary number
	// the input is between 0...99
	if(dec_num > 99) return -1;

	// dec_num = second_digit*10 + first_digit
	uint8_t first_digit = dec_num % 10;
	uint8_t second_digit = dec_num / 10;

	// number in bcd
	uint8_t bcd_num = (second_digit<<4) | (first_digit);

	return bcd_num;
}

/******************************************************************************
* Function:         void bcd_error(void);
* Description:		bcd error routine
* Input:			-
* Output:			-
* Notes:			-
******************************************************************************/
void bcd_error(void) {}
	
/******************************************************************************
* Function:         
* Description:		
* Input:			
* Output:			
* Notes:			
******************************************************************************/
uint8_t calc_day_of_week(uint16_t year, uint8_t month, uint8_t date) {
	// output is between 0...6
	// day of the week is calculated by the following formula
	uint8_t century = year/100;
	uint8_t day_of_week = ((int)(date + floor(2.6*month-0.2) - 2*century + year + floor(year/4) + floor(century/4))) % 7;
	return day_of_week;
}


/******************************************************************************
* Global Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         void twi_init(void)
* Description:		sets up the SCL Clock frequency for the Two-wire Serial Interface
* Input:			-
* Output:			-
* Notes:			SCL Clock Frequency = 100 kHz
					calculated from excel
******************************************************************************/
void twi_init(void) {
	TWBR = 0x08;
	TWSR = (0<<TWPS1) | (0<<TWPS0);
}

/******************************************************************************
* Function:         void twi_mt_mode(uint8_t current_edit, uint8_t second, uint8_t minute, uint8_t hour, uint8_t date, uint8_t month, uint8_t year);
* Description:		Two-wire Serial Interface Master Transmitter mode
* Input:			current edit -> either TIME or DATE
					second, minute, hour, date, month, year -> data that we want to send to the slave
* Output:			-
* Notes:			-
******************************************************************************/
void twi_mt_mode(uint8_t current_edit, uint8_t second, uint8_t minute, uint8_t hour, uint8_t date, uint8_t month, uint8_t year) {
	// Generate START Condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the START condition has been transmitted
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != START) twi_error(TWSR);
	
	// Send slave address + write bit (0)
	TWDR = DS1307_ADR_W;
	
	// Clear TWINT to start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the SLA+W has been transmitted and ACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_SLA_ACK) twi_error(TWSR);
	
	// Send word address (which sets the register pointer on the DS1307)
	if(current_edit == TIME) {
		// Address of the first time register (Seconds)
		TWDR = SEC_ADR;
	} else if(current_edit == DATE) {
		// Address of the first date register (Day)
		TWDR = DAY_ADR;
	}
	
	// Clear TWINT to start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the data has been transmitted and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_DATA_ACK) twi_error(TWSR);
	

	// IMPORTANT! Data shall be written in BCD format!
	// Send the first data
	if(current_edit == TIME) {
		// Data for the seconds register
		if(decimal_to_bcd(second) == -1) bcd_error(); // do some error routine
		TWDR = decimal_to_bcd(second) | (0<<CH);
	} else if(current_edit == DATE) {
		// Data for the day register
		TWDR = calc_day_of_week(year, month, date);
	}
	
	// Clear TWINT to start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the data has been transmitted and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_DATA_ACK) twi_error(TWSR);
	
	// Send the second data
	if(current_edit == TIME) {
		// Data for the Minutes register
		if(decimal_to_bcd(minute) == -1) bcd_error();
		TWDR = decimal_to_bcd(minute);
	} else if(current_edit == DATE) {
		// Data for the Date register
		if(decimal_to_bcd(date) == -1) bcd_error();
		TWDR = decimal_to_bcd(date);
	}
	
	// Clear TWINT to start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the data has been transmitted and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_DATA_ACK) twi_error(TWSR);
	
	// Send the third data
	if(current_edit == TIME) {
		// Data for the Hours register. This is the last data for TIME registers
		if(decimal_to_bcd(hour) == -1) bcd_error();
		TWDR = (0<<HOURMODE) | decimal_to_bcd(hour);
	} else if(current_edit == DATE) {
		// Data for the Month register
		if(decimal_to_bcd(month) == -1) bcd_error();
		TWDR = decimal_to_bcd(month);
	}
	
	// Clear TWINT to start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the data has been transmitted and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_DATA_ACK) twi_error(TWSR);
	
	// Send the fourth data
	if(current_edit == DATE) {
		// Data for the Year Register (only the last two digits of the decimal 4-digit year is needed)
		uint8_t year_short = year % 100;
		if(decimal_to_bcd(year_short) == -1) bcd_error();
		TWDR = decimal_to_bcd(year_short);
	}
	
	// Clear TWINT to start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the data has been transmitted and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_DATA_ACK) twi_error(TWSR);
	
	// Generate STOP Condition
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

/******************************************************************************
* Function:         void twi_mr_mode(uint8_t current_edit, uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *date, uint8_t *month, uint8_t *year)
* Description:		Two-wire Serial Interface Master Receiver mode
* Input:			*second, *minute, *hour, *date, *month, *year -> pointer adresses of the variables that we change 
* Output:			-
* Notes:			-
******************************************************************************/
void twi_mr_mode(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *date, uint8_t *month, uint8_t *year) {
	// Generate START Condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the START condition has been transmitted
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != START) twi_error(TWSR);
	
	// First we set up the DS1307 Register pointer to point to the first register we want to read: the seconds
	// Send slave address + write bit (0)
	TWDR = DS1307_ADR_W;
	
	// Clear TWINT to start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the SLA+W has been transmitted and ACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_SLA_ACK) twi_error(TWSR);
	
	// Send word address (which sets the register pointer on the DS1307)
	TWDR = SEC_ADR;
	
	// Clear TWINT to start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// Wait for TWINT flag set, this indicates that the data has been transmitted and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));
	
	// Check the status register
	if((TWSR & 0xF8) != MT_DATA_ACK) twi_error(TWSR);
	
	// Generate REPEATED START Condition
}