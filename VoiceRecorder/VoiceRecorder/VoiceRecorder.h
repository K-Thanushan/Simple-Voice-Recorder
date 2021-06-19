#pragma once

class VoiceRecorder
{
	public:
	
		VoiceRecorder(int d);
		void MenuPosition1(int a, char *Displaytext1, char *Displaytext2);
		void MenuPosition2(int a, int b, char *Displaytext1, char *Displaytext2);
		void Start(void);
		void Enter(void);
		void Up(void);
		void Down(void);
		void Back(void);
	
};