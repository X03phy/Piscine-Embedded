/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:19:58 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/09 19:51:55 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int unpressed = 1;

// INT0 Interrupt
void INT0_vect( void ) __attribute__( ( signal, used ) );

void INT0_vect( void )
{
	// Disable INT0
	EIMSK &= ~ ( 1 << INT0 );

	if ( ( EICRA & ( 1 << ISC00 ) ) == 0 ) // Is falling edge
	{
		PORTB ^= ( 1 << PB0 ); // Toggle led
		EICRA |=  ( 1 << ISC00 ) | ( 1 << ISC01 ); // Set rising edge
	}
	else
		unpressed = 0;

	// Enable INT0
	EIMSK |= ( 1 << INT0 );
}

int main( void )
{
	// Set PD0 (led D1) as an output
	DDRB |= ( 1 << PD0 );

	// Set PD2 as input (button)
	DDRD &= ~ ( 1 << PD2 );

	// Enable internal pull-up resistors for PD2
	PORTD |= ( 1 << PD2 );

	// Disable global interruptions
	cli();

	// Set INT0 to Falling edge
	EICRA &= ~ ( 1 << ISC00 );
	EICRA |=  ( 1 << ISC01 );

	// Enable INT0
	EIMSK |=  ( 1 << INT0 );

	// Enable global interruptions
	sei();

	while ( 1 )
	{
		// Debouce button
		_delay_ms( 20 );

		if ( !unpressed )
		{
			unpressed = 1;

			// Set INT0 to Falling edge
			EICRA &= ~ ( 1 << ISC00 );
			EICRA |=  ( 1 << ISC01 );
		}
	}

	return ( 0 );
}
