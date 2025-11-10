/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:21:56 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/10 08:47:18 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define DEBOUNCE_DELAY 20 // Delay (ms) to filter button bouncing

int main( void )
{
	uint8_t counter = 0;

	// Set PB0, PB1, PB2, PB4 as outputs (LEDs)
	DDRB |= ( 1 << PB0 ) | ( 1 << PB1 ) | ( 1 << PB2 ) | ( 1 << PB4 );

	// Set PD2 and PD4 as inputs (buttons)
	DDRD &= ~ ( ( 1 << PD2 ) | ( 1 << PD4 ) );

	// Enable internal pull-up resistors for PD2 and PD4
	PORTD |= ( 1 << PD2 ) | ( 1 << PD4 );

	while ( 1 )
	{
		// If SW1 (PD2) is pressed
		if ( ! ( PIND & ( 1 << PD2 ) ) )
		{
			if ( counter < 15 )
				counter += 1;
			while ( ! ( PIND & ( 1 << PD2 ) ) ); // Wait until button is released
			_delay_ms( DEBOUNCE_DELAY );  // Delay after release to avoid bouncing
		}
		// If SW2 (PD4) is pressed
		else if ( ! ( PIND & ( 1 << PD4 ) ) )
		{
			if ( counter > 0 )
				counter -= 1;
			while ( ! ( PIND & ( 1 << PD4 ) ) ); // Wait until button is released
			_delay_ms( DEBOUNCE_DELAY );  // Delay after release to avoid bouncing
		}

		PORTB = ~ ( ( 1 << PB0 ) | ( 1 << PB1 ) | ( 1 << PB2 ) | ( 1 << PB4 ) ) // Clear LED bits
		      | ( ( ( counter >> 3 ) & 1 ) << PB0 )  // bit 3 → PB0
		      | ( ( ( counter >> 2 ) & 1 ) << PB1 )  // bit 2 → PB1
		      | ( ( ( counter >> 1 ) & 1 ) << PB2 )  // bit 1 → PB2
		      | ( ( counter & 1 ) << PB4 );          // bit 0 → PB4
	}

	return ( 0 );
}
