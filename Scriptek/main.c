/******************************************************************************
 * Created:		03-10-2024 12h
 * Author :		Balassa Hodi
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL 
#include <util/delay.h>

#include <inttypes.h>

#include "lcd.h"
#include "uart.h"
#include "peripherals.h"

#include "excercises.h"

#include<math.h>


/******************************************************************************
* Macros
******************************************************************************/
#define TRUE 1
#define FALSE 0

// Ex. 7
#define SZAM1 0
#define SZAM2 1

#define OSSZEADAS 1
#define KIVONAS 2
#define SZORZAS 3
#define OSZTAS 4

// Ex. 13
#define INCREASE 0
#define DECREASE 1


/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint8_t task_1ms = FALSE, task_10ms =FALSE, task_100ms=FALSE, task_500ms=FALSE;
uint16_t timer_cnt=0;
uint16_t ad_result;

// Ex. 6
uint16_t time_1 = 0;
uint16_t time_2 = 0;
uint8_t PD1_pushed = FALSE;

// Ex. 7
uint8_t PD2_pushed = FALSE;
uint8_t PD3_pushed = FALSE;
uint8_t PD4_pushed = FALSE;
uint8_t PD5_pushed = FALSE;
uint8_t PD6_pushed = FALSE;
uint8_t PD7_pushed = FALSE;

uint8_t szam_1 = 0;
uint8_t szam_2 = 0;

uint8_t szam_valaszto = SZAM1;

uint8_t muveleti_jel = OSSZEADAS;

// Ex. 12
uint16_t led_blink_period_time = 0;

// Ex. 13
uint8_t running_led_direction = INCREASE;

// Ex. 14
uint8_t helyiertek = 0;
uint8_t number_on_lcd = 0;

// Ex. 16
uint16_t random_number;

// Ex. 17
char character = '0';
uint8_t sorszam = 0;

// Ex. 19
uint16_t period_time = 1000;

