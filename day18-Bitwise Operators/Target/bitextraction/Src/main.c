

#include <stdint.h>
/*
Extract bit positions from 9th to 14th [14:9] in a
given data and save it in to another variable .

Data =0x BC10;

Binary Value for 0XBC10 = 1011 1100 0001 0000;

Data to be extracted =011 110

Binary Value for 0XBC10 after shift =  0000 0000 01 01 1110;
&
Mask data = 0000 0000 00 11 1111
Output Data=01 1110;

Hex Value for Mask Data= 3F;

ANSWER IS 1E;
 */

int main(void)
{
    /* Loop forever */
	uint16_t Data =0xBC10;
	uint8_t ExtractedBits;

	ExtractedBits= ((Data >> 9) & (0x3F));


	for(;;);
}
