#ifndef THERMOMETER_H_
#define THERMOMETER_H_

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
#define F_CPU 8000000UL

#define LOW 0
#define HIGH 1

#define FALSE 0
#define TRUE 1

// ROM Commands
#define SEARCH_ROM 0xF0
#define READ_ROM 0x33
#define MATCH_ROM 0x55
#define SKIP_ROM 0xCC
#define ALARM_SEARCH 0xEC

// Function Commands
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE
#define WRITE_SCRATCHPAD 0x4E
#define COPY_SCRATCHPAD 0x48
#define RECALL_E2 0xB8
#define READ_POWER_SUPPLY 0xB4

/******************************************************************************
* Global Function Declarations
******************************************************************************/
int16_t read_temperature(void);


#endif /* THERMOMETER_H_ */