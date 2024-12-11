/******************************************************************************
 * Created:		11-12-2024
 * Author :		Bence Bakonyvari & Balassa Hodi & Zalan Simon
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include <avr/io.h>

#define F_CPU 8000000UL
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>

#include "lcd.h"
#include "uart.h"
#include "peripherals.h"
#include "twi.h"
#include "thermometer.h"
#include "can.h"

/******************************************************************************
* Macros
******************************************************************************/
#define FALSE 0
#define TRUE 1

#define TIME 0
#define DATE 1

#define NOEDIT 2
#define EDIT 0
#define SAVE 1

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint16_t timer_cnt=0;
uint8_t task_10ms = FALSE, task_100ms = FALSE, task_500ms = FALSE, task_1s = FALSE;
uint16_t ad_result;

// button pressed
uint8_t PA0_pushed = FALSE;
uint8_t PA1_pushed = FALSE;
uint8_t PA2_pushed = FALSE;
uint8_t PA3_pushed = FALSE;
uint8_t PA4_pushed = FALSE;

// current edit
uint8_t curr_edit = NOEDIT;

// edit or save
uint8_t time_edit_save = SAVE;
uint8_t date_edit_save = SAVE;

// time variables
uint16_t time_0 = 0;
uint16_t time_1 = 0;

// time and date variables
uint8_t masodperc = 0;
uint8_t perc = 0;
uint8_t ora = 0;
uint8_t nap = 0;
uint8_t honap = 0;
uint16_t ev = 0;

// edited value variable
uint8_t edited_value = 0;
char edited_text[6];

// temperature variable
uint16_t homerseklet = 0;


/******************************************************************************
* External Variables
******************************************************************************/

/******************************************************************************
* Local Function Declarations
******************************************************************************/
void port_init(void);
uint8_t calc_month_length(uint8_t month, uint16_t year);

