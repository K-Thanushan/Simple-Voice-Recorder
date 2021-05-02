#ifndef DisplayMenu_h
#define DisplayMenu_h

#include "Arduino.h"


class DisplayMenu
{
    public:
        DisplayMenu(int UpButton, int DownButton, int EnterButton, int BackButton, int Position);
        void Start();
        void Up();
        void Down();
        void Enter();
        void Back();
        void MenuPosition(int a, String Displaytext1);
        int _enter;
        int _back;
        int _down;
        int _up;
        int _Position;
};

#endif;