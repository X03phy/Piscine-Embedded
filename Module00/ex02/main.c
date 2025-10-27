/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 15:15:16 by ebonutto          #+#    #+#             */
/*   Updated: 2025/10/27 16:17:04 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main( void )
{
	DDRB |= ( 1 << PB0 ); // Configure PB0 as an output

	DDRD &= ~ ( 1 << PD2 ); // Configure SW1 as an entry
	PORTD |= ( 1 << PD2 ); // Activate SW1

	while ( 1 )
	{
		if ( ! ( PIND & ( 1 << PD2 ) ) ) // Button pressed -> PIND bit = 0
		{
			PORTB |= ( 1 << PB0 );
		}
		else
		{
			PORTB &= ~ ( 1 << PB0 ); // Button not pressed -> PIND bit = 1
		}
	}

	return ( 0 );	
}
