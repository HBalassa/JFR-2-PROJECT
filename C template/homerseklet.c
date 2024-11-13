/******************************************************************************
 * Created: 
 * Author :
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <math.h>
#define DS18S20_PIN 7
#include <wiringPi.h>
#include <wiringPiI2C.h>

/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint8_t data[9];
int size = sizeof(data) / sizeof(data[0]);
int16_t rawTemperature=0;

/******************************************************************************
* External Variables
******************************************************************************/


    // Reset és szenzor kiválasztása
    OneWire_reset(ow);
    OneWire_write_byte(ow, 0xCC); // Skip ROM parancs
    OneWire_write_byte(ow, 0x44); // Start conversion

    // Várakozás a konverzióra (750 ms szükséges lehet)
    delay(750);

    // Újra kiválasztjuk a szenzort
    OneWire_reset(ow);
    OneWire_write_byte(ow, 0xCC); // Skip ROM
    OneWire_write_byte(ow, 0xBE); // Read Scratchpad

    // Scratchpad beolvasása
    for (int i = 0; i < 9; i++) {
        data[i] = OneWire_read_byte(ow);
    }


}

int main() {
	pinMode(DS18B20_PIN, OUTPUT);

    // Hőmérséklet konvertálásának elindítása
    convert_temp();

    // Hőmérséklet leolvasása
    uint16_t raw_temp = read_temp();

    // Nyers adat átalakítása Celsius-fokká
    float tempC = (float)raw_temp / 16.0;

    // Kiírás a soros portra
    printf("Hőmérséklet: %.2f °C\n", tempC);

    OneWire ow;
    OneWire_init(&ow, /* GPIO Pin szám itt */);

    float temperature = readTemperature(&ow);
    printf("Hőmérséklet: %.2f °C\n", temperature);

    return 0;
}


/******************************************************************************
* Local Function Declarations
******************************************************************************/


/******************************************************************************
* Local Function Definitions
******************************************************************************/
void reverse_array(int arr[], int size) {
    int temp;
    int left = 0;
    int right = size - 1;
/******************************************************************************
* Function:         int main(void)
* Description:      main function
* Input:            
* Output:           
* Notes:            
******************************************************************************/
/******************************************************************************
* Function:         pin inicializálás
* Description:      
* Input:            
* Output:           
* Notes:            
******************************************************************************/
DDRTO-92=True;
PINTO-92=(1<<)
DDRSO=True;
/******************************************************************************
* Function:         strachpad beolvasás
* Description:      
* Input:            
* Output:           
* Notes:            
******************************************************************************/
for (int i = 0; i < 16; i++) {
        data[i] = OneWire_read_byte(ow);
    }

/******************************************************************************
* Function:         homerseklet_kiszámolása(void)
* Description:      
* Input:            
* Output:           
* Notes:            
******************************************************************************/


}

int main(void)
{

	
	/* Replace with your application code */
	while(1)
	{
	(left < right) {
        // Cseréljük a bal és jobb oldali elemeket
        data = arr[left];
        arr[left] = arr[right];
        arr[right] = data;
        // Mozgatjuk a mutatókat
        left++;
        right--;
	}
    }

	reverse_array(data, size);

for (int j = 0, i<16,j++){	
	int j = 2;
	if(data[0]==1){				//ha a hömersklet negatív
	rawTemperature= rawTemperature-pov(i,j);
	}
	else{
	rawTemperature =rawTemperature+pov(i,j);
	}  
	i++;
	
}
printf(rawTemperature);

/******************************************************************************
* Interrupt Routines
******************************************************************************/
