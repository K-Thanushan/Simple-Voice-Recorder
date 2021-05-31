#include <iostream>
#include <fstream>
using namespace std;

#define  wav_echo2_16(filin, fileout, eco_rate)                     \
short int temp;                                                 \
short int ch[eco_rate], ch2[eco_rate];                          \
int i,j;                                                        \
    for(i=0; i<eco_rate; i++)                                   \
            ch2[i] = 0;                                         \
while(!filin.eof())                                             \
{                                                               \
    for(i=0; i<eco_rate && !filin.eof(); i++)                   \
    filin.read((char*)&ch[i], sizeof(short int));               \
    for(i=0; i<eco_rate; i++)                                   \
            {                                                   \
                temp  = ch[i];                                  \
                ch[i]+=ch2[i];                                  \
                fileout.write((char*)&ch[i], sizeof(short int));\
                ch2[i] = temp;                                  \
            }                                                   \
                                                                \
}


struct WAVEFILEHEADER
{
char ChunkId[4];
int ChunkSize;
char Format[4];
char SubChunkFmt[4];
int SubChunkFmtSize;
short int AudioFormat;
short int NumChannels;

int SampleRate;
int ByteRate;
short int BlockAlign;
short int BitsPerSample;

char ChunkData[4];
int SubChunkDataSize;
};

int main () {
  fstream filin("africa-toto.wav",ios::in|ios::binary);
  fstream fileout("ExampleWrite.wav",ios::out| ios::binary);
  WAVEFILEHEADER wav;
  filin.read((char*)&wav,sizeof(wav));
  fileout.write((char*)&wav, sizeof(wav));/// write the header to output file 
  wav_echo2_16(filin,fileout,10000);  ///modify the data and update the output file
  filin.close();
  fileout.close();
  return 0;
}