/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:40:10 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/09 20:17:18 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PWM (Pulse Width Modulation)

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t brightness = 0;
volatile int8_t direction = 1;

// TMER0_COMPA Interrupt
void TIMER0_COMPA_vect( void ) __attribute__( ( signal, used ) );

void TIMER0_COMPA_vect( void )
{
	brightness += direction;

	// Switch direction
	if ( brightness == 255 || brightness == 0 )
		direction = -direction;

	OCR1A = brightness;
}

// 2 ms between each step
// 2ms -> 
// freq ​= 16 000 000 / ( 256 * ( 1 + 124 ) ) = 500 Hz -> 2 ms
int main( void )
{
	DDRB |= ( 1 << PB1 ); // Configure PB1 (led D2) as an output

	cli();

	TCCR1A = ( 1 << COM1A1 ) | ( 1 << WGM10 );  // non-inverting, 8-bit Fast PWM
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS10 );    // Fast PWM, prescaler = 1
	OCR1A = 0;                                  // initial PWM at 0

	TCCR0A = ( 1 << WGM01 );  // CTC mode
	TCCR0B = ( 1 << CS02 );   // prescaler = 256

	OCR0A = 124;
	TIMSK0 = ( 1 << OCIE0A );  // Enable interrupt

	sei();

	while ( 1 );

	return ( 0 );
}
