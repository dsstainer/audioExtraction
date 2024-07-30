//Use the High-level API
#include <stdlib.h> //for malloc
#include <stdio.h> //Files from standard library with <>
#include <dirent.h> //for directory streams
#include <string.h> //for string functions eg strcpy
#include <stdbool.h> //for booleans
#define MINIMP3_IMPLEMENTATION
#include "minimp3/minimp3_ex.h" //Header files from diff directories with ""
#include "minimp3/minimp3.h"

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
struct Audio{
    int hz;
    int bitrate;
    int channels;
    int num_samples;
    mp3d_sample_t* samples;//array of int16_ts (same as mp3d_sample_t)
    //MFCCs?
};
struct Speaker{
    int nationality;
    char* nationality_name; //pointer to array of chars (i.e. first element in array)
    size_t len_nationality_name;
    int number;
    struct Audio* audio;
};

//enum CountryID
//We need to first use our mp3 decoder from github to convert mp3 data to the same form as our data structure above.


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

int stringToInt(const char* s, size_t s_len){
    int res = 0;
    int temp = 0;
    int mult = 1;
    for(int i=s_len-2; i>-1;i--){//-2 since we start from before the \0
        char c = s[i];
        temp = s[i] - '0';
        res += (temp*mult);
        mult *= 10;
    }
    return res;
}

void extractMetadata(const char* filename, size_t filename_length, struct Speaker* currentSpeaker){
    char digit[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    size_t digit_len = 10;
    int current = 0;
    bool numFound = false;
    while(numFound == false){
        //Check whether current is a number
        for(int i=0; i<digit_len; i++){
            if(filename[current] == digit[i]){
                numFound = true;
                break;
            }
        }
        current += 1;
    }
    current -= 1;//index of first digit 
    //we want 0 to current-1
    char* nationality_name = malloc((current+1)*sizeof(char));
    strncpy(nationality_name, filename, current);
    nationality_name[current] = '\0';
    //and current to filename_length - 4 (the .mp3)
    char* number_chararr = malloc(((filename_length-4-current))*sizeof(char));
    strncpy(number_chararr, filename+current, (filename_length-4-(current+1)));//-2 from -4(.mp3) +1(len) +1(\0)
    number_chararr[(filename_length-4-(current+1))] = '\0';
    //paseint
    int number = stringToInt(number_chararr, ((filename_length-current)-4)); //TODO: this may have to be on the heap??? deallocated at end of this function?

    //set data
    currentSpeaker->nationality_name = nationality_name;
    currentSpeaker->len_nationality_name = current+1;
    currentSpeaker->number = number;

    //free memory
    free(number_chararr);
}

int getlenspeakerarr(char* d_name, size_t len){
    int res = 0;
    DIR* d_stream;
    struct dirent* d_entry;
    d_stream = opendir(d_name);
    if(d_stream != NULL){
        d_entry = readdir(d_stream);
        while(d_entry != NULL){
            if(d_entry->d_name[0] != '.'){
                res += 1;
            }
            d_entry = readdir(d_stream);
        }
    }
    closedir(d_stream);
    return res;
}

void extractAudioData(const char* concat_filename, size_t len_concat_filename, struct Speaker* currentSpeaker){
    FILE* fptr;
    mp3dec_t mp3d;
    mp3dec_file_info_t info;
    if(mp3dec_load(&mp3d, concat_filename, &info, NULL, NULL)){
        //error
        printf("Error opening file");
    }
    //info.buffer is an array of size info.samples which contains all the audio samples.
    //We can set our audio to this.
    struct Audio *a = malloc(sizeof(struct Audio));
    a->samples = info.buffer;
    a->num_samples = info.samples;
    a->bitrate = info.avg_bitrate_kbps;
    a->channels = info.channels;
    a->hz = info.hz;
    currentSpeaker->audio = a;
}

int main(){
    //for every file in dataset/recording
    DIR *d_stream;//directory stream
    struct dirent* d_entry;//directory entry
    FILE *fptr; //file ptr for each dirent
    char d_name[] = "./dataset/recordings/";
    size_t len_d_name = sizeof(d_name)/sizeof(d_name[0]);
    size_t len_speakerArr = getlenspeakerarr(d_name, len_d_name);
    struct Speaker** speakerArr = malloc(len_speakerArr*sizeof(struct Speaker*));
    d_stream = opendir(d_name);//open stream into recordings

    int speakerArrIndex = 0;
    if(d_stream != NULL){//if our stream is correctly opened
            d_entry = readdir(d_stream);//read first file name
            while(d_entry != NULL){
                //READ FILE NAME
                size_t *filename_length = malloc(sizeof(size_t));
                const char* filename = getfilename(d_entry->d_name, filename_length);
                printf("%i / %i\n", speakerArrIndex+1, len_speakerArr);
                //skip any files starting with .
                if(filename[0] != '.'){
                    //EXTRACT METADATA FROM FILE NAME
                    struct Speaker* currentSpeaker = malloc(sizeof(struct Speaker));
                    extractMetadata(filename, *filename_length, currentSpeaker);
                    //OPEN FILE USING FILE NAME
                    const char* concat_filename = concat(d_name, len_d_name ,filename, *filename_length);
                    //File processing
                    extractAudioData(concat_filename, len_d_name+*filename_length-1, currentSpeaker);
                    
                    //Save to data structure
                    speakerArr[speakerArrIndex] = currentSpeaker;
                    speakerArrIndex += 1;

                    //freeing memory
                    free((char*)concat_filename);
                    
                }

                d_entry = readdir(d_stream);
                //freeing memory used on this iteration...
                free(filename_length);
                free((char*)filename);//we are allowed to cast const away only when using free? we want filename to be const throughout its lifetime
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
    for(int i=0; i<len_speakerArr; i++){
        free(speakerArr[i]->nationality_name);
        //TODO: some freeing of audio
        free(speakerArr[i]->audio->samples);// i.e. free(info.buffer)
        free(speakerArr[i]->audio);
        free(speakerArr[i]);
        scanf("Hi");
    }
    free(speakerArr);
}