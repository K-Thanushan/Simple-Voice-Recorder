/*
 * VoiceRecorder.cpp
 *
 * Created: 6/16/2021 10:45:13 AM
 * Author : K.Thanushan
 */ 
#include <avr/io.h>
#include "VoiceRecorder.h"
#define UpButton PINH3
#define DownButton PINH4
#define EnterButton PINH5
#define BackButton PINH6
#define ButtonDirection DDRH
#define ButtonCommand PINH





int main(void)
{
    VoiceRecorder MyRecorder(1);
	MyRecorder.Start();
	ButtonDirection = 0x00;
	ButtonCommand = 0x00;
    while (1) 
    {
		if (ButtonCommand & (1<< UpButton))
		{
			MyRecorder.Up();
		}
		
		else if (ButtonCommand & (1<< DownButton))
		{
			MyRecorder.Down();
		}
		
		else if (ButtonCommand & (1<< EnterButton))
		{
			MyRecorder.Enter();
		}
		else if (ButtonCommand & (1<< BackButton))
		{
			MyRecorder.Back();
		}
    }
}

