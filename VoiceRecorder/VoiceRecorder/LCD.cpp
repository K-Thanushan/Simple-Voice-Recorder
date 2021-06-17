#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

#define Lcd_Direction DDRE
#define Lcd_Port PORTE
#define RS PORTE2
#define EN PORTE3


LCD::LCD(int a){

}

void LCD::Command(unsigned char command)
{
	Lcd_Port = (Lcd_Port & 0x0F) | (command & 0xF0);
	Lcd_Port &= ~(1<<RS);
	Lcd_Port |= (1<<EN);
	_delay_us(1);
	Lcd_Port &= ~(1<<EN);
	
	_delay_us(200);
	
	Lcd_Port = (Lcd_Port & 0x0F) | (command << 4);
	Lcd_Port &= ~(1<<RS);
	Lcd_Port |= (1<<EN);
	_delay_us(1);
	Lcd_Port &= ~(1<<EN);
	_delay_ms(2);
}

void LCD::Print(unsigned char Data)
{
	Lcd_Port = (Lcd_Port & 0x0F) | (Data & 0xF0);
	Lcd_Port |= (1<<RS);
	Lcd_Port |= (1<<EN);
	_delay_us(1);
	Lcd_Port &= ~(1<<EN);
	
	_delay_us(200);
	
	Lcd_Port = (Lcd_Port & 0x0F) | (Data << 4);
	Lcd_Port |= (1<<RS);
	Lcd_Port |= (1<<EN);
	_delay_us(1);
	Lcd_Port &= ~(1<<EN);
	_delay_ms(2);
}

void LCD::Initialize(void)
{
	Lcd_Direction = 0xFF;
	_delay_ms(20);
	
	Command(0x02);
	Command(0x28);
	Command(0x0c);
	Command(0x06);
	Command(0x01);
	_delay_ms(2);
}

void LCD::PrintString(char *String)
{
	for(int i=0; String[i] != 0; i++)
	{
		Print(String[i]);
	}
}

void LCD::PrintString_xy (char row, char pos, char *Str)
{
	if (row == 0 && pos<16)
	Command((pos & 0x0F)|0x80);
	else if (row == 1 && pos<16)
	Command((pos & 0x0F)|0xC0);
	PrintString(Str);
}

void LCD::Clear(void)
{
	Command (0x01);
	_delay_ms(2);		
	Command (0x80);		
}