/******************************************************************************
 * Created:		05-09-2024 12h
 * Author :		Balassa Hodi
 * File name:	gyak01_09.05
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include<avr/io.h>
#include<avr/interrupt.h> 
#include<avr/delay.h>
#include<inttypes.h>

/******************************************************************************
* Macros
******************************************************************************/
#define F_CPU 8000000UL
#define TRUE 1
#define FALSE 0

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint8_t task_10ms = FALSE, task_100ms = FALSE, task_500ms = FALSE;
uint16_t timer_cnt = 0;

/******************************************************************************
* External Variables
******************************************************************************/


/******************************************************************************
* Local Function Declarations
******************************************************************************/
void port_init(void);
void timer_init(void);
void external_interrupt_init(void);

/******************************************************************************
* Local Function Definitions
******************************************************************************/

/******************************************************************************
* Function:         int main(void)
* Description:      main function
* Input:            
* Output:           
* Notes:            
******************************************************************************/
int main(void) {
	port_init();
	timer_init();
	external_interrupt_init();
	sei(); // set enable interrupt (mindig kell ha interrupt rutin van a scriptben)
	
	/* Replace with your application code */
	while(1) {
		/*PORTF = PORTF ^ 0x01; // PORTF 0. bitjet villogtatjuk
		_delay_ms(500); // nem hatekony, mert ilyenkor a mikrovezerlo nem tud mast csinalni */
		
		if(task_10ms == TRUE) {
			PORTF ^= 0x01;
			task_10ms = FALSE;
		}
		
		if(task_100ms == TRUE) {
			PORTF = PORTF ^ (1<<PF1);
			task_100ms = FALSE;
		}
		
		if(task_500ms == TRUE) {
			PORTF = PORTF ^ (1<<PF2);
			task_500ms = FALSE;
		}
	}
}

/******************************************************************************
* Function:         void port_init(void)
* Description:      initialize I/O ports
* Input:			-
* Output:			-
* Notes:			-
******************************************************************************/
void port_init(void) {
	DDRA = 0xff;
	PORTA = 0xff;
	
	DDRF = (1<<PF0) | (1<<PF1) | (1<<PF2) | (1<<PF3);
	PORTF = 0x0f; // ugyanaz mint a felette levo sor
	
	DDRD = (0<<PD0); // bemenet
	PORTD = (1<<PD0); // belso ellenallas bekapcsolva
}

/******************************************************************************
* Function:			void timer_init(void)
* Description:		initialize Timer0
* Input:			-
* Output:			-
* Notes:			-
******************************************************************************/
void timer_init(void) {
	TCCR0A = (1<<WGM01) | (0<<WGM00) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	OCR0A = 77; // igy lesz 100Hz-es a timer
	
	TIMSK0 = (1<<OCIE0A); // ezzel az interrupt mask-ot allitjuk be
}

/******************************************************************************
* Function:			void external_interrupt_init(void)
* Description:		initialize External interrupt
* Input:			-
* Output:			-
* Notes:			-
******************************************************************************/
void external_interrupt_init(void) {
	EICRA = (1<<ISC01) | (0<<ISC00); // ez letrehozza hogy a lefuto eleknel legyen az interrupt
	EIMSK = (1<<INT0);
}


/******************************************************************************
* Interrupt Routines
******************************************************************************/
ISR(TIMER0_COMP_vect) { // Timer0 compare match vector (ez kell nekunk)
	/*PORTA = PORTA ^ 0x01; // PORTA 0. bitjet villogtatjuk*/
	
	timer_cnt++;
	if((timer_cnt % 1) == 0) task_10ms = TRUE;
	if((timer_cnt % 10) == 0) task_100ms = TRUE;
	if((timer_cnt % 50) == 0) task_500ms = TRUE;
}

ISR(INT0_vect) {
	PORTA = PORTA ^ 0x01;
}