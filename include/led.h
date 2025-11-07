#ifndef LED_H
#define LED_H

#include <avr/io.h>


/*──────────────────────────────────────────────*/
/*─ SINGLE COLOR LED DEFINITIONS ───────────────*/
/*──────────────────────────────────────────────*/

/* LED pins on PORTB */
#define D1_LED PB0  /* LED D1 connected to PB0 */
#define D2_LED PB1  /* LED D2 connected to PB1 */
#define D3_LED PB2  /* LED D3 connected to PB2 */
#define D4_LED PB4  /* LED D4 connected to PB4 */

/*──────────────────────────────────────────────*/
/*─ RGB LED (D5) DEFINITIONS ──────────────────*/
/*──────────────────────────────────────────────*/

/* RGB LED pins on PORTD */
#define D5_LED_RED   PD5  /* Red component of D5 */
#define D5_LED_GREEN PD6  /* Green component of D5 */
#define D5_LED_BLUE  PD3  /* Blue component of D5 */

/* Predefined colors by combining RGB components */
#define D5_LED_YELLOW  ( D5_LED_RED | D5_LED_GREEN )            /* Red + Green */
#define D5_LED_CYAN    ( D5_LED_GREEN | D5_LED_BLUE )          /* Green + Blue */
#define D5_LED_MAGENTA ( D5_LED_RED | D5_LED_BLUE )            /* Red + Blue */
#define D5_LED_WHITE   ( D5_LED_RED | D5_LED_GREEN | D5_LED_BLUE ) /* Red + Green + Blue */

/*──────────────────────────────────────────────*/
/*─ FUNCTION PROTOTYPES ────────────────────────*/
/*──────────────────────────────────────────────*/

/* Initialize D5 RGB LED pins as outputs */
void init_D5_led_RGB( void );

/* TODO: add function prototypes to set specific colors on D5 RGB LED */
void set_D5_led_color( uint8_t color );


#endif
