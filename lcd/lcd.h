#ifndef LCD_H
#define LCD_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define OUTPUT 1  // TODO: remove this

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for displaymode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for displaycontrol
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for displayfunction
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class LCD {
	public:
		LCD(uint8_t rs, uint8_t rw, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

		LCD(uint8_t rs, uint8_t rw, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

		void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
				uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
				uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
		void begin(uint8_t cols, uint8_t rows, uint8_t dotsize = LCD_5x8DOTS);


		void clear();
		void home();

		void createChar(uint8_t, uint8_t[]);
		void setCursor(uint8_t, uint8_t);
		size_t write(uint8_t);
	
	private:
		void send(uint8_t, uint8_t);
		void write4bits(uint8_t);
		void pulseEnable();

		uint8_t _rs_pin; // LOW: command. HIGH: character.
		uint8_t _rw_pin; // LOW: write to LCD. HIGH: read from LCD.
		uint8_t _enable_pin; // activated by a HIGH pulse.
		uint8_t _data_pins[8];

		uint8_t _displayfunction;
		uint8_t _displaycontrol;
		uint8_t _displaymode;

		uint8_t _numlines;
		uint8_t _currline;
};

#endif