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
#ifndef GPIO_H_
#define GPIO_H_

#include "stdint.h"
#include "CONFIG.h"

#define SIM_5		(*((volatile uint32_t *) 0x40048038))
#define GPIOA_BASE	((volatile uint32_t *) 0x400FF000)
#define GPIOB_BASE	((volatile uint32_t *) 0x400FF040)
#define GPIOC_BASE	((volatile uint32_t *) 0x400FF080)
#define GPIOD_BASE	((volatile uint32_t *) 0x400FF0C0)
#define GPIOE_BASE	((volatile uint32_t *) 0x400FF100)

#define PORTA_PCR_BASE	(*((volatile uint32_t *) 0x4004A054))

typedef struct
{
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;
} GPIO_t;


typedef struct
{
	uint8_t flag_port_a : 1;
	uint8_t flag_port_b : 1;
	uint8_t flag_port_c : 1;
	uint8_t flag_port_d : 1;
	uint8_t flag_port_e : 1;
} gpio_interrupt_flags_t;

#define GPIOA ((GPIO_t*) GPIOA_BASE )
#define GPIOB ((GPIO_t*) GPIOB_BASE )
#define GPIOC ((GPIO_t*) GPIOC_BASE )
#define GPIOD ((GPIO_t*) GPIOD_BASE )
#define GPIOE ((GPIO_t*) GPIOE_BASE )

#define PORTA	((volatile uint32_t *) 0x40049000 )
#define PORTB	((volatile uint32_t *) 0x4004A000 )
#define PORTC	((volatile uint32_t *) 0x4004B000 )
#define PORTD	((volatile uint32_t *) 0x4004C000 )
#define PORTE	((volatile uint32_t *) 0x4004D000 )

void GPIO_clock_gating(const char *ports);
void GPIO_pin_alt(const char *port, uint32_t pin, uint32_t alt); //sets pin mux control alternative
void GPIO_pin_init_io(GPIO_t *gpio, uint32_t pin, int output); //initialize pin as output or input
void GPIO_pin_clr(GPIO_t *gpio, uint32_t pin);		//clears pin value
void GPIO_pin_set(GPIO_t *gpio, uint32_t pin);		//sets pin value to 1
void GPIO_pin_tog(GPIO_t *gpio, uint32_t pin);		//toggles pin value
void GPIO_pin_pe(const char *port, uint32_t pin); //sets pull enable to 1
void GPIO_pin_ps(const char *port, uint32_t pin); //sets pull select to 1
uint8_t GPIO_pin_read(GPIO_t *gpio, uint32_t pin);	//reads pin value
void GPIO_pin_set_interrupt_fe(const char *port, uint32_t pin); //sets ISF flag and Interrupt on falling-edge.
void GPIO_port_isf_clr(const char *port, uint32_t pin);  //clears Interrupt Status Flag
void GPIO_port_irq_clr(GPIO_t *gpio); 		//clears interruption request flags
uint8_t GPIO_port_irq_get(GPIO_t *gpio, uint32_t pin);	//gets interruption request flag info

void GPIO_callback_init(GPIO_Name_t gpio, void (*handler)(uint32_t flags));

boolean_t GPIO_get_b1_pressed();
void GPIO_set_b1_pressed(boolean_t value);
boolean_t GPIO_get_b2_pressed();
void GPIO_set_b2_pressed(boolean_t value);
boolean_t GPIO_get_menu_pressed();
void GPIO_set_menu_pressed(boolean_t value);
boolean_t GPIO_get_kick_pressed();
boolean_t GPIO_get_snare_pressed();
boolean_t GPIO_get_bongo_pressed();
boolean_t GPIO_get_hithat_pressed();
boolean_t GPIO_get_guiro_pressed();
boolean_t GPIO_get_rimshot_pressed();
boolean_t GPIO_get_tambor_pressed();
boolean_t GPIO_get_cynbal_pressed();
boolean_t GPIO_get_cowbell_pressed();

void GPIOE_callback_init();


#endif /* GPIO_H_ */
