#ifndef LCD_NOKIA_H_
#define LCD_NOKIA_H_

#include "stdint.h"
#include "PIT.h"

void LCD_nokia_init(void);
/*It writes a byte in the LCD memory. The place of writting is the last place that was indicated by LCDNokia_gotoXY. In the reset state
 * the initial place is x=0 y=0*/
void LCD_nokia_write_byte(uint8_t data_or_command, uint8_t data);
void LCD_nokia_clear(void);
/*It is used to indicate the place for writing a new character in the LCD. The values that x can take are 0 to 84 and y can take values
 * from 0 to 5*/
void LCD_nokia_goto_xy(uint8_t x, uint8_t y);
void LCD_nokia_bitmap(const uint8_t bitmap []);
void LCD_nokia_image_mem(uint32_t address);
void LCD_nokia_send_char(uint8_t);
void LCD_nokia_send_string(uint8_t string []);
void LCD_nokia_delay(void);

void LCD_nokia_show_welcome();
void LCD_nokia_show_error();
void LCD_nokia_clr_welcome_flag();
uint8_t LCD_nokia_get_welcome_flag();
void LCD_nokia_show_menu();
void LCD_nokia_show_sequence();
void LCD_nokia_show_sequence_playing();
void LCD_nokia_show_sequence_stopped();
void LCD_nokia_show_manual();
void LCD_nokia_show_current();
void LCD_nokia_show_time();
void LCD_nokia_show_date();

#endif /* LCD_NOKIA_H_ */
