/******************************************************************************
 * Created:		12-09-2024 12h
 * Author :		Balassa Hodi
 * File Name:	gyak02_09.12
 * File source:	Feher Arpad github file
******************************************************************************/


 /******************************************************************************
* Include files
******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL 
#include <avr/delay.h>

#include <inttypes.h>


/******************************************************************************
* Macros
******************************************************************************/
#define TRUE 1
#define FALSE 0

// C port bitjei az LCD labakon
// LCD ports 
#define LCD_D7 7
#define LCD_D6 6
#define LCD_D5 5
#define LCD_D4 4
// LCD enable
#define LCD_E 3
// LCD RS
#define LCD_RS 2


/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint8_t task_10ms = FALSE, task_100ms = FALSE, task_500ms = FALSE;
uint16_t timer_cnt=0;


/******************************************************************************
* External Variables
******************************************************************************/


/******************************************************************************
* Local Function Declarations
******************************************************************************/
void port_init(void);
void timer_init(void);
void external_int_init(void);
void lcd_enable_pulse(void);
void lcd_init(void);
void lcd_write_char(char c);

/******************************************************************************
* Local Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         int main(void)
* Description:      main fuggveny
* Input:
* Output:
* Notes:
******************************************************************************/
int main(void)
{
	port_init();
	timer_init();
	external_int_init();
	sei();
	lcd_init();


	/* Replace with your application code */
	while (1)
	{
		if(task_10ms == TRUE)
		{
			PORTF ^= 0x01;
			task_10ms = FALSE;
		}
		
		if(task_100ms == TRUE)
		{
			PORTF ^= 0X02;
			task_100ms = FALSE;
		}
		
		if(task_500ms == TRUE)
		{
			
			PORTF ^= 0x04;
			task_500ms = FALSE;
		}
	}
}

/******************************************************************************
* Function:         void port_init(void)
* Description:      I/O portok inicializalasa
* Input:
* Output:
* Notes:
******************************************************************************/
void port_init(void)
{
	DDRA = 0xff;
	PORTA = 0xff;
	
	DDRF = 0x0f;
	PORTF = 0x0f;
	
	DDRD = (0<<PD0);
	PORTD = (1<<PD0);
	
	// LCD beallitasa
	DDRC = (1<<LCD_E) | (1<<LCD_RS) | (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5) | (1<<LCD_D4);
	PORTC = (0<<LCD_E) | (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4); 
}

/******************************************************************************
* Function:         void timer_init(void)
* Description:      timer inicializalasa
* Input:
* Output:
* Notes:			10 ms, CTC
******************************************************************************/
void timer_init(void)
{
	TCCR0A = (0<<WGM00) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	OCR0A = 77;
	TIMSK0 = (1<<OCIE0A);
}

/******************************************************************************
* Function:         void external_int_init(void)
* Description:      kulso megszakitas beallitasa
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
* Function:         void lcd_enable_pulse(void)
* Description:      
* Input:
* Output:
* Notes:			azert kell ez, mert egy idore magas szintre kell rakni az E labat, majd alacsonyra, es igy fog megjelenni az adat az lcd-n
******************************************************************************/
void lcd_enable_pulse(void) {
	PORTC = PORTC | (1<<LCD_E); // E lab magas jelszinten
	
	_delay_us(1); // 1 us-t varunk, mert a datsheet szerint ennyi eleg (sot sok is, mert 460 ns van ott)
	
	PORTC = PORTC & ~(1<<LCD_E); // ~ -> bitenkenti negalas -> a sor eredmenye az lesz, hogy minden marad ugy ahogy volt, kiveve az E port 0 biten lesz
}

/******************************************************************************
* Function:         void lcd_init(void)
* Description:      lcd inicializalasa
* Input:
* Output:
* Notes:			ST7066 datasheet 25 oldala alapjan
******************************************************************************/
void lcd_init(void) {
	_delay_ms(50);
	
	// function set 1
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (1<<LCD_D5) | (1<<LCD_D4);
	lcd_enable_pulse(); // kikuldjuk az adatokat az lcd-re
	_delay_us(40);
	
	// function set 2
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (1<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	PORTC = (0<<LCD_RS) | (1<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	_delay_us(40);
	
	// function set 3
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (1<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	PORTC = (0<<LCD_RS) | (1<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	_delay_us(40);
	
	// display on/off control
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	PORTC = (0<<LCD_RS) | (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5) | (1<<LCD_D4);
	lcd_enable_pulse();
	_delay_us(40);
	
	// display clear
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (1<<LCD_D4);
	lcd_enable_pulse();
	_delay_ms(2);
	
	// entry mode set
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	PORTC = (0<<LCD_RS) | (0<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5) | (0<<LCD_D4);
	lcd_enable_pulse();
	_delay_ms(10);
	
}

/******************************************************************************
* Function:         
* Description:      
* Input:
* Output:
* Notes:		karakter kuldese az lcd kijelzore (eloszor a felso negy, majd also negy bitet)
******************************************************************************/
void lcd_write_char(char c) {
	_delay_us(40); // varni kell mielott kikuldjuk a karaktert az lcd-re
	
	PORTC = (c & 0xf0) | (1<<LCD_RS); // a karekter felso negy bitjet adja hozza; // RS-t magas szintre kell hozni
	lcd_enable_pulse();
	PORTC = ((c & 0x0f)<<4) | (1<<LCD_RS); // karakter also negy bitjet adja hozza, majd balra tolja 4 bittel
	lcd_enable_pulse();
}

/******************************************************************************
* Interrupt Routines
******************************************************************************/
ISR(TIMER0_COMP_vect) //timer CTC interrupt
{
	timer_cnt++;
	if((timer_cnt % 1) == 0) task_10ms = TRUE;
	if((timer_cnt % 10) == 0) task_100ms =TRUE;
	if((timer_cnt % 50) == 0) task_500ms = TRUE;
}

ISR(INT0_vect) //ext 0 interrupt
{
	lcd_write_char('a');
	PORTA = PORTA ^ 0x01;
}