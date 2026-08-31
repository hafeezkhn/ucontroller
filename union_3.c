#include<Stdio.h>
#include<stdint.h>

#define AGE 18

int main()
{

	uint8_t age=12;
	if(age <AGE)
	{
		printf("You cannot vote! \n");
		fflush(stdout);
	}
	else
	{		printf("You can vote! \n");
	fflush(stdout);


	}

	return 0;

}
