# Audio Extraction
This project is an attempt at creating an accent detector

# Steps 
## 1. Get Dataset
Speech accent archive is the dataset I am going to use for this project:
https://www.kaggle.com/datasets/rtatman/speech-accent-archive/data
Just download and unzip it

## 2. Read dataset contents into program
We want waveform data in our program. 
But what do we mean by waveform data?
At the moment, our input data is encoded in mp3. 
But we want to extract the intensity and timestamp of the data only, in order to extract waveform data.
So we will have to decode the data from mp3 to some other format (we want lpcm in particular), using a third part lib
although I would like to create an MP3 encoder/decoder as well, this would be too much for this project.
We will use this github repo to take mp3 to pcm:
https://github.com/lieff/minimp3

### How do we use this project?
