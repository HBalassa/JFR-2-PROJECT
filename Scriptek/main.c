/******************************************************************************
 * Created:		26-10-2024
 * Author :		Balassa Hodi
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include <avr/io.h>

#define F_CPU 8000000UL
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "lcd.h"
#include "uart.h"
#include "peripherals.h"

/******************************************************************************
* Macros
******************************************************************************/
#define FALSE 0
#define TRUE 1

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint16_t timer_cnt=0;
uint8_t task_10ms = FALSE, task_100ms = FALSE, task_500ms = FALSE;
uint16_t ad_result;

uint16_t helyiertek = 0;

//szovegtomb
char string_for_write[50];
char ido_string[] = "HH/MM";
char datum_string[] = "YY/MM/DD";

//időzítős segedvaltozok
uint16_t time_0 = 0;
uint16_t time_1 = 0;
uint16_t time_2 = 0;
uint16_t time_3 = 0;
uint16_t display_time = 0;

//menu vizsgalo logikai valtozok
uint8_t date_edit = FALSE;
uint8_t time_edit = FALSE;

//gomblenyomas vizsgalo valtozok
uint8_t PA0_pushed = FALSE;
uint8_t PA0_counter = 0;

uint8_t PA1_pushed = FALSE;
uint8_t PA1_counter = 0;

uint8_t PA2_pushed = FALSE;
uint8_t PA2_counter = 0;

uint8_t PA3_pushed = FALSE;
uint8_t PA3_counter = 0;

uint8_t PA4_pushed = FALSE;
uint8_t PA4_counter = 0;
/******************************************************************************
* External Variables
******************************************************************************/

