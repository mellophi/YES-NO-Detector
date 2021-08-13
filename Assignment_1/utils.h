//function to remove noise from the segmented input
void remove_noise(FILE *input, FILE *output, int noise_offset, int multiplier){
	char singleLine[100];
	while(!feof(input)){
		fgets(singleLine, 100, input);
		int x = atoi(singleLine);
		//removing the noise from the data for a single word and then saving it to a temporary file
		if(abs(x) > noise_offset * multiplier){
			fprintf(output, "%d\n", x);
		}
	}
}

//fuction to extract the features of the speech (Energy and ZCR) and save it in a different file
void feature_extractor(FILE *input, FILE *output){
	char singleLine[100];
	double energy = 0, zcr = 0, p_x = 0, cnt = 0;
	while(!feof(input)){
		fgets(singleLine, 100, input);
		if(!isalpha(singleLine[0])){
			double x = atof(singleLine);
			if(abs(x) > 1){
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
	}
}

//function to analyse the features of the speech and give output accordingly for the entire file
char* analyse_features(FILE *input){
	char singleLine[100], *stopstring;
	double energy, zcr, count_frames = 0, count_fricatives = 0;
	while(!feof(input)){
		fgets(singleLine, 100, input);
		energy = strtod(singleLine, &stopstring);
		zcr = atof(stopstring+1);
		if(zcr > 20)
			count_fricatives ++;
		count_frames ++;
	}
	if(count_fricatives >= count_frames * 0.4)
		return "YES";
	else
		return "NO";
}

//for a perticular part of the file
char* analyse_features(FILE *input, long start, long end){
	char singleLine[100], *stopstring;
	double energy, zcr, count_frames = 0, count_fricatives = 0;
	fseek(input, start, SEEK_SET);
	while(!feof(input) && ftell(input) <= end){
		fgets(singleLine, 100, input);
		energy = strtod(singleLine, &stopstring);
		zcr = atof(stopstring+1);
		if(zcr > 15)
			count_fricatives ++;
		count_frames ++;
	}
	if(count_fricatives >= count_frames * 0.4)
		return "YES";
	else
		return "NO";
}

//analyse features using the energy and zcr array
char* analyse_features(double *energy, double *zcr, long start, long end){
	double count_fricatives = 0;
	for(int i=start; i<=end-1; ++i){
		if(zcr[i] > 15 && zcr[i+1] > 15)
			count_fricatives ++;
	}
	double count_frames = end - start + 1;
	printf("count frames : %lf\ncount fricatives : %lf\n", count_frames, count_fricatives);
	if(count_fricatives >= count_frames * 0.3)
		return "YES";
	else
		return "NO";
}

//creates a temporary file by extracting information from the input file in the given range
void save_temp(FILE *input, FILE *output, long start, long end){
	char singleLine[100], *stopstring;
	fseek(input, start, SEEK_SET);
	while(!feof(input) && ftell(input) <= end){
		fgets(singleLine, 100, input);
		fprintf(output, singleLine);
	}
}

//returns the number of null samples in the input (0, 1, -1)
long count_null(FILE *input){
	char singleLine[100];
	long cnt = 0;
	while(!feof(input)){
		fgets(singleLine, 100, input);
		if(!isalpha(singleLine[0])){
			int x = atoi(singleLine);
			if(x != 0 && x != 1 && x != -1){
				cnt = ftell(input);
				break;
			}
		}
	}
	return cnt;
}

//normalizing the input and giving an output file which contains the normalized output
void normalize_input(FILE *input, FILE *output, int limit = 5000){
	long max = 0, dcShift = 0, sampleCount = 0;
	char singleLine[100];

	//calculate DC shift
	while(!feof(input)){
		fgets(singleLine, 100, input);
		if(!isalpha(singleLine[0])){
			int x = atoi(singleLine);
			dcShift += x;
			sampleCount ++;
		}
	}
	dcShift /= sampleCount;

	//calculate normalization factor
	rewind(input);
	while(!feof(input)){
		fgets(singleLine, 100, input);
		if(!isalpha(singleLine[0])){
			int x = atoi(singleLine);
			if(max < abs(x)){
				max = abs(x);
			}
		}
	}
	double normalizationFactor = (max - limit)/(max * 1.0);
	rewind(input);
	while(!feof(input)){
		fgets(singleLine, 100, input);
		if(!isalpha(singleLine[0])){
			double x = atoi(singleLine);
			int normalized_output = floor((x-dcShift) * normalizationFactor);
			if(abs(normalized_output) > 1)
				fprintf(output, "%d\n", normalized_output);
		}
	}

	printf("SAMPLES : %d\nMAX AMPLITUDE : %d\nNORMALIZATION FACTOR : %lf\nDC SHIFT : %lf\n", sampleCount, max, normalizationFactor, dcShift);
}