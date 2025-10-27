/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:18:15 by ebonutto          #+#    #+#             */
/*   Updated: 2025/10/27 17:37:19 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main( void )
{
	// Set PB0 as output (LED)
	DDRB |= ( 1 << PB0 ); // DDRB = Data Direction Register B

	// Activate the led
	PORTB |= ( 1 << PB0 );

	while ( 1 )
	{
		// Infinite loop to make the led shine
	}

	return ( 0 );
}

