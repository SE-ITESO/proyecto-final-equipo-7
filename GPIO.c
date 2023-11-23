/*
*   @file GPIO.c
*
*	@brief
*		This is the source file for the GPIO device driver for Kinetis K64.
*		It contains all the implementation for configuration functions and runtime functions.
*		i.e., this is the application programming interface (API) for the GPIO peripheral.'
*
*	@author J. Luis Pizano Escalante, luispizano@iteso.mx
*
*	@date	19/02/2021
*
*	@used and modified by Diego Delgado, Alberto Quintana
*
 */
#include "MK64F12.h"
#include "GPIO.h"
#include "CONFIG.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include <stdbool.h>

volatile static gpio_interrupt_flags_t g_intr_status_flag = {0};

volatile static uint32_t g_intr_register_dec_pin = 0;

volatile static uint32_t g_intr_register_pin = 0;

volatile static boolean_t g_b1_pressed = FALSE;
volatile static boolean_t g_b2_pressed = FALSE;
volatile static boolean_t g_menu_pressed = FALSE;
volatile static boolean_t g_kick_pressed = FALSE;
volatile static boolean_t g_snare_pressed = FALSE;
volatile static boolean_t g_bongo_pressed = FALSE;
volatile static boolean_t g_hithat_pressed = FALSE;
volatile static boolean_t g_guiro_pressed = FALSE;
volatile static boolean_t g_rimshot_pressed = FALSE;
volatile static boolean_t g_tambor_pressed = FALSE;
volatile static boolean_t g_cynbal_pressed = FALSE;
volatile static boolean_t g_cowbell_pressed = FALSE;

static void (*GPIOA_callback)(uint32_t flags) = 0;
static void (*GPIOB_callback)(uint32_t flags) = 0;
static void (*GPIOC_callback)(uint32_t flags) = 0;
static void (*GPIOD_callback)(uint32_t flags) = 0;

boolean_t GPIO_get_b1_pressed(){
	return g_b1_pressed;
}
void GPIO_set_b1_pressed(boolean_t value){
	g_b1_pressed = value;
}

boolean_t GPIO_get_b2_pressed(){
	return g_b2_pressed;
}
void GPIO_set_b2_pressed(boolean_t value){
	g_b2_pressed = value;
}

boolean_t GPIO_get_menu_pressed(){
	return g_menu_pressed;
}
void GPIO_set_menu_pressed(boolean_t value){
	g_menu_pressed = value;
}

boolean_t GPIO_get_kick_pressed(){
	return g_kick_pressed;
}

boolean_t GPIO_get_snare_pressed(){
	return g_snare_pressed;
}

boolean_t GPIO_get_bongo_pressed(){
	return g_bongo_pressed;
}

boolean_t GPIO_get_hithat_pressed(){
	return g_hithat_pressed;
}

boolean_t GPIO_get_guiro_pressed(){
	return g_guiro_pressed;
}

boolean_t GPIO_get_rimshot_pressed(){
	return g_rimshot_pressed;
}

boolean_t GPIO_get_tambor_pressed(){
	return g_tambor_pressed;
}

boolean_t GPIO_get_cynbal_pressed(){
	return g_cynbal_pressed;
}

boolean_t GPIO_get_cowbell_pressed(){
	return g_cowbell_pressed;
}

void GPIO_set_kick_pressed(boolean_t value){
	g_kick_pressed = value;
}

void GPIO_set_snare_pressed(boolean_t value){
	g_snare_pressed = value;
}

void GPIO_set_bongo_pressed(boolean_t value){
	g_bongo_pressed = value;
}

void GPIO_set_hithat_pressed(boolean_t value){
	g_hithat_pressed = value;
}

void GPIO_set_guiro_pressed(boolean_t value){
	g_guiro_pressed = value;
}

void GPIO_set_rimshot_pressed(boolean_t value){
	g_rimshot_pressed = value;
}

void GPIO_set_tambor_pressed(boolean_t value){
	g_tambor_pressed = value;
}

void GPIO_set_cynbal_pressed(boolean_t value){
	g_cynbal_pressed = value;
}

void GPIO_set_cowbell_pressed(boolean_t value){
	g_cowbell_pressed = value;
}



