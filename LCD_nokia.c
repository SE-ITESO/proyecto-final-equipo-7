#include "fsl_gpio.h"
#include "fsl_port.h"
#include "SPI.h"
#include "LCD_nokia.h"
#include "stdint.h"
#include "fsl_dspi.h"
#include "GPIO.h"
#include "MEM.h"
#include "CONFIG.h"
#include "LCD_nokia_images.h"
#include "UART.h"

#include "I2C.h"
#include "MANAGER.h"


extern const uint8_t graphic[IMAGE_SIZE];//prueba
extern const uint8_t ITESO[IMAGE_SIZE];

static const uint8_t ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f
};

uint8_t string_manual_option[]="1)Manual"; /*! String to be printed in the LCD*/
uint8_t string_sequence_option[]="2)Sequence"; /*! String to be printed in the LCD*/
uint8_t string_sequence_mode[]="Sequence";
uint8_t string_sequence_stopped[]="Stopped";
uint8_t string_sequence_playing[]="Playing";
uint8_t string_manual_mode[]="Manual Mode";
uint8_t string_manual_1[]="Press any";
uint8_t string_manual_2[]="Key";

uint8_t string_current[]="Corriente";

//agregado
uint8_t masterTxData[TRANSFER_SIZE] = {0U};
uint8_t g_welcome_flag = 1;

void LCD_nokia_init(void)
{
	const gpio_pin_config_t gpio_output_config =
		{
				kGPIO_DigitalOutput,
				1
		};

	CLOCK_EnableClock(kCLOCK_Spi0);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortC);

	GPIO_PinInit(GPIOD,0,&gpio_output_config);
	GPIO_PinInit(GPIOC,DATA_OR_CMD_PIN,&gpio_output_config );
	GPIO_PinInit(GPIOC,RESET_PIN,&gpio_output_config);

	PORT_SetPinMux(PORTD,CE_LCD, kPORT_MuxAsGpio);//CE_LCD
	PORT_SetPinMux(PORTC, DATA_OR_CMD_PIN, kPORT_MuxAsGpio);//DC
	PORT_SetPinMux(PORTC, RESET_PIN, kPORT_MuxAsGpio);//RST

	GPIO_PortClear(GPIOC, PIN1 << RESET_PIN);
	LCD_nokia_delay();
	GPIO_PortSet(GPIOC, PIN1 << RESET_PIN);

	LCD_nokia_write_byte(LCD_CMD, 0x21); //Tell LCD that extended commands follow
	LCD_nokia_write_byte(LCD_CMD, 0xB1); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
	LCD_nokia_write_byte(LCD_CMD, 0x04); //Set Temp coefficent
	LCD_nokia_write_byte(LCD_CMD, 0x13); //LCD bias mode 1:48: Try 0x13 or 0x14
	LCD_nokia_write_byte(LCD_CMD, 0x20); //We must send 0x20 before modifying the display control mode
	LCD_nokia_write_byte(LCD_CMD, 0x0C); //Set display control, normal mode. 0x0D for inverse
}

void LCD_nokia_show_current(){
	LCD_nokia_clear();
	LCD_nokia_goto_xy(6,2);
	LCD_nokia_send_string(string_current);

}



void LCD_nokia_show_menu(){
	LCD_nokia_clear();
	LCD_nokia_goto_xy(6,2);
	LCD_nokia_send_string(string_manual_option);
	LCD_nokia_goto_xy(6,3);
	LCD_nokia_send_string(string_sequence_option); /*! It prints a string stored in an array*/
}

void LCD_nokia_show_sequence(){
	LCD_nokia_clear();
	LCD_nokia_goto_xy(5,0);
	LCD_nokia_send_string(string_sequence_mode);
	LCD_nokia_goto_xy(5,5);
	LCD_nokia_send_string(string_sequence_stopped);
}

void LCD_nokia_show_sequence_stopped(){
	LCD_nokia_goto_xy(5,5);
	LCD_nokia_send_string(string_sequence_stopped);
}

void LCD_nokia_show_sequence_playing(){

	LCD_nokia_goto_xy(5,5);
	LCD_nokia_send_string(string_sequence_playing);
}

void LCD_nokia_show_manual(){
	 LCD_nokia_clear();
	 LCD_nokia_goto_xy(6,2);
	 LCD_nokia_send_string(string_manual_mode);
	 LCD_nokia_goto_xy(6,3);
	 LCD_nokia_send_string(string_manual_1);
	 LCD_nokia_goto_xy(6,4);
	 LCD_nokia_send_string(string_manual_2);
}

void LCD_nokia_show_welcome()
{
	LCD_nokia_bitmap(ITESO);
}


void LCD_nokia_clr_welcome_flag()
{
	g_welcome_flag =0;
	LCD_nokia_show_menu();
	PIT_stop(PIT_CHANNEL_1);

}

uint8_t LCD_nokia_get_welcome_flag()
{
    return g_welcome_flag;
}

void LCD_nokia_bitmap(const uint8_t bitmap[]) {

    uint16_t totalBytes = SCREENW * SCREENH / 8;
    GPIO_PortSet(GPIOC, 1u << DATA_OR_CMD_PIN);

    for (uint16_t i = 0; i < totalBytes; ++i) {
        LCD_nokia_write_byte(LCD_DATA, bitmap[i]);
    }
}

