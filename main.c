//Aim: take data dataset/speakers_all and dataset/recordings 
//We want a data structure containing all the data
//What should the format of the ds be?

//struct Speaker
    //struct Audio
        //encoding rate (how many samples per second)
        //bit depth (how many bits per sample?)
        //stereo/duo (I think I'm only gonna support stereo for now)
        //array of struct Sample (this is variable length!?! what do we do?)
            //intensity
            //number in sequence
        //MFCC representation of audio?
    //nationality
    //other metadata?  e.g. time of clip/gender??

//We need to first use our mp3 decoder from github to convert mp3 data to the same form as our data structure above.

//Use the High-level API
#include <stdio.h> //Files from standard library with <>
#include "minimp3/minimp3_ex.h" //Header files from diff directories with ""

//First see if we can run a main function

int main(){
    printf("Hello world");
}