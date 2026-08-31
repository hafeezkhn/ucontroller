#include <stdio.h>

/* write a program to decode a given 32 bit packet information and
 * print the values of different field.Create a structure with member elements
 * as packet fields
 *
 *   */
#include <stdint.h>

struct Packet
{
	uint32_t Crc :2;
	uint32_t Status:1;
	uint32_t Payload:12 ;
	uint32_t Bat :3;
	uint32_t Sensor:3 ;
	uint32_t Longaddr :8;
	uint32_t Shortaddr:2 ;
	uint32_t Addrmode :1;

};


int main(void)
{
    uint32_t  PacketData;
    printf("Enter the PacketData \n");
    fflush(stdout);

    scanf("%X",&PacketData);

    struct Packet frame;
    frame.Crc = (uint8_t)(PacketData & 0X3);

	frame.Status=(uint8_t)((PacketData>>2) & 0X1);

	frame.Payload=(uint16_t)((PacketData>>3) & 0XFFF);


	frame.Bat=(uint8_t)((PacketData>>15) & 0X7);
	frame.Sensor=(uint8_t)((PacketData>>18) & 0X7);
	frame.Longaddr=(uint8_t)((PacketData>>21) & 0XFF);
	frame.Shortaddr=(uint8_t)((PacketData>>29) & 0X3);
	frame.Addrmode=(uint8_t)((PacketData>>29) & 0X3);

	printf("Crc  : %x   \n",frame.Crc);
fflush(stdout);
	printf("Status  : %x   \n",frame.Status);
	fflush(stdout);

	printf("Payload  : %x   \n",frame.Payload);
	fflush(stdout);

	printf("Bat  : %x   \n",frame.Bat);
	fflush(stdout);

	printf("Sensor  : %x   \n",frame.Sensor);
	fflush(stdout);

	printf("Longaddr  : %x   \n",frame.Longaddr);
	fflush(stdout);

	printf("Shortaddr  : %x   \n",frame.Shortaddr);
	fflush(stdout);

	printf("Addrmode  : %x   \n",frame.Addrmode);
	fflush(stdout);

	printf("Size of Struct is %I64llu \n",sizeof(frame));
	fflush(stdout);



	for(;;);
}






