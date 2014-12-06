#include "lcd.h"

LCD::LCD(uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	init(1, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

void LCD::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	_rs_pin = rs;
	_rw_pin = rw;
	_enable_pin = enable;

	_data_pins[0] = d0;
	_data_pins[1] = d1;
	_data_pins[2] = d2;
	_data_pins[3] = d3;
	_data_pins[4] = d4;
	_data_pins[5] = d5;
	_data_pins[6] = d6;
	_data_pins[7] = d7;

	pinMode(_rs_pin, OUTPUT);
	pinMode(_rw_pin, OUTPUT);
	pinMode(_enable_pin, OUTPUT);

	_displayfunction = LCD_1LINE | LCD_5x8DOTS;

	if (fourbitmode) {
		_displayfunction |= LCD_4BITMODE;
	} else {
		_displayfunction |= LCD_8BITMODE;
	}

	begin(16, 1);
}

void LCD::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
	if (lines > 1) {
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;
	_currline = 0;
	
	delayMicroseconds(50000);

	digitalWrite(_rs_pin, LOW);
	digitalWrite(_enable_pin, LOW);
	digitalWrite(_rw_pin, LOW);

	if (! (_displayfunction & LCD_8BITMODE)) {
		// try to set 4 bit mode
		for (int i = 0; i < 3; i++) {
			write4bits(0x03);
			delayMicroseconds(4500);
		}

		// try to set 4-bit interface
		write4bits(0x02);
	}


	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOTOFF | LCD_BLINKOFF;
	
	// init to default text direction (from left to right)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	command(LCD_FUNCTIONSET | _displayfunction);
	command(LCD_ENTRYMODESET | _displaymode);

	display();
	clear();
}

void LCD::command(uint8_t value)
{
	send(value, LOW);
}

size_t LCD::write(uint8_t value)
{
	send(value, HIGH);
	return 1;
}

void LCD::send(uint8_t value, uint8_t mode)
{
	digitalWrite(_rs_pin, mode);

	digitalWrite(_rw_pin, LOW);

	if (! (_displayfunction & LCD_8BITMODE)) {
		write4bits(value >> 4);
		write(value);
	}
}

void LCD::write4bits(uint8_t value)
{
	for (int i = 0; i < 4; i++) {
		pinMode(_data_pins[i], OUTPUT);
		digitalWrite(_data_pins[i], (value >> i) & 0x01);
	}

	pulseEnable();
}

void LCD::pulseEnable()
{
	/*
	When data is supplied to data pins, a high-t-low pulse must be apploed to this pin
	in order for LCD to latch in the data present at the data pins. This pulse must be 
	a minimum of 450ns wide.
	*/

	digitalWrite(_enable_pin, LOW);
	delayMicroseconds(1);

	digitalWrite(_enable_pin, HIGH);
	delayMicroseconds(1);

	digitalWrite(_enable_pin, LOW);
	delayMicroseconds(100);  // commands need > 37ns to settle
}