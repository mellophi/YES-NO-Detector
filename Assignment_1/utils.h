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

void feature_extractor(FILE *input, FILE *output){
	char singleLine[100];
	double energy = 0, zcr = 0, p_x = 0, cnt = 0;
	while(!feof(input)){
		fgets(singleLine, 100, input);
		if(!isalpha(singleLine[0])){
			double x = atof(singleLine);
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