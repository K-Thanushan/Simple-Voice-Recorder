/*
 * VoiceRecorder.cpp
 *
 * Created: 6/16/2021 10:45:13 AM
 * Author : K.Thanushan
 */ 
#include <avr/io.h>
#include "VoiceRecorder.h"
#define UpButton PIND1
#define DownButton PIND2
#define EnterButton PIND3
#define BackButton PIND4
#define ButtonDirection DDRD
#define ButtonCommand PIND





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