// Ex. 20
uint8_t button_press_cnt = 0;


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
* Description:      I/O portok inicializ�l�sa
* Input:
* Output:
* Notes:
******************************************************************************/
void port_init(void)
{
	DDRA = 0xff;
	PORTA = 0xff;
	// Ex. 16
	PORTA = 0;


	DDRB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5) | (1<<PB6) | (1<<PB7);
	PORTB = (0<<PB0) | (0<<PB1) | (0<<PB2) | (0<<PB3) | (0<<PB4) | (0<<PB5) | (0<<PB6) | (0<<PB7);
	
	DDRF = (1<<PF3) | (1<<PF2) | (1<<PF1) | (0<<PF0);
	//PORTF = 0x0f;
	
	DDRD = (0<<PD0) | (0<<PD1) | (0<<PD2) | (0<<PD3) | (0<<PD4) | (0<<PD5) | (0<<PD6) | (0<<PD7);
	PORTD = (1<<PD0) | (1<<PD1) | (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
	
	DDRC = (1<<LCD_E) | (1<<LCD_RS) | (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5) | (1<<LCD_D4);
	PORTC = (0<<LCD_E) | (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
}



/******************************************************************************
* Function:         int main(void)
* Description:      main f�ggv�ny
* Input:
* Output:
* Notes:
******************************************************************************/
int main(void)
{
	port_init();
	timer_init();
	external_int_init();
	lcd_init();
	uart0_init(BAUD9600);
	adc_init();
	sei();

	// Ex. 14
	// lcd_write_string("00000000=000");

	// Ex. 16
	// random_number = random_number_generator();
	// time_1 = timer_cnt;

	// Ex. 19
	// lcd_set_cursor_position(0);
	// char write_string[50];
	// sprintf(write_string, "%4d", period_time);
	// lcd_write_string(write_string);

	/* Replace with your application code */
	while (1) {

		if(task_1ms == TRUE) {
			// Ex. 12
			// if(timer_cnt - time_1 == led_blink_period_time) {
			// 	PORTB = PORTB ^ (1<<PB0);
			// 	time_1 = timer_cnt;
			// }

			// Ex. 13
			// if(timer_cnt - time_1 == led_blink_period_time) {
			// 	if(running_led_direction == INCREASE) {
			// 		PORTB *= 2;
			// 		if(PORTB == (1<<PB7)) running_led_direction = DECREASE;
			// 	} else if(running_led_direction == DECREASE) {
			// 		PORTB /= 2;
			// 		if(PORTB == (1<<PB0)) running_led_direction = INCREASE;
			// 	}

			// 	time_1 = timer_cnt;
			// }

			// Ex. 16
			// if(timer_cnt - time_1 == random_number) {
			// 	PORTA = 0xff;
			// 	time_1 = timer_cnt;
			// }

			

			task_1ms = FALSE;
		}

		if(task_10ms == TRUE) {
			//PORTF ^= 0x01;

			// Ex. 6
			// if((PIND & (1<<PD1)) == 0 && PD1_pushed == FALSE) {
			// 	print_time_diff(time_1, time_2);

			// 	PD1_pushed = TRUE;
			// }
			// if((PIND & (1<<PD1)) == (1<<PD1) && PD1_pushed == TRUE) PD1_pushed == FALSE;

			// Ex. 7
			// button_calculator();

			// Ex. 9
			// if((PIND & (1<<PD4)) == 0 && PD4_pushed == FALSE) {
			// 	print_decimal_to_binary(169);

			// 	PD4_pushed = TRUE;
			// }
			// if((PIND & (1<<PD4)) == (1<<PD4) && PD4_pushed == TRUE) PD4_pushed = FALSE;

			// Ex. 12
			// Irjuk ki a potmeter erteket az LCD kijelzore (0...1023)
			// char write_string[50];
			// sprintf(write_string, "%4d", ad_result);
			// lcd_set_cursor_position(0);
			// lcd_write_string(write_string);
			// ADCSRA |= (1<<ADSC);

			// Ex. 13
			// Irjuk ki a potmeter erteket az LCD kijelzore (0...1023)
			// char write_string[50];
			// sprintf(write_string, "%4d", ad_result);
			// lcd_set_cursor_position(0);
			// lcd_write_string(write_string);
			// ADCSRA |= (1<<ADSC);

			// // Gombkezeles
			// if((PIND & (1<<PD1)) == 0 && PD1_pushed == FALSE) {
			// 	time_1 = timer_cnt;
			// 	led_blink_period_time = ad_result;
			// 	running_led_direction = INCREASE;
			// 	PORTB = (1<<PB0);

			// 	PD1_pushed = TRUE;
			// }
			// if((PIND & (1<<PD1)) == (1<<PD1) && PD1_pushed == TRUE) PD1_pushed == FALSE;

			// Ex. 14
			// if((PIND & (1<<PD1)) == 0 && PD1_pushed == FALSE) {
			// 	lcd_set_cursor_position(0);

			// 	number_on_lcd ^= (1<<helyiertek);
			// 	//number_on_lcd = 1011'1'000 ^ 0000'1'000 -> 1011'0'000

			// 	for(uint8_t i=0; i<8; i++) {
			// 		if(number_on_lcd & (1<<(7-i))) lcd_write_char('1');
			// 		else lcd_write_char('0');
			// 	}

			// 	char write_sting[50];
			// 	sprintf(write_string, "=%d", number_on_lcd);
			// 	lcd_write_string(write_string);

			// 	PD1_pushed = TRUE;
			// }
			// if((PIND & (1<<PD1)) == (1<<PD1) && PD1_pushed == TRUE) PD1_pushed = FALSE;
		
			// Ex. 18
			// stopwatch();

			// Ex. 19
			// if((PIND & (1<<PD1)) == 0 && PD1_pushed == FALSE) {
			// 	period_time += 10;
			// 	lcd_set_cursor_position(0);
			// 	write_string[50];
			// 	sprintf(write_string, "%4d", period_time);
			// 	lcd_write_string(write_string);

			// 	PD1_pushed = TRUE;
			// }
			// if((PIND & (1<<PD1)) == (1<<PD1) && PD1_pushed == TRUE) PD1_pushed = FALSE;

			// if((PIND & (1<<PD2)) == 0 && PD2_pushed == FALSE) {
			// 	period_time -= 10;
			// 	lcd_set_cursor_position(0);
			// 	write_string[50];
			// 	sprintf(write_string, "%4d", period_time);
			// 	lcd_write_string(write_string);

			// 	PD2_pushed = TRUE;
			// }
			// if((PIND & (1<<PD2)) == (1<<PD2) && PD2_pushed == TRUE) PD2_pushed = FALSE;

			// // Ex. 21
			// // lenyomjuk a gombot
			// if((PIND & (1<<PD1)) == 0 && PD1_pushed == FALSE) {
			// 	time_1 = timer_cnt;
			// 	PD1_pushed = TRUE;
			// }
			// // felengedjuk a gombot
			// if((PIND & (1<<PD1)) == (1<<PD1) && PD1_pushed == TRUE) {
			// 	time_2 = timer_cnt;

			// 	lcd_set_cursor_position(0);

			// 	if(time_2 - time_1 > 1000) {
			// 		lcd_write_string(" LONG");
			// 	} else {
			// 		lcd_write_string("SHORT");
			// 	}

			// 	PD1_pushed = FALSE;
			// }


			task_10ms = FALSE;
		}
		
		if(task_100ms == TRUE) {
			// ADCSRA |= (1<<ADSC); 
			// PORTA = ad_result;
			// PORTF ^= 0X02;
			// char write_string[50];
			// sprintf(write_string,"%4d",ad_result);
			// lcd_set_cursor_position(0);
			// lcd_write_string(write_string);
			// uart_write_string(write_string);
			// uart_write_string("\r\n");

			// Ex. 17
			// ad_select_letter_number();
			// ADCSRA |= (1<<ADSC);
			

			task_100ms = FALSE;
		}
		
		if(task_500ms == TRUE) {
			//char szoveg[] = "szoveg a kijelzon";
			//lcd_write_string(szoveg);
			//lcd_set_cursor_position(0);
			//lcd_clear_display();
			//lcd_write_string("masik szoveg");
			
			PORTF ^= 0x04;
			task_500ms = FALSE;
		}
  }
}

/******************************************************************************
* Interrupt Routines
******************************************************************************/
ISR(TIMER0_COMP_vect) { //timer CTC interrupt
	timer_cnt++;

	if((timer_cnt % 1) == 0) task_10ms = TRUE;
	if((timer_cnt % 10) == 0) task_100ms =TRUE;
	if((timer_cnt % 50) == 0) task_500ms = TRUE;

	// if timer -> 1ms
	// task_1ms = TRUE;
	// if((timer_cnt % 10) == 0) task_10ms = TRUE;
	// if((timer_cnt % 100) == 0) task_100ms =TRUE;
	// if((timer_cnt % 500) == 0) task_500ms = TRUE;
}

ISR(INT0_vect) { //ext 0 interrupt
	PORTA = PORTA ^ 0x01;

	// Ex. 6
	// print_time_diff(time_1, time_2);
	// _delay_ms(20);

	// Ex. 7
	// if(szam_valaszto == SZAM1) {
	// 	szam_valaszto = SZAM2;
	// } else if(szam_valaszto == SZAM2) {
	// 	szam_valszto = SZAM1;
	// }

	// Ex. 12
	// PORTB = PORTB | (1<<PB0);
	// time_1 = timer_cnt;
	// led_blink_period_time = ad_result;

	// Ex. 14
	// if(helyiertek != 7) {
	// 	helyiertek++;
	// } else helyiertek = 0;

	// Ex. 16
	// time_2 = timer_cnt;
	// char write_string[50];
	// sprintf(write_string, "%4d", time_2-time_1);
	// lcd_set_cursor_position(0);
	// lcd_write_string(write_string);
	// _delay_ms(20);

	// Ex. 17
	// lcd_write_char(character);
	// sorszam++;
	// _delay_ms(20);

	// Ex. 20
	// button_press_cnt++;
	// char write_string[50];
	// sprintf(write_string, "%3d", button_press_cnt);
	// lcd_set_cursor_position(0);
	// lcd_write_string(write_string);
	// _delay_ms(20);

}

ISR(USART0_RX_vect) {
	char c = UDR0;
	if(c == 0x7F)
		lcd_clear_display();
	else
		lcd_write_char(c);
}

ISR(ADC_vect) {
	ad_result = ADC;
}