void GPIO_callback_init(GPIO_Name_t gpio, void (*handler)(uint32_t flags))
{
	switch(gpio){
		case GPIOA_name:
			GPIOA_callback = handler;
			break;

		case GPIOB_name:
			GPIOB_callback = handler;
			break;

		case GPIOC_name:
			GPIOC_callback = handler;
			break;

		case GPIOD_name:
			GPIOD_callback = handler;
			break;

		default:
			break;
	}
}
/*
void PORTA_IRQHandler(void)
{
	g_intr_status_flag.flag_port_a = TRUE;
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOA);

	if(irq_status & BUTTON_MENU_MASK){
		g_menu_pressed=TRUE;
	}
	GPIO_PortClearInterruptFlags(GPIOA, irq_status);
}

void PORTB_IRQHandler(void)
{
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOB);

	if(irq_status & SOUND_KICK_MASK){
		g_kick_pressed=TRUE;
	}

	if(irq_status & SOUND_SNARE_MASK){
		g_snare_pressed=TRUE;
	}

	if(irq_status & SOUND_GUIRO_MASK){
		g_guiro_pressed=TRUE;
	}

	if(irq_status & SOUND_TAMBOR_MASK){
		g_tambor_pressed=TRUE;
	}

	if(GPIOB_callback)
	{
		GPIOB_callback(irq_status);
	}
	GPIO_PortClearInterruptFlags(GPIOB, irq_status);
}

void PORTC_IRQHandler(void)
{
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOC);

	if(irq_status & SOUND_BONGO_MASK){
		g_bongo_pressed=TRUE;
	}

	if(irq_status & SOUND_HITHAT_MASK){
		g_hithat_pressed=TRUE;
	}

	if(irq_status & SOUND_RIMSHOT_MASK){
		g_rimshot_pressed=TRUE;
	}

	if(irq_status & SOUND_CYNBAL_MASK){
		g_cynbal_pressed=TRUE;
	}

	if(irq_status & SOUND_COWBELL_MASK){
		g_cowbell_pressed=TRUE;
	}

	if(GPIOC_callback)
	{
		GPIOC_callback(irq_status);
	}

	GPIO_PortClearInterruptFlags(GPIOC, irq_status);
}

void PORTD_IRQHandler(void)
{
	g_intr_status_flag.flag_port_d = TRUE;
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOD);

	if(irq_status & BUTTON_B1_MASK){
		g_b1_pressed=TRUE;
	}

	if(irq_status & BUTTON_B2_MASK){
		g_b2_pressed=TRUE;
	}

	GPIO_PortClearInterruptFlags(GPIOD, irq_status);


}
*/

void GPIO_port_irq_clr(GPIO_t *gpio)
{
	if(GPIOA == gpio)
	{
		g_intr_status_flag.flag_port_a = false;
	}
	if(GPIOB == gpio)
	{
		g_intr_status_flag.flag_port_b = false;
	}
	if(GPIOD == gpio)
	{
		g_intr_status_flag.flag_port_d = false;
	}
	else
	{
		g_intr_status_flag.flag_port_c = false;
	}
}



uint8_t GPIO_port_irq_get(GPIO_t *gpio, uint32_t pin)
{
	uint8_t status = 0;

	if(GPIOA == gpio)
	{
		status = g_intr_status_flag.flag_port_a;
	}
	else if(GPIOB == gpio)
	{
		if(pin==g_intr_register_pin){
		status = g_intr_status_flag.flag_port_b;

		}
	}
	else if(GPIOC == gpio)
	{
		if(pin==g_intr_register_pin){
			status = g_intr_status_flag.flag_port_c;
		}
	}
	else if(GPIOD == gpio)
		{
			if(pin==g_intr_register_pin){
				status = g_intr_status_flag.flag_port_d;
			}
		}
	return(status);
}



void GPIO_clock_gating(const char *ports)
{
    while (*ports != '\0')
    {
        switch (*ports)
        {
            case 'A':
                SIM_5 |= 0x0200;
                break;
            case 'B':
                SIM_5 |= 0x0400;
                break;
            case 'C':
                SIM_5 |= 0x0800;
                break;
            case 'D':
                SIM_5 |= 0x1000;
                break;
            case 'E':
                SIM_5 |= 0x2000;
                break;
            default:
                break;
        }
        ports++;
    }
}

