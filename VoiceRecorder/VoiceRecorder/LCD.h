#pragma once

class LCD
{
    public:
	
        LCD(int a);
        void Command(unsigned char command);
        void Print(unsigned char Data);
        void Initialize(void);
        void PrintString(char *String);
        void PrintString_xy (char row, char pos, char *Str);
        void Clear(void);
		
};