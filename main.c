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
    //id
    //other metadata?  e.g. time of clip/gender??

//enum CountryID
//We need to first use our mp3 decoder from github to convert mp3 data to the same form as our data structure above.

//Use the High-level API
#include <stdlib.h> //for malloc
#include <stdio.h> //Files from standard library with <>
#include <dirent.h> //for directory streams
#include <string.h> //for string functions eg strcpy
#include <stdbool.h> //for booleans
#include "minimp3/minimp3_ex.h" //Header files from diff directories with ""

//First see if we can run a main function

//How do we extract audio samples?
const char* getfilename(const char* d_name, size_t* len){
    char* final_char;

    final_char = strchr(d_name, '\0');//get pointer to first \0
    size_t filename_length = (final_char-d_name)+1;//use that to compute size of filename
    char* filename = malloc(filename_length);
    strncpy(filename, d_name, filename_length);//copy filename into filename
    *len = filename_length; //save length of filename into passed param to save
    return filename;

}

const char* concat(const char* s1, size_t len_s1, const char* s2, size_t len_s2){
    size_t concat_length = len_s1 + len_s2 -1;//-2+1 for the terminating character
    char* res = malloc(concat_length);
    strncpy(res, s1, len_s1-1);
    strncpy(res+(len_s1-1), s2, len_s2);
    return res;
}

int main(){
    //for every file in dataset/recording
    DIR *d_stream;//directory stream
    struct dirent* d_entry;//directory entry
    FILE *fptr; //file ptr for each dirent
    char d_name[] = "./dataset/recordings/";
    size_t len_d_name = sizeof(d_name)/sizeof(d_name[0]);
    d_stream = opendir(d_name);//open stream into recordings

    if(d_stream != NULL){//if our stream is correctly opened
            d_entry = readdir(d_stream);//read first file name
            while(d_entry != NULL){
                //READ FILE NAME
                size_t *filename_length = malloc(sizeof(size_t));
                const char* filename = getfilename(d_entry->d_name, filename_length);
                //EXTRACT METADATA FROM FILE NAME

                
                //OPEN FILE USING FILE NAME
                const char* concat_filename = concat(d_name, len_d_name ,filename, *filename_length);
                fptr = fopen(concat_filename, "r");
                //File processing
                fclose(fptr);

                d_entry = readdir(d_stream);
            }
        closedir(d_stream);
    }
    else{
        printf("Error encountered opening directory\n");
    }


    //get accent from filename
    //get country_id from accent
    //get id from filename
    //TODO: create different model based on gender
    //extract audio samples, using minimp3
    
}