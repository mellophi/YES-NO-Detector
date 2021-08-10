// Assignment_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *input, *output;
	input = fopen("../Input/noise.txt", "r");
	output = fopen("../Output/energy_zcr_noise.txt", "a");
	char singleLine[100];
	double energy = 0, zcr = 0, p_x = 0, cnt = 0;
	
	while(!feof(input)){
		fgets(singleLine, 100, input);
		long long n = sizeof(singleLine)/sizeof(singleLine[0]);
		if(!isalpha(singleLine[0])){
			double x = _atoi64(singleLine);
			if(cnt == 100){
				fprintf(output, "%lf %lf\n", (energy/100.0), zcr);
				energy = 0;
				cnt = 0;
				zcr = 0;
			}
			energy += x*x;
			zcr += p_x*x < 0;
			cnt++;
			p_x = x;
		}
	}
	
	fclose(input);
	fclose(output);
	return 0;
}

