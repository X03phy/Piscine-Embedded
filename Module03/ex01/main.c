/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:58:24 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/07 12:50:12 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://forum.arduino.cc/t/pulsing-rgb-led-understanding-ocm-and-phase-correct-pwm/1212393

#include <avr/io.h>
#include <util/delay.h>

#define RED_BIT PD5
#define GRN_BIT PD6
#define BLU_BIT PD3

int main( void )
{
	// R, G, B
	DDRD |= ( ( 1 << RED_BIT ) | ( 1 << GRN_BIT ) | ( 1 << BLU_BIT ));

	while ( 1 )
	{
		PORTD = ( 1 << RED_BIT );
		_delay_ms( 1000 );
		PORTD = ( 1 << GRN_BIT );
		_delay_ms( 1000 );
		PORTD = ( 1 << BLU_BIT );
		_delay_ms( 1000 );

		PORTD = ( ( 1 << RED_BIT ) | ( 1 << GRN_BIT ) ); // yellow
		_delay_ms( 1000 );
		PORTD = ( ( 1 << GRN_BIT ) | ( 1 << BLU_BIT ) ); // cyan
		_delay_ms( 1000 );
		PORTD = ( ( 1 << RED_BIT ) | ( 1 << BLU_BIT ) ); // magenta
		_delay_ms( 1000 );
		PORTD = ( ( 1 << RED_BIT ) | ( 1 << GRN_BIT ) | ( 1 << BLU_BIT ) ); // white
		_delay_ms( 1000 );
	}

	return ( 0 );
}
