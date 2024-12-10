#ifndef LCD_H_
#define LCD_H_


/******************************************************************************
* Include files
******************************************************************************/
#include <inttypes.h>

/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Macros
******************************************************************************/
//LCD
#define LCD_D7 7
#define LCD_D6 6
#define LCD_D5 5
#define LCD_D4 4

#define LCD_E 3
#define LCD_RS 2

/******************************************************************************
* Global Function Declarations
******************************************************************************/
void lcd_init(void);
void lcd_write_char(char c);
void lcd_set_cursor_position(uint8_t pos);
void lcd_clear_display(void);
void lcd_write_string(char *c);
#endif /* LCD_H_ */