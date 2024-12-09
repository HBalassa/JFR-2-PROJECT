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
#define LOW 0
#define HIGH 1


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
void enable_pullup_resistor(void);
void set_output(uint8_t output);
void reset_presence_pulse(void);

/******************************************************************************
* Local Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         void enable_pullup_resistor()
* Description:		enable pullup resistor on the given IO PORT
* Input:			-
* Output:			-
* Notes:			- 
******************************************************************************/
void enable_pullup_resistor(void) {
	/*
	Melyik PIN-en van rajta a thermometer?
	Azt a PORT-ot kell beallitani majd ugy, hogy a pullup resistor rajta legyen:
	DDRx = (0<<Pxn);
	PORTx = (1<<Pxn);
	*/
}

/******************************************************************************
* Function:         void set_output(uint8_t output);
* Description:		set output HIGH/LOW on a given IO PORT
* Input:			uint8_t output -> HIGH = 1; LOW = 0
* Output:			-
* Notes:			- 
******************************************************************************/
void set_output(uint8_t output) {
	/*
	DDRx = (1<<Pxn);
	*/
	if(output == HIGH) /*PORTx = (1<<Pxn); */
	else if(output == LOW) /* PORTx = (0<<Pxn); */
}

/******************************************************************************
* Function:         void reset_pulse(void)
* Description:		master initiates a reset pulse to the 1-wire communication
					then slave initiates a presence pulse to the 1-wire communication
* Input:			-
* Output:			-
* Notes:			-
******************************************************************************/
void reset_presence_pulse(void) {
	set_output(LOW);
	_delay_us(480);
	enable_pullup_resistor();
	_delay_us(480);
}

/******************************************************************************
* Global Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         
* Description:
* Input:
* Output:
* Notes:
******************************************************************************/



/******************************************************************************
* Interrupt Routines
******************************************************************************/






