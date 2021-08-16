#pragma once
#include "arduino.h"

class VoiceRecorder
{
	public:
	
		VoiceRecorder(int UpButton, int DownButton, int EnterButton, int BackButton);
		void MenuPosition1(int a, String Displaytext1, String Displaytext2);
		void MenuPosition2(int a, int b, String Displaytext1, String Displaytext2);
		void startRecording();
		void stopRecording();
		void startPlayback(String fileName);
		void freq_shift(String fileName);
		void Start(void);
		void Enter(void);
		void Up(void);
		void Down(void);
		void Back(void);

	private:
		static void changeState();
		String threedig(int decimal);
		void ADC_init();
		int readADC(int channel);
};