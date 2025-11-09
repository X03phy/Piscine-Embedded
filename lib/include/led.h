#ifndef LED_H
#define LED_H

#include <avr/io.h>

/* Macros */

/*  Doc  */
#define LED_PORT    PORTB
#define LED_DDR     DDRB
#define LED_PIN_REG PINB

/*  Doc  */
#define LED_D1_PIN PB0
#define LED_D2_PIN PB1
#define LED_D3_PIN PB2
#define LED_D4_PIN PB4

#define LED_D1_MASK ( 1 << LED_D1_PIN )
#define LED_D2_MASK ( 1 << LED_D2_PIN )
#define LED_D3_MASK ( 1 << LED_D3_PIN )
#define LED_D4_MASK ( 1 << LED_D4_PIN )

#define LED_ALL_MASK ( LED_D1_MASK | LED_D2_MASK | LED_D3_MASK | LED_D4_MASK )


#define LED_ON( pin_mask )     ( LED_PORT |= ( pin_mask ) )
#define LED_OFF( pin_mask )    ( LED_PORT &= ~ ( pin_mask ) )
#define LED_TOGGLE( pin_mask ) ( LED_PORT ^= ( pin_mask ) )

/* Prototypes */

/*  Doc  */
void init_led_D1( void );
void init_led_D2( void );
void init_led_D3( void );
void init_led_D4( void );
void init_leds( void );



#endif