void LCD_nokia_write_byte(uint8_t data_or_command, uint8_t data)
{

	GPIO_pin_clr(GPIOD, CE_LCD);
	dspi_transfer_t masterXfer;
	    //////////////////////
	if(data_or_command){
		GPIO_pin_set(GPIOC, DATA_OR_CMD_PIN);
	}
    else{
    	GPIO_pin_clr(GPIOC, DATA_OR_CMD_PIN);
    }
    masterXfer.txData      = &data;//
	masterXfer.rxData      = NULL; //no se reciben datos
	masterXfer.dataSize    = 1; //1 byte a transmitir
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	DSPI_MasterTransferBlocking(SPI0, &masterXfer);
	GPIO_pin_set(GPIOD, CE_LCD);
}

void LCD_nokia_send_char(uint8_t character) {
  uint16_t index = 0;

  LCD_nokia_write_byte(LCD_DATA, 0x00); //Blank vertical line padding

  for (index = 0 ; index < 5 ; index++)
	  LCD_nokia_write_byte(LCD_DATA, ASCII[character - 0x20][index]);
    //0x20 is the ASCII character for SpaCE_LCD (' '). The font table starts with this character

  LCD_nokia_write_byte(LCD_DATA, 0x00); //Blank vertical line padding
}

void LCD_nokia_send_string(uint8_t characters []) {
  while (*characters)
	  LCD_nokia_send_char(*characters++);
}

void LCD_nokia_clear(void) {
	uint16_t index = 0;
  for (index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++)
	  LCD_nokia_write_byte(LCD_DATA, 0x00);
  LCD_nokia_goto_xy(0, 0); //After we clear the display, return to the home position
}

void LCD_nokia_goto_xy(uint8_t x, uint8_t y) {
	LCD_nokia_write_byte(LCD_CMD, 0x80 | x);  // Column.
	LCD_nokia_write_byte(LCD_CMD, 0x40 | y);  // Row.  ?
}

void LCD_nokia_delay(void)
{
	int counter;

	for(counter=0; counter<1500; counter++)
	{

	}
}



//Para mandar fecha a display
//para mandar a  display
void LCD_nokia_show_date() {
    uint8_t formattedDate[6];

    uint8_t* uart_rx_date = UART_get_rx_date();
    	uint8_t* i2c_rx_date = I2C_get_rx_date();

    	uint16_t year =  i2c_rx_date[I2C_YEAR_INDEX];
    	    uint8_t month = i2c_rx_date[I2C_MONTH_INDEX];
    	    uint8_t day = i2c_rx_date[I2C_DAY_INDEX];

    formattedDate[0] = (day >> 4) & UNITS_MASK;       // Tens of Day
    formattedDate[1] = day & UNITS_MASK;              // Units of Day
    formattedDate[2] = (month >> 4) & UNITS_MASK;     // Tens of Month
    formattedDate[3] = month & UNITS_MASK;            // Units of Month
    formattedDate[4] = (year >> 4) & UNITS_MASK;      // Tens of Year
    formattedDate[5] = year & UNITS_MASK;             // Units of Year

    LCD_nokia_clear();
    LCD_nokia_goto_xy(2, 0);
    LCD_nokia_send_char(formattedDate[0] + '0');
    LCD_nokia_send_char(formattedDate[1] + '0');
    LCD_nokia_send_char('/');
    LCD_nokia_send_char(formattedDate[2] + '0');
    LCD_nokia_send_char(formattedDate[3] + '0');
    LCD_nokia_send_char('/');
    LCD_nokia_send_char('2');
    LCD_nokia_send_char('0');
    LCD_nokia_send_char(formattedDate[4] + '0');
    LCD_nokia_send_char(formattedDate[5] + '0');
}


void LCD_nokia_show_time() {
    uint8_t formattedTime[6];

      uint8_t* i2c_rx_time = I2C_get_rx_time();

            uint8_t seconds = i2c_rx_time[I2C_SECONDS_INDEX];
             uint8_t minutes = i2c_rx_time[I2C_MINUTES_INDEX];
             uint8_t hours = i2c_rx_time[I2C_HOURS_INDEX];


    formattedTime[0] = ((hours >> 4) & UNITS_MASK);       // Tens of Hours
    formattedTime[1] = hours & UNITS_MASK;                // Units of Hours
    formattedTime[2] = ((minutes >> 4) & UNITS_MASK);     // Tens of Minutes
    formattedTime[3] = minutes & UNITS_MASK;              // Units of Minutes
    formattedTime[4] = ((seconds >> 4) & UNITS_MASK);     // Tens of Seconds
    formattedTime[5] = seconds & UNITS_MASK;              // Units of Seconds


    LCD_nokia_goto_xy(2, 1);
    LCD_nokia_send_char(formattedTime[0] + '0');
    LCD_nokia_send_char(formattedTime[1] + '0');
    LCD_nokia_send_char(':');
    LCD_nokia_send_char(formattedTime[2] + '0');
    LCD_nokia_send_char(formattedTime[3] + '0');
    LCD_nokia_send_char(':');
    LCD_nokia_send_char(formattedTime[4] + '0');
    LCD_nokia_send_char(formattedTime[5] + '0');
}


void LCD_nokia_show_data(){

	LCD_nokia_show_date();
		LCD_nokia_show_time() ;



}

