#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#define DS1307_ADR 0b1101000

uint8_t decimal_to_bcd(uint16_t);

void main(void) {
  uint8_t num = (DS1307_ADR<<1) | (1<<0);

  for(uint8_t i=0; i<8; i++) {
    if((num & (1<<(7-i))) == 0) printf("0");
    else printf("1");
  }
  
  
  // decimal_to_bcd(2145);
}

uint8_t decimal_to_bcd(uint16_t dec_num) {
  if(dec_num == 0) return 0;
  // to convert from dec to bcd, just simply convert every digit to a 4-bit binary number
  // intervall: 0...59 | 2000...2100

  // examine the length of the number
  uint8_t length = 0;
  uint16_t result = dec_num / pow(10, length);

  while(result != 0) {
    length++;
    result = dec_num / pow(10, length);
  }

  // write to bcd
}