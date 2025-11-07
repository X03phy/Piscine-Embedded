/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:40:10 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/07 11:27:07 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PWM (Pulse Width Modulation)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t brightness = 0;
volatile int8_t direction = 1;

ISR( TIMER0_COMPA_vect )
{
	brightness += direction;

	// Switch direction
	if ( brightness == 255 || brightness == 0 )
		direction = -direction;

	OCR1A = brightness;
}

int main( void )
{
	DDRB |= ( 1 << PB1 ); // Configure PB1 as an output

	cli();
	TCCR1A = ( 1 << COM1A1 ) | ( 1 << WGM10 );  // non-inverting, 8-bit Fast PWM
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS10 );    // Fast PWM, prescaler = 1
	OCR1A = 0;                                  // initial PWM at 

	TCCR0A = ( 1 << WGM01 );  // CTC mode
	TCCR0B = ( 1 << CS02 ); // prescaler = 256

	OCR0A = 124;
	TIMSK0 = ( 1 << OCIE0A ); // Activate interrupt
	
	sei();

	while ( 1 );

	return ( 0 );
}
