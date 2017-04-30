#include <stdio.h>
#include <string.h>
#include <stdlib.h>

	int main(int argc, char* argv[])
{
	if(argc > 2)
	{
		int i;
		int validInput = 1;
		int sum = 0;
		for(i=1; i<argc-1; i++)
		{
			int num = atoi(argv[i]);
			if(strcmp(argv[i],"0") != 0 && num == 0)
			{
				validInput = 0;
				break;
			}
			else sum = sum + num;
		}
		if(validInput == 1)
		{
			if(sum == atoi(argv[argc-1])) printf("1");
			else printf("0");
		}
		else printf("Invalid Input: The numbers you passed cannot be converted to integers!\n");
	}
	else if(argc == 2 && strcmp(argv[1],"-h")==0)
	{
		printf("Syntax:\t./program <int1> <int2> ... <intn>\n\nNote that you must pass at least 2 integers as parameters.\n");
	}
	else
	{
		printf("You must pass at least two parameters!\n");
	}
	return 0;
}
