# YES-NO Detector

This project is developed for detection of "YES" and "NO" using the Acoustic and Phonetics approach.
Here we try to first mark the end and start of the word using the Short Term Energy (for 100 frames) and
Zero Cross Rate (ZCR).

Following is the classification we do - 
* If the energy of i, i+1, i+2 frames are more than the energy of noise then we say that the word has started.
* If the energy of i, i+1, i+2 frames is less than 3 * the threshold energy then we say that the word has ended.
* After getting the word we just analyse the features (energy and zcr) and if 30% of the word contains high zcr then we say it is YES.
* Else we say it is NO.

## Instructions to run the file
* Clone this repo or download it as zip.
* Go inside and open the [Assignment_1.sln](https://github.com/mellophi/YES-NO-Detector/blob/master/Assignment_1.sln) file in Visual Studio 10+.
* Run the main_file.cpp (CTRL + F5).

## Input
Here I have used the sample input provided to use and divided it into 8 parts. Ran the program in each of the parts.
If you want to use your own input the do the following steps - 
* Record a single word (YES/NO) and save it as a .txt file in the [Input](https://github.com/mellophi/YES-NO-Detector/tree/master/Input) folder.
* Give that name while running it.

## Preprocessing
Before we try to do any analysis we have to remove the DC Shift and Normalize the input to 5000.

## Processing
I have created a header file [utils.h](https://github.com/mellophi/YES-NO-Detector/blob/master/Assignment_1/utils.h) which contains the utitlity function for all the things that has been done.
The [main_file.cpp](https://github.com/mellophi/YES-NO-Detector/blob/master/Assignment_1/main_file.cpp) contians the code to run the program.
Here, I have tried out 2 types of process to do the classification.

* _Process 1_ : Removing all the noise and using necessary padding then analysing the input
* _Process 2_ : Marking the start and end of the word and then analysing the input.

When we first run the application it will ask which process we want to use. Process 1 provided more accuracy than process 2 but it contains following
hardcoded data due to which it's dynamic nature decreases.

### HARDCODED DATA FOR FIRST PROCESS

Noise offset = +45/-45 (representing the ambient noise which has been found by analysing the noise as input)
Muliplier = 7.2 (found by tuning)

Process 2 however does not use any such hardcoded data due to which it's dynamic nature is more. We can test both of them out and check for ourself 😄

## Output
![picture alt](https://github.com/mellophi/YES-NO-Detector/blob/master/Output/process1.PNG "Output for process 1")

![picture alt](https://github.com/mellophi/YES-NO-Detector/blob/master/Output/process2.PNG "Output for process 2")

