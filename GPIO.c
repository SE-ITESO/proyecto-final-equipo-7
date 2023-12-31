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
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include <stdbool.h>

volatile static gpio_interrupt_flags_t g_intr_status_flag = {0};

volatile static uint32_t g_intr_register_dec_pin = 0;

volatile static uint32_t g_intr_register_pin = 0;

static void (*GPIOB_callback)(uint32_t flags) = 0;
static void (*GPIOC_callback)(uint32_t flags) = 0;
static void (*GPIOD_callback)(uint32_t flags) = 0;
static void (*GPIOE_callback)(uint32_t flags) = 0;


void GPIOB_callback_init( void (*handler)(uint32_t flags))
{
	GPIOB_callback = handler;

}

void PORTB_IRQHandler(void)
{
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOB);

	if (GPIOB_callback)
	{
		GPIOB_callback(irq_status);
	}

	GPIO_port_isf_clr("B", 10);
}

void GPIOC_callback_init( void (*handler)(uint32_t flags))
{
	GPIOC_callback = handler;

}

void PORTC_IRQHandler(void)
{
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOC);

	if (GPIOC_callback)
	{
		GPIOC_callback(irq_status);
	}

	GPIO_port_isf_clr("C", 11);
}

void GPIOD_callback_init( void (*handler)(uint32_t flags))
{
	GPIOD_callback = handler;

}

void PORTD_IRQHandler(void)
{
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOD);

	if (GPIOD_callback)
	{
		GPIOD_callback(irq_status);
	}

	GPIO_port_isf_clr("D", 3);
}

void GPIOE_callback_init( void (*handler)(uint32_t flags))
{
	GPIOE_callback = handler;

}

void PORTE_IRQHandler(void)
{
	uint32_t irq_status = 0;
	irq_status = GPIO_PortGetInterruptFlags(GPIOE);

	if (GPIOE_callback)
	{
		GPIOE_callback(irq_status);
	}

	GPIO_port_isf_clr("E", 24);
}


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
