#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#define DS1307_ADR 0b1101000

uint8_t decimal_to_bcd(uint16_t);

void main(void) {
  uint8_t num = (DS1307_ADR<<1) | (1<<0);

  // check DS1307_ADR
  for(uint8_t i=0; i<8; i++) {
    if((num & (1<<(7-i))) == 0) printf("0");
    else printf("1");
  }

  printf("\n");
  
  // check decimal to bcd
  for(uint8_t i=0; i<8; i++) {
    if(i == 4) {
      printf(" ");
    }
    if((decimal_to_bcd(46) & (1<<(7-i))) == 0) printf("0");
    else printf("1");
  }
}

uint8_t decimal_to_bcd(uint16_t dec_num) {
  // to convert from dec to bcd, just simply convert every digit to a 4-bit binary number
  // the input is between 0...99
  if(dec_num > 99) return -1;

  // dec_num = second_digit*10 + first_digit
  uint8_t first_digit = dec_num % 10;
  uint8_t second_digit = dec_num / 10;

  // number in bcd
  uint8_t bcd_num = (second_digit<<4) | (first_digit);

  return bcd_num;
}
