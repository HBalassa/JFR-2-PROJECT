/******************************************************************************
* Include files
******************************************************************************/
#include "peripherals.h"
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
* Function:         void timer_init(void)
* Description:      timer inicializálása
* Input:
* Output:
* Notes:			10 ms, CTC
******************************************************************************/
void timer_init(void)
{
	TCCR0A = (1<<WGM01) | (0<<WGM00) | (1<<CS2) | (0<<CS1) | (1<<CS0);
	OCR0A = 77;
	TIMSK0 = (1<<OCIE0A);
}

/******************************************************************************
* Function:         void external_int_init(void)
* Description:      küls? megszakítás inicializálása
* Input:
* Output:
* Notes:
******************************************************************************/
void external_int_init(void)
{
	EICRA = (1<<ISC01) | (0<<ISC00);
	EIMSK = (1<<INT0);
}

/******************************************************************************
* Function:         void adc_init(void)
* Description:      ADC konfigurálása
* Input:
* Output:
* Notes:
******************************************************************************/
void adc_init(void)
{
	ADMUX = 0;
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1<<ADSC);
}