/******************************************************************************
* Local Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         void port_init(void)
* Description:      I/O portok inicializ?l?sa
* Input:
* Output:
* Notes:
******************************************************************************/
void port_init(void)
{
	DDRA = (0<<PA0) | (0<<PA1) | (0<<PA2) | (0<<PA3) | (0<<PA4);
	PORTA = (1<<PA0) | (1<<PA1) | (1<<PA2) | (1<<PA3) | (1<<PA4);
	
	DDRD = (0<<PD0);
	PORTD = (1<<PD0);
	
	DDRB = 0xff;
	PORTB = 0;
	
	DDRF = (1<<PF1) | (1<<PF2);
	PORTF = (1<<PF1) | (1<<PF2);
	
	DDRC = (1<<LCD_E) | (1<<LCD_RS) | (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5) | (1<<LCD_D4);
	PORTC = (0<<LCD_E) | (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
}

/******************************************************************************
* Function:         uint8_t calc_month_length(uint8_t date, uint8_t month, uint16_t year);
* Description:      calculates the length of a month
* Input:			month, year
* Output:			the length of the month, either: 28, 29, 30, 31
* Notes:
******************************************************************************/
uint8_t calc_month_length(uint8_t month, uint16_t year) {
	switch(month) {
		case 1: return 31;
		case 2: {
			if((year % 400) == 0 || ((year % 4) == 0 && (year % 100) != 0)) return 29;
			else return 28;
		}
		case 3: return 31;
		case 4: return 30;
		case 5: return 31;
		case 6: return 30;
		case 7: return 31;
		case 8: return 31;
		case 9: return 30;
		case 10: return 31;
		case 11: return 30;
		case 12: return 31;
	}
}

/******************************************************************************
* Function:         int main(void)
* Description:      main f?ggv?ny
* Input:
* Output:
* Notes:
******************************************************************************/
int main(void)
{
	port_init();
	timer_init();
	external_int_init();
	uart0_init(BAUD9600);
	lcd_init();
	adc_init();
	
	// TWI initialization
	twi_init();
	
	// CAN initialization
	can_init();
	
	sei();
	
	// String for time and date
	char string_for_write[50];
	
    /* Replace with your application code */
    while (1) 
    {
		if(task_10ms)
		{
			// Set time and save time -> BUTTON1 logic
			if((PINA & (1<<PA0)) == 0 && PA0_pushed == FALSE) {
				if(curr_edit == NOEDIT || curr_edit == TIME) {
					PORTB ^= (1<<PB0);
					curr_edit = TIME;
					
					edited_value = 0;
					sprintf(edited_text, "PERC");
					
					time_0 = timer_cnt;
					
					if(time_edit_save == SAVE) {
						time_edit_save = EDIT;
					} else if(time_edit_save == EDIT) {
						time_edit_save = SAVE;
						twi_mt_mode(curr_edit, masodperc, perc, ora, nap, honap, ev);
					}
					
				}
				
				PA0_pushed = TRUE;
			}
			if((PINA & (1<<PA0)) == (1<<PA0) && PA0_pushed == TRUE) PA0_pushed = FALSE;
			
			// Set date and save date -> BUTTON5 logic
			if((PINA & (1<<PA4)) == 0 && PA4_pushed == FALSE) {
				if(curr_edit == NOEDIT || curr_edit == DATE) {
					PORTB ^= (1<<PB0);
					curr_edit = DATE;
					
					edited_value = 0;
					sprintf(edited_text, "NAP");
					
					time_0 = timer_cnt;
					
					if(date_edit_save == SAVE) {
						date_edit_save = EDIT;
					} else if(date_edit_save == EDIT) {
						date_edit_save = SAVE;
						twi_mt_mode(curr_edit, masodperc, perc, ora, nap, honap, ev);
					}
				}
				
				PA4_pushed = TRUE;
			}
			if((PINA & (1<<PA4)) == (1<<PA4) && PA4_pushed == TRUE) PA4_pushed = FALSE; 
			
			// change between values that will be edited -> BUTTON4 logic
			if((PINA & (1<<PA3)) == 0 && PA3_pushed == FALSE) {
				if(curr_edit == TIME) {
					if(edited_value != 1) edited_value++;
					else edited_value = 0;
					
					switch(edited_value) {
						case 0: {
							sprintf(edited_text, "PERC");
							break;
						}
						case 1: {
							sprintf(edited_text, "ORA");
							break;
						}
					}
					
				} else if(curr_edit == DATE) {
					if(edited_value != 2) edited_value++;
					else edited_value = 0;
					
					switch(edited_value) {
						case 0: {
							sprintf(edited_text, "NAP");
							break;
						}
						case 1: {
							sprintf(edited_text, "HONAP");
							break;
						}
						case 2: {
							sprintf(edited_text, "EV");
							break;
						}
					}
				}
				
				PA3_pushed = TRUE;
			}
			if((PINA & (1<<PA3)) == (1<<PA3) && PA3_pushed == TRUE) PA3_pushed = FALSE;
			
			// increase the actual value of the edited value -> BUTTON2 logic
			if((PINA & (1<<PA1)) == 0 && PA1_pushed == FALSE) {
				if(curr_edit == TIME) {
					switch(edited_value) {
						case 0: {
							if(perc != 59) perc++;
							else perc = 0;
							break;
						}
						case 1: {
							if(ora != 23) ora++;
							else ora = 0;
							break;
						}
					}
				} else if(curr_edit == DATE) {
					switch(edited_value) {
						case 0: {
							if(nap != calc_month_length(honap, ev)) nap++;
							else nap = 1;
							break;
						}
						case 1: {
							if(honap != 12) honap++;
							else honap = 1;
							break;
						}
						case 2: {
							if(ev != 2099) ev++;
							else ev == 2000;
							break;
						}
					}
				}
				
				PA1_pushed = TRUE;
			}
			if((PINA & (1<<PA1)) == (1<<PA1) && PA1_pushed == TRUE) PA1_pushed = FALSE;
			
			// decrease the actual value of the edited value -> BUTTON3 logic
			if((PINA & (1<<PA2)) == 0 && PA2_pushed == FALSE) {
				if(curr_edit == TIME) {
					switch(edited_value) {
						case 0: {
							if(perc != 0) perc--;
							else perc = 59;
							break;
						}
						case 1: {
							if(ora != 0) ora--;
							else ora = 23;
							break;
						}
					}
					} else if(curr_edit == DATE) {
					switch(edited_value) {
						case 0: {
							if(nap != 1) nap--;
							else nap = calc_month_length(honap, ev);
							break;
						}
						case 1: {
							if(honap != 1) honap--;
							else honap = 12;
							break;
						}
						case 2: {
							if(ev != 2000) ev--;
							else ev = 2099;
							break;
						}
					}
				}
				
				PA2_pushed = TRUE;
			}
			if((PINA & (1<<PA2)) == (1<<PA2) && PA2_pushed == TRUE) PA2_pushed = FALSE;
			
			
			task_10ms=FALSE;
		}
		if(task_100ms)
		{
			if(curr_edit == TIME) {
				if(timer_cnt - time_0 < 100) {
					if(time_edit_save == EDIT) sprintf(string_for_write, "IDO SZERKESZTESE");
					if(time_edit_save == SAVE) sprintf(string_for_write, "IDO ELMENTVE    ");
				} else {
					if(time_edit_save == EDIT) sprintf(string_for_write, "%02d:%02d %10s", ora, perc, edited_text);
					if(time_edit_save == SAVE) {
						sprintf(string_for_write, "%04d%02d%02d %02d%02d %02d", ev, honap, nap, ora, perc, homerseklet);
						// sprintf(string_for_write, "%04d%02d%02d %02d%02d %02d", ev, honap, nap, ora, perc, masodperc);
						curr_edit = NOEDIT;
					}
				}
			}
			
			if(curr_edit == DATE) {
				if(timer_cnt - time_0 < 100) {
					if(date_edit_save == EDIT) sprintf(string_for_write, "DATUM SZERK.    ");
					if(date_edit_save == SAVE) sprintf(string_for_write, "DATUM ELMENTVE  ");
				} else {
					if(date_edit_save == EDIT) sprintf(string_for_write, "%04d-%02d-%02d %5s", ev, honap, nap, edited_text);
					if(date_edit_save == SAVE) {
						sprintf(string_for_write, "%04d%02d%02d %02d%02d %02d", ev, honap, nap, ora, perc, homerseklet);
						// sprintf(string_for_write, "%04d%02d%02d %02d%02d %02d", ev, honap, nap, ora, perc, masodperc);
						curr_edit = NOEDIT;
					}
				}
			}
			
			if(curr_edit == NOEDIT) {
				sprintf(string_for_write, "%04d%02d%02d %02d%02d %02d", ev, honap, nap, ora, perc, homerseklet);
				// sprintf(string_for_write, "%04d%02d%02d %02d%02d %02d", ev, honap, nap, ora, perc, masodperc);
			}
			
			lcd_set_cursor_position(0);
			lcd_write_string(string_for_write);
			
			
			task_100ms=FALSE;
		}
		if(task_500ms)
		{
			if(curr_edit == NOEDIT) {
				twi_mr_mode(&masodperc, &perc, &ora, &nap, &honap, &ev);	
			}
			task_500ms=FALSE;
		}
		
		if(task_1s) {
			// send data via CAN
			// DATE, TIME
			uint8_t cnt_4bit = 3;
			
			uint8_t ev_rovid = ev % 100;
			uint8_t honap_0_bit = honap & 1;
			uint8_t honap_123_bit = honap & (0b00001110);
			uint8_t hetnapja = 5;
			uint8_t nap_01_bit = nap & (0b00000011);
			uint8_t nap_234_bit = nap & (0b00011100);
			uint8_t masodperc_01_bit = masodperc & (0b00000011);
			uint8_t masodperc_2345_bit = masodperc & (0b00111100);
			
			uint8_t can_tx_data[5];
			can_tx_data[0] = ev_rovid | (honap_0_bit<<7);
			can_tx_data[1] = (honap_123_bit>>1) | (hetnapja<<3) | (nap_01_bit<<6);
			can_tx_data[2] = (nap_234_bit>>2) | (ora<<3);
			can_tx_data[3] = perc | (masodperc_01_bit<<6);
			can_tx_data[4] = (masodperc_2345_bit>>2) |(cnt_4bit<<4);
			CAN_SendMob(0,0x1FE,FALSE,5,can_tx_data);
			
			// TEMPERATURE
			homerseklet = (read_temperature()>>1);
			
			int16_t homerseklet_can = (homerseklet-100) * 10;
			uint8_t homerseklet_can0_8 = (uint8_t)homerseklet_can;
			uint8_t homerseklet_can9_12 = (uint8_t)(homerseklet_can>>8);

			uint8_t can_tx_data_1[2];
			can_tx_data_1[0] = 0x00;
			can_tx_data_1[1] = 99;
			can_tx_data_1[0] = homerseklet_can0_8;
			can_tx_data_1[1] = homerseklet_can9_12;
			CAN_SendMob(1,0x1FF,FALSE,2,can_tx_data_1);
			
			task_1s = FALSE;
		}
    }
}

/******************************************************************************
* Interrupt Routines
******************************************************************************/
ISR(TIMER0_COMP_vect) //timer CTC interrupt
{
	timer_cnt++;
	if(timer_cnt % 1 == 0) task_10ms = TRUE;
	if(timer_cnt % 10 == 0) task_100ms = TRUE;
	if(timer_cnt % 50 == 0) task_500ms =TRUE;
	if(timer_cnt % 100 == 0) task_1s = TRUE;
}

ISR(INT0_vect) //extint 0 interrput
{

}

ISR(USART0_RX_vect)
{
	// char c = UDR0;
	// lcd_write_char(c);
}

ISR(ADC_vect)
{
	ad_result = ADC;
}