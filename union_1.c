#include<stdio.h>
#include<stdint.h>

union Address
{
	uint16_t shortAddr;
	uint32_t longAddr;

};

int main()
{
	union Address addr;
	addr.shortAddr=0xABCD;
	addr.longAddr =0xFFFFAAAA;
	printf("Short addr =%x \n",addr.shortAddr);
	fflush(stdout);
	printf("long addr =%x \n",addr.longAddr);
	fflush(stdout);
	printf("Size of %lld \n",sizeof(addr));

	return 0;
}
