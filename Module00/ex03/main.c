/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 15:53:19 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/05 17:23:00 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define DEBOUNCE_DELAY 20 // Delay (ms) to filter button bouncing

int main( void )
{
	uint8_t previous_state;
	uint8_t current_state;

	// Set PB0 as output (LED)
	DDRB |= ( 1 << PB0 );

	// Set PD2 (SW1) as input (button)
	DDRD &= ~ ( 1 << PD2 );

	// Enable internal pull-up resistor for PD2
	PORTD |= ( 1 << PD2 );

	previous_state = 1; // Button not pressed = HIGH

	while ( 1 )
	{
		current_state = PIND & ( 1 << PD2 ); // It is the same as asking: Is it HIGH ?

		if ( previous_state && ! current_state )
		{
			_delay_ms( DEBOUNCE_DELAY ); // Delay after release to avoid bouncing

			if ( ! ( PIND & ( 1 << PD2 ) ) ) // Check if it's not the button boucing
			{
				PORTB ^= ( 1 << PB0 ); // Switch current led state
			}
		}

		previous_state = ( current_state ? 1 : 0 ); // Memorize current state
	}

	return ( 0 );
}
