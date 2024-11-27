#ifndef TWI_H_
#define TWI_H_

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
#define TRUE 1
#define FALSE 0

// DS1307 TWSR Register status codes
#define START 0x08
#define REP_START 0x10
#define MT_SLA_ACK 0x18
#define MT_SLA_NACK 0x20
#define MT_DATA_ACK 0x28
#define MT_DATA_NACK 0x30
#define ARB_LOST 0x38
#define MR_SLA_ACK 0x40
#define MR_SLA_NACK 0x48
#define MR_DATA_ACK 0x50
#define MR_DATA_NACK 0x58

// DS1307 slave address
#define DS1307_ADR_W 0b11010000
#define DS1307_ADR_R 0b11010001

// edit time or date
#define TIME 0
#define DATE 1

// DS1307 Registers Address
#define SEC_ADR 0x00
#define MIN_ADR 0x01
#define HOU_ADR 0x02
#define DAY_ADR 0x03
#define DAT_ADR 0x04
#define MON_ADR 0x05
#define YEA_ADR 0x06
#define CTRL_ADR 0x07

// DS1307 Seconds Register bits
#define CH 7

// DS1307 Hours Register bits
#define AMPM 5
#define HOURMODE 6

// DS1307 Control Register bits
#define RS0 0
#define RS1 1
#define SQWE 4
#define OUT 7

/******************************************************************************
* Global Function Declarations
******************************************************************************/
void twi_init(void);
void twi_mt_mode(uint8_t current_edit, uint8_t second, uint8_t minute, uint8_t hour, uint8_t date, uint8_t month, uint16_t year);
void twi_mr_mode(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *date, uint8_t *month, uint16_t *year);

#endif /* TWI_H_ */
