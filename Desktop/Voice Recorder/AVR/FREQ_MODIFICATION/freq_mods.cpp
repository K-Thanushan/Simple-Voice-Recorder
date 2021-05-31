#include <iostream>
#include <fstream>
#include <complex>
#include <cmath>

using namespace std;

complex<int> mycomplex_num (0,1);
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

void freq_shift(string file_in_name, string file_out_name, int shift, int sample_rate){
    fstream filin_1(file_in_name,ios::in|ios::binary);
    fstream fileout_1(file_out_name,ios::out| ios::binary);
    WAVEFILEHEADER wav;
    filin_1.read((char*)&wav,sizeof(wav));
    fileout_1.write((char*)&wav, sizeof(wav));
    int time = 0;
    while (!filin_1.eof()){
        short int ch = 0;
        filin_1.read((char*)&ch, sizeof(short int));
        float ret_val = ch*time*exp(imag(mycomplex_num)*shift*time);
        fileout_1.write((char*)&ret_val, sizeof( float));
        time += 1/sample_rate;
    }
    filin_1.close();
    fileout_1.close();
}

void wav_echo(string file_in_name, string file_out_name, int eco_rate) {
        fstream filin(file_in_name,ios::in|ios::binary);
        fstream fileout(file_out_name,ios::out| ios::binary);
        WAVEFILEHEADER wav;
        filin.read((char*)&wav,sizeof(wav));
        fileout.write((char*)&wav, sizeof(wav));
        short int temp;                                                 
        short int ch[eco_rate], ch2[eco_rate];                          
        int i,j;                                                        
            for(i=0; i<eco_rate; i++)                                   
                    ch2[i] = 0;                                         
        while(!filin.eof())                                             
        {                                                              
            for(i=0; i<eco_rate && !filin.eof(); i++)                   
            filin.read((char*)&ch[i], sizeof(short int));            
            for(i=0; i<eco_rate; i++)                                   
                    {                                                   
                        temp  = ch[i];                                  
                        ch[i]+=ch2[i];                                  
                        fileout.write((char*)&ch[i], sizeof(short int));
                        ch2[i] = temp;                                  
                    }                                                   
                                                                        
        }
        filin.close();
        fileout.close();
}

int main () {
  wav_echo("africa-toto.wav","echo.wav",10000);  ///modify the data and update the output file 
  freq_shift("africa-toto.wav","shift.wav",16000,300);
  return 0;
}