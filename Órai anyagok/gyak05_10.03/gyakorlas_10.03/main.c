/******************************************************************************
 * Created:		03-10-2024 12h
 * Author :		Balassa Hodi
 * File name:	gyakorlas_10.03
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

#include <math.h>

/******************************************************************************
* Macros
******************************************************************************/
#define TRUE 1
#define FALSE 0

// 8. feladat
#define OSSZEADAS 1
#define KIVONAS 2
#define SZORZAS 3
#define OSZTAS 4
#define SZAM_1 1
#define SZAM_2 2


/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint8_t task_10ms =FALSE, task_100ms=FALSE, task_500ms=FALSE;
uint16_t timer_cnt=0;
uint16_t ad_result;

// 8. feladat
uint8_t szam_1;
uint8_t szam_2;
uint8_t eredmeny;
uint8_t muveleti_jel = OSSZEADAS;
uint8_t szam_valaszto = SZAM_1;

// PD gombok
uint8_t PD1_pushed = FALSE;
uint8_t PD2_pushed = FALSE;
uint8_t PD3_pushed = FALSE;
uint8_t PD4_pushed = FALSE;

/******************************************************************************
* External Variables
******************************************************************************/


/******************************************************************************
* Local Function Declarations
******************************************************************************/
void port_init(void);

// feladatok fuggvenyei
void print_voltage(void);
void print_decimal(void);
void print_binary(void);
void AD_calculator();