/******************************************************************************
* Local Function Declarations
******************************************************************************/
void port_init(void);

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
	PORTA = (1<<PA0) | (1<<PA1) | (1<<PA2) | (1<<PA3) | (1<<PA4);;
	
	DDRF = (1<<PF1) | (1<<PF2);
	PORTF = (1<<PF1) | (1<<PF2);
	
	DDRD = (0<<PD0);
	PORTD = (1<<PD0);
	
	DDRC = (1<<LCD_E) | (1<<LCD_RS) | (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5) | (1<<LCD_D4);
	PORTC = (0<<LCD_E) | (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
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
	//uart0_init(BAUD9600);
	lcd_init();
	adc_init();
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		if(task_10ms)
		{
			//BUTTON1 = PA0
			
			if((PINA & (1<<PA0)) == 0 && PA0_pushed == FALSE)
			{
				if (PA4_counter != 1)
				{
				PA0_counter++; //számolom, hogy hányszor nyomom be a gombot, ezzel fogom majd ellenőrizni, azt is, hogy éppen IDŐ szerkesztésben vagyunk e

				}
				if (PA0_counter == 1)
				{
					time_0 = timer_cnt;					
				}
				
				if (PA0_counter == 2)
				{
					//küldje ki az óra IC-jére xdd
					time_1 = timer_cnt;
				}
				
				PA0_pushed = TRUE;
			}
			if((PINA & (1<<PA0)) == (1<<PA0) && PA0_pushed == TRUE) PA0_pushed = FALSE;
			
			//BUTTON2 = PA1
			
// 			if((PINA & (1<<PA1)) == 0 && PA1_pushed == FALSE)
// 			{
// 				//ha az idő szerkesztésben vagyunk
// 				if (PA0_counter == 1);
// 				{
// 					PA1_counter++;
// 					if (ido_string[helyiertek] == "H" | ido_string[helyiertek] == "M")
// 					{
// 						ido_string[helyiertek] = 0;
// 					}
// 					else
// 					{
// 						ido_string[helyiertek]= 0 + PA1_counter;
// 					
// 					}
// 						
// 					
// 				}
// 				
// 				PA1_pushed = TRUE;
// 			}
// 			if((PINA & (1<<PA1)) == (1<<PA1) && PA1_pushed == TRUE) PA1_pushed = FALSE;


		//BUTTON4 PA3
			
		if((PINA & (1<<PA3)) == 0 && PA3_pushed == FALSE)
		{
			//had idő szerkesztében vagyunk:
			if (PA0_pushed == 1)
			{
				switch (helyiertek)
				{
					case 0: helyiertek = 1;
					break;
					case 1: helyiertek = 3;
					break;
					case 3: helyiertek = 4;
					break;
					case 4: helyiertek = 0;
					break;	
				}
			}
			
			//ha dátum szerkesztésben vagyunk
			if(PA4_counter == 1)
			{
				if (helyiertek != 1 | helyiertek != 4 | helyiertek != 7 | )
				{
					helyiertek++;
				}
				else if (helyiertek == 1)
				{
					helyiertek = 3;
				}
				else if (helyiertek == 4)
				{
					helyiertek = 6;
				}
				else if (helyiertek == 7)
				{
					helyiertek = 0;
				}

			}
			
			
			PA3_pushed = TRUE;
		}
		if((PINA & (1<<PA3)) == (1<<PA3) && PA3_pushed == TRUE) PA3_pushed = FALSE;
			
			
			//BUTTON5 PA4
			if((PINA & (1<<PA4)) == 0 && PA4_pushed == FALSE)
			{
				if (PA0_counter != 1)
				{
					PA4_counter++;
				}
				if (PA4_counter == 1)
				{
					time_2 = timer_cnt;
				}
				
				if (PA4_counter == 2)
				{
					time_3 = timer_cnt;
				}
				
				PA4_pushed = TRUE;
			}
			if((PINA & (1<<PA4)) == (1<<PA4) && PA4_pushed == TRUE) PA4_pushed = FALSE;
			
		
			
			task_10ms=FALSE;
		}
		
		if(task_100ms)
		{
			//LCD kiirás
			
			//BUTTON1:
			if (PA0_counter == 1 && PA4_counter != 1) //elso benyomas utan ÉS HA NEM VAGYUNK ÉPPEN DÁTUM SZERKESZTOBEN
			{
			
				
				if (timer_cnt-time_0 < 100) //amint letelik az egy másodperc jelenjen meg a szerkesztő menü
				{
					sprintf(string_for_write, "IDO SZERKESZTESE");
					time_edit = TRUE;
					lcd_set_cursor_position(0);
					lcd_write_string(string_for_write);
				}
				else
				{
					if (time_edit == TRUE)
					{
						lcd_clear_display();
						time_edit = FALSE;
					}
					lcd_set_cursor_position(0);
					lcd_write_string(ido_string);
					
					
				}

			}
			
			
			//BUTTON 1 második kiírás
			if (PA0_counter == 2 && PA4_counter !=1)
			{
				
				if (timer_cnt-time_1 < 100)
				{
					lcd_set_cursor_position(0);
					sprintf(string_for_write, "IDO MENTVE");
					lcd_write_string(string_for_write);
				}
				else
				{
					lcd_clear_display();
					//ide raknek majd egy logikai változó igazzá tételt ami azt ellenőrzi, hogy éppen az 'alaphelyzetben vagyunk'
					PA0_counter = 0;
				}
				
				
			}
			
			
			//BUTTON5
			if (PA4_counter == 1 && PA0_counter != 1) //elso benyomas utan ÉS HA NEM VAGYUNK ÉPPEN IDŐ SZERKESZTOBEN
			{
						
				if (timer_cnt-time_2 < 100) //amint letelik az egy másodperc jelenjen meg a szerkesztő menü
				{
					sprintf(string_for_write, "DATUM SZERKESZTESE");
					date_edit = TRUE;
					lcd_set_cursor_position(0);
					lcd_write_string(string_for_write);
				}
				else
				{
					if (date_edit == TRUE)
					{
						lcd_clear_display();
						date_edit = FALSE;
					}
					lcd_set_cursor_position(0);
					lcd_write_string(datum_string);
				}
					
				
			}
							
				//BUTTON 5 második kiírás
				if (PA4_counter == 2 && PA0_counter !=1)
				{
				
					if (timer_cnt-time_3 < 100)
					{
						lcd_set_cursor_position(0);
						sprintf(string_for_write, "DATUM MENTVE");
						lcd_write_string(string_for_write);
					}
					else
					{
						lcd_clear_display();
						//ide raknek majd egy logikai változó igazzá tételt ami azt ellenőrzi, hogy éppen az 'alaphelyzetben vagyunk'
						PA4_counter = 0;
					}
	
				}
					
			task_100ms=FALSE;
		}
		if(task_500ms)
		{
			task_500ms=FALSE;
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
}

ISR(INT0_vect) //extint 0 interrput
{

}

ISR(USART0_RX_vect)
{
	//char c = UDR0;
	//lcd_write_char(c);
}

ISR(ADC_vect)
{
	ad_result = ADC;
}