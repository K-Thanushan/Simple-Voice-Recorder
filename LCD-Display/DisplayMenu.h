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
        void Position_1();
        void Position_2();
        void Position_3();
        void Position_4();
        void Position_5();
        void Position_6();
        void Position_7();
        void Position_8();
        void Position_9();
        void Position_10();
        void Position_11();
        void Position_12();
        void Position_13();
        void Position_14();
        void ButtonPress();
        int _enter;
        int _back;
        int _down;
        int _up;
        int _Position;
};

#endif;