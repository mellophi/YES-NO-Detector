#include "stdafx.h"
#include <stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <math.h>
//self created header file to save the utility functions
#include "utils.h"

#define noiseFrame 5

const char path[] = "../Input/";
char filename[1000], temp[1000];

int samples[1000000], choice;

double thresholdEnergy = 0, thresholdZCR = 0;

FILE *input, *output, *input_n, *noise_op;


int main(){
	printf("Enter 1: Remove noise and pad the data then analyse \n");
	printf("Enter 2 : Put start and end markers to the word and analyse \n");
	printf("Enter choice and filename seperated by space : ");
	scanf("%d", &choice);
	scanf("%s", &filename);
	printf("-----------------------------------\n");
	strcpy(temp, path);
	strcat(temp, filename);


	fopen_s(&input, temp, "r"); //getting the input
	fopen_s(&input_n, "normalized_input.txt", "a+"); //normalized output is saved in this file
	fopen_s(&output, "features.txt", "a+"); //extracting the features and saving
	fopen_s(&noise_op, "noise_features.txt", "a+");//values after removing noise is save in this file


	//In this process we are removing the noise and applying necessary pad
	//then extracting the features and analysing it
	if(choice == 1){
		normalize_input(input, input_n);
		rewind(input_n);
		remove_noise(input_n, noise_op, 45, 7.2);
		rewind(noise_op);
		feature_extractor(noise_op, output);
		rewind(output);
		printf("-----------------------------------\n");
		printf("OUTPUT : %s\n", analyse_features(output));
	}

	//In this process we are trying to find the start and end marker for a word and then 
	//analysing the features of that word
	else{
		normalize_input(input, input_n);
		rewind(input_n);
		feature_extractor(input_n, output);

		char singleLine[100], *stop;
	
		//finding the number of segments and the start and stop positions of each of the segments
		rewind(output);
		double energy[10000], zcr[10000];
		long start, end, seg_cnt = 0, frameCount = 0, flag = 0;
		while(!feof(output)){
			fgets(singleLine, 100, output);
			energy[frameCount] = strtod(singleLine, &stop);
			zcr[frameCount] = atof(stop + 1);
			frameCount++;
		}

		//finding threshold noise energy and threshold noise zcr using first 15 valid frames
		for(int i=0; i<noiseFrame; ++i){
			thresholdEnergy += energy[i];
			thresholdZCR += zcr[i];
		}
		thresholdEnergy /= noiseFrame;
		thresholdZCR /= noiseFrame;

		//for each frame input do this to find the start and end points
		for(int i=0; i<frameCount-3; ++i){
			if(!flag && energy[i+1] > thresholdEnergy * 3 && energy[i+2] > thresholdEnergy * 3 && energy[i+3] > thresholdEnergy * 3){
					start = i;
					flag = 1;
			}
			else if(flag && energy[i+1] <= thresholdEnergy * 3 && energy[i+2] <= thresholdEnergy * 3 && energy[i+3] <= thresholdEnergy * 3){
				end = i;
				flag = 0;
				break;
			}
		}
		if(flag == 1) end = frameCount - 3;


		printf("START FARME : %d END FRAME : %d\n", start, end);
		printf("ENERGY OF START FRAME : %lf\nENERGY OF END FRAME : %lf\n", energy[start], energy[end]);
		printf("-----------------------------------\n");
		printf("OUTPUT : %s\n", analyse_features(energy, zcr, start, end));

	}

	//cleaning the temporary files created
	fclose(input);
	fclose(output);
	fclose(noise_op);
	fclose(input_n);
	remove("normalized_input.txt");
	remove("features.txt");
	remove("noise_features.txt");
	
	return 0;
}