/******************************************************************************
* Local Function Definitions
******************************************************************************/
/******************************************************************************
* Function:         int main(void)
* Description:      main függvény
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
    /* Replace with your application code */
    while (1)
    {
		if(task_10ms == TRUE)
		{
			//PORTF ^= 0x01;
			task_10ms = FALSE;
			
			// PD gombok lenyomasa
			//OSSZEADAS
			if((PIND & (1<<PD1)) == 0 && PD1_pushed == FALSE) {
				PORTA ^= (1<<PA1);
				
				muveleti_jel = OSSZEADAS;
				PD1_pushed = TRUE;
			}
			if((PIND & (1<<PD1)) == (1<<PD1) && PD1_pushed == TRUE) PD1_pushed = FALSE;
			
			// KIVONAS
			if((PIND & (1<<PD2)) == 0 && PD2_pushed == FALSE) {
				PORTA ^= (1<<PA2);
				
				muveleti_jel = KIVONAS;
				PD2_pushed = TRUE;
			}
			if((PIND & (1<<PD2)) == (1<<PD2) && PD2_pushed == TRUE) PD2_pushed = FALSE;
			
			// SZORZAS
			if((PIND & (1<<PD3)) == 0 && PD3_pushed == FALSE) {
				PORTA ^= (1<<PA3);
				
				muveleti_jel = SZORZAS;
				PD3_pushed = TRUE;
			}
			if((PIND & (1<<PD3)) == (1<<PD3) && PD3_pushed == TRUE) PD3_pushed = FALSE;
			
			// OSZTAS
			if((PIND & (1<<PD4)) == 0 && PD4_pushed == FALSE) {
				PORTA ^= (1<<PA4);
				
				muveleti_jel = SZORZAS;
				PD4_pushed = TRUE;
			}
			if((PIND & (1<<PD4)) == (1<<PD4) && PD4_pushed == TRUE) PD4_pushed = FALSE;
		}
		
		if(task_100ms == TRUE)
		{
			//ADCSRA |= (1<<ADSC); 
			//PORTA =ad_result;
			//PORTF ^= 0X02;
			//char write_string[50];
			//sprintf(write_string,"%4d",ad_result);
			//lcd_set_cursor_position(0);
			//lcd_write_string(write_string);
			//uart_write_string(write_string);
			//uart_write_string("\r\n");
			
			// 1. feladat
			// print_voltage();
			
			// 2. feladat
			// print_decimal();
			
			// 3. feladat
			// print_binary();
			
			// 8. feladat
			AD_calculator();
			
			task_100ms = FALSE;
		}
		
		if(task_500ms == TRUE)
		{
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
* Function:         void port_init(void)
* Description:      I/O portok inicializálása
* Input:
* Output:
* Notes:
******************************************************************************/
void port_init(void)
{
	DDRA = 0xff;
	PORTA = 0xff;
	
	DDRF = (1<<PF3) | (1<<PF2) | (1<<PF1) | (0<<PF0);
	//PORTF = 0x0f;
	
	// kulso megszakitas
	DDRD = (0<<PD0) | (0<<PD1) | (0<<PD2) | (0<<PD3) | (0<<PD4);
	PORTD = (1<<PD0) | (1<<PD1) | (1<<PD2) | (1<<PD3) | (1<<PD4);
	
	DDRC = (1<<LCD_E) | (1<<LCD_RS) | (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5) | (1<<LCD_D4);
	PORTC = (0<<LCD_E) | (0<<LCD_RS) | (0<<LCD_D7) | (0<<LCD_D6) | (0<<LCD_D5) | (0<<LCD_D4);
}

/******************************************************************************
* Function:     void print_voltage(void)
* Description:      
* Input:
* Output:
* Notes:		Irjon egy fuggvenyt, ami kiirja az LCD kijelzore az AD atalakitas
				bemeneti analog feszultsegerteket voltban, 3 tizedesjegy pontossaggal.
******************************************************************************/
void print_voltage(void) {
	// feszultseg ertek 0V...5V
	
	// feszultseg kiszamolasa (mV)
	int voltage = ((uint32_t)ad_result * 5 * 1000)/1023;
	
	char write_string[50];
	
	sprintf(write_string, "%d.%03d V", voltage/1000, voltage%1000);
	
	lcd_set_cursor_position(0);
	lcd_write_string(write_string);
	
	// atalakitas kezdese
	ADCSRA |= (1<<ADSC);
}

/******************************************************************************
* Function:     void print_decimal();
* Description:      
* Input:
* Output:
* Notes:		Írjon egy függvényt, ami az LCD kijelzõn 6 tizedesjegy pontos törtszámot jelenít meg.
				Írja ki a PI értékét.
******************************************************************************/
void print_decimal(void) {
	char write_string[50];
	
	uint8_t int_part = (uint8_t)M_PI;
	uint32_t float_part = (M_PI - int_part) * 1000000;
	
	sprintf(write_string, "%d.%ld", int_part, float_part);
	
	lcd_set_cursor_position(0);
	lcd_write_string(write_string);
}


/******************************************************************************
* Function:     void print_binary();
* Description:      
* Input:
* Output:
* Notes:		Írjon egy függvényt, ami egy 8 bites változót az LCD kijelzõn binárisan megjelenít.
******************************************************************************/
void print_binary() {
	lcd_set_cursor_position(0);
	
	uint8_t valtozo = 0b01011101;
	
	for(uint8_t i=0; i<8; i++) {
		if(valtozo & (1<<(7-i))) lcd_write_char('1');
		else lcd_write_char('0');
	}
}

/******************************************************************************
* Function:     void AD_calculator();
* Description:
* Input:
* Output:
* Notes:		8. feladat:
				Készítsen egy számológépet úgy, hogy a számokat a boardon található potméterrel
				kell megadni (AD átalakítás). Azt, hogy a potméterrel melyik számot adja meg és a
				mûveleti jelet gombnyomással kell kiválasztani.
******************************************************************************/
void AD_calculator() {
	// 100ms-os taskba megy majd
	
	char write_string[50];
	switch(muveleti_jel) {
		case OSSZEADAS:
			sprintf(write_string, "%3d+%3d=%3d", szam_1, szam_2, szam_1+szam_2);
			break;
		case KIVONAS:
			sprintf(write_string, "%3d-%3d=%3d", szam_1, szam_2, szam_1-szam_2);
			break;
		case SZORZAS:
			sprintf(write_string, "%3d*%3d=%6d", szam_1, szam_2, szam_1*szam_2);
			break;
		case OSZTAS:
			break;
	}
	
	lcd_set_cursor_position(0);
	lcd_write_string(write_string);
	
	switch(szam_valaszto) {
		case SZAM_1:
			szam_1 = ad_result/4;
			break;
		case SZAM_2:
			szam_2 = ad_result/4;
			break;
	}
	
	ADCSRA |= (1<<ADSC);
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
	szam_valaszto++;
	if(szam_valaszto>SZAM_2) szam_valaszto = SZAM_1;
	
	PORTA = PORTA ^ 0x01;
}

ISR(USART0_RX_vect)
{
	char c = UDR0;
	if(c == 0x7F)
		lcd_clear_display();
	else
		lcd_write_char(c);
}

ISR(ADC_vect)
{
	ad_result = ADC;
}