void GPIO_pin_alt(const char *port, uint32_t pin, uint32_t alt)
{
    volatile uint32_t *port_pcr;

    switch (*port)
    {
        case 'A':
            port_pcr = (volatile uint32_t *)(PORTA + pin);
            break;
        case 'B':
            port_pcr = (volatile uint32_t *)(PORTB + pin);
            break;
        case 'C':
            port_pcr = (volatile uint32_t *)(PORTC + pin);
            break;
        case 'D':
            port_pcr = (volatile uint32_t *)(PORTD + pin);
            break;
        case 'E':
            port_pcr = (volatile uint32_t *)(PORTE + pin);
            break;
        default:
            return;
    }

    *port_pcr = (alt*256);
}

void GPIO_pin_init_io(GPIO_t *gpio, uint32_t pin, int output)
{

    if (output)
    {
        // set pin as output
        gpio->PDDR |= (1u << pin);
    }
    else
    {
        // set pin as input
        gpio->PDDR &= ~(1u << pin);
    }
}


void GPIO_pin_clr(GPIO_t *gpio, uint32_t pin)
{
	gpio->PCOR = (1u << pin);
}

void GPIO_pin_set(GPIO_t *gpio, uint32_t pin)
{
    gpio->PSOR = (1u << pin);
}

void GPIO_pin_tog(GPIO_t *gpio, uint32_t pin)
{
    gpio->PTOR = (1u << pin);
}



void GPIO_pin_pe(const char *port, uint32_t pin)
{
	 volatile uint32_t *port_pcr;

	    switch (*port)
	    {
	        case 'A':
	            port_pcr = (volatile uint32_t *)(PORTA + pin);
	            break;
	        case 'B':
	            port_pcr = (volatile uint32_t *)(PORTB + pin);
	            break;
	        case 'C':
	            port_pcr = (volatile uint32_t *)(PORTC + pin);
	            break;
	        case 'D':
	            port_pcr = (volatile uint32_t *)(PORTD + pin);
	            break;
	        case 'E':
	            port_pcr = (volatile uint32_t *)(PORTE + pin);
	            break;
	        default:
	            return;
	    }

	    *port_pcr |= 2;
}

void GPIO_pin_ps(const char *port, uint32_t pin)
{
	 volatile uint32_t *port_pcr;

	    switch (*port)
	    {
	        case 'A':
	            port_pcr = (volatile uint32_t *)(PORTA + pin);
	            break;
	        case 'B':
	            port_pcr = (volatile uint32_t *)(PORTB + pin);
	            break;
	        case 'C':
	            port_pcr = (volatile uint32_t *)(PORTC + pin);
	            break;
	        case 'D':
	            port_pcr = (volatile uint32_t *)(PORTD + pin);
	            break;
	        case 'E':
	            port_pcr = (volatile uint32_t *)(PORTE + pin);
	            break;
	        default:
	            return;
	    }

	    *port_pcr |= 1;
}

uint8_t GPIO_pin_read(GPIO_t *gpio, uint32_t pin)
{
	return (gpio->PDIR >> (pin)) & 0x01;
}

void GPIO_pin_set_interrupt_fe(const char *port, uint32_t pin)
{
	volatile uint32_t *port_pcr;

	switch (*port)
	{
		case 'A':
			port_pcr = (volatile uint32_t *)(PORTA + pin);
			break;
		case 'B':
			port_pcr = (volatile uint32_t *)(PORTB + pin);
			break;
		case 'C':
			port_pcr = (volatile uint32_t *)(PORTC + pin);
			break;
		case 'D':
			port_pcr = (volatile uint32_t *)(PORTD + pin);
			break;
		case 'E':
			port_pcr = (volatile uint32_t *)(PORTE + pin);
			break;
		default:
			return;
	}

	*port_pcr = (*port_pcr &  ~(0xF000U)) | ((0xAU<< (16U)) & (0xF0000U));

}

void GPIO_port_isf_clr(const char *port, uint32_t pin)
{
	volatile uint32_t *port_pcr;

	switch (*port)
	{
		case 'A':
			port_pcr = (volatile uint32_t *)(PORTA + pin);
			break;
		case 'B':
			port_pcr = (volatile uint32_t *)(PORTB + pin);
			break;
		case 'C':
			port_pcr = (volatile uint32_t *)(PORTC + pin);
			break;
		case 'D':
			port_pcr = (volatile uint32_t *)(PORTD + pin);
			break;
		case 'E':
			port_pcr = (volatile uint32_t *)(PORTE + pin);
			break;
		default:
			return;
	}

	*port_pcr |= PORT_PCR_ISF_MASK;

}
