
#include <stdint.h>
#include <stdio.h>

union Packet
{
	uint32_t packetvalue;
	struct
	{
	uint32_t Crc  : 2;
	uint32_t Status :1;
	uint32_t Payload:12;
	uint32_t Bat: 3;
	uint32_t Sensor:3;
	uint32_t Longaddr:8;
	uint32_t Shortaddr:2;
	uint32_t Addrmode:1;

	}packetFields;


};

int main(void)
{
uint32_t Packetdata;
union Packet frame;


printf("enter the Packet data \n");
fflush(stdout);
scanf("%x",&frame.packetvalue);

/*
frame.Crc =(uint8_t)(Packetdata &0x3);
frame.Status=(uint8_t)((Packetdata >>2) &0x1);
frame.Payload=(uint16_t)((Packetdata >>3) &0xFFF);
frame.Bat=(uint8_t)((Packetdata>>15) &0x7);
frame.Sensor=(uint8_t)((Packetdata >>18)&0x7);
frame.Longaddr=(uint8_t)((Packetdata>>21) &0xFF);
frame.Shortaddr=(uint8_t)((Packetdata>>29) &0x3);
frame.Addrmode=(uint8_t)((Packetdata >>31) &0x1);
*/

printf("Crc: %x \n",frame.packetFields.Crc);
fflush(stdout);
printf("Status: %x \n",frame.packetFields.Status);
fflush(stdout);
printf("Payload: %x \n",frame.packetFields.Payload);
fflush(stdout);
printf("Bat: %x \n",frame.packetFields.Bat);
fflush(stdout);
printf("Sensor: %x \n",frame.packetFields.Sensor);
fflush(stdout);
printf("Longaddr: %x \n",frame.packetFields.Longaddr);
fflush(stdout);
printf("Shortaddr: %x \n",frame.packetFields.Shortaddr);
fflush(stdout);
printf("Addrmode: %x \n",frame.packetFields.Addrmode);
fflush(stdout);

printf("Size of strcut is %I64lld  \n" ,sizeof(frame));
}
