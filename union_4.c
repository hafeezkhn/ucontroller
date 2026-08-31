#include<Stdio.h>
#include<stdint.h>

#define PI_VALUE 3.1415F
#define AREA_OF_A_CIRCLE(r)  ((PI_VALUE)*(r)*(r))

int main()
{
	float area_Circle;

// 3.1415 * 1+1* 1+1
	area_Circle=AREA_OF_A_CIRCLE(1+1);
	printf("Area =%f \n",area_Circle);
	fflush(stdout);

	return 0;
}
