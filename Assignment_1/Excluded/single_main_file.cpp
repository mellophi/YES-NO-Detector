#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

const int noise_offset = 45;
const double frame_size = 100.0, multiplier = 7.0;

int main(){
	FILE *input, *temp, *features, *input;
	input = fopen("D:\\IITG\\Semester 1\\Speech Processing\\Input\\yes_4.txt", "r");
	temp = fopen("temp.txt", "w");
	features = fopen("features.txt", "a");

	//removing noise and svaing in a temporary file
	remove_noise(input, temp, noise_offset, multiplier);
	fclose(temp);
	fclose(input);

	temp = fopen("temp.txt", "r");
	//reading from the temporary file and calculating the energy and zcr and saving it in a different features file
	feature_extractor(temp, features);
	fclose(temp);
	fclose(features);

	features = fopen("features.txt", "r");
	printf("%s\n", analyse_features(features));
	fclose(features);

	remove("features.txt");
	remove("temp.txt");
	return 0;
}