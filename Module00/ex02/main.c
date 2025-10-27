/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 15:15:16 by ebonutto          #+#    #+#             */
/*   Updated: 2025/10/27 17:51:31 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define DEBOUNCE_DELAY 20 // Delay (ms) to filter button bouncing

int main( void )
{
	DDRB |= ( 1 << PB0 ); // Configure PB0 as an output

	DDRD &= ~ ( 1 << PD2 ); // Configure SW1 as an entry
	PORTD |= ( 1 << PD2 ); // Activate SW1

	while ( 1 )
	{
		if ( ! ( PIND & ( 1 << PD2 ) ) ) // Button pressed -> PIND bit = 0
		{
			_delay_ms( DEBOUNCE_DELAY ); // Delay after release to avoid bouncing
			
			if ( ! ( PIND & ( 1 << PD2 ) ) ) // Check if it's not the button boucing
			{
				PORTB |= ( 1 << PB0 );
			}
		}
		else  // Button not pressed -> PIND bit = 1
		{
			_delay_ms( DEBOUNCE_DELAY ); // Delay after release to avoid bouncing

			if ( ( PIND & ( 1 << PD2 ) ) ) // Check if it's not the button boucing
			{
				PORTB &= ~ ( 1 << PB0 );
			}
		}
	}

	return ( 0 );
}
