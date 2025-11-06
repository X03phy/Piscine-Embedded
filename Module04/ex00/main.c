/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:19:58 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/06 12:52:25 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://wikigeii.iut-troyes.univ-reims.fr/index.php?title=Cours:Atmega328p

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DEBOUNCE_DELAY 20

volatile uint8_t button_state = 0;  // 0 = relâché, 1 = appuyé

ISR( INT0_vect )
{
	EIMSK &= ~( 1 << INT0 );  // Deactivate
	
	_delay_ms( DEBOUNCE_DELAY );

	if ( EICRA & ( 1 << ISC00 ) )  // Rising edge
	{
		button_state = 0;
		EICRA &= ~( 1 << ISC00 );
		PORTB ^= ( 1 << PB0 );
	}
	else  // Falling edge
	{
		button_state = 1;
		EICRA |= ( 1 << ISC00 );
	}

	_delay_ms( DEBOUNCE_DELAY );
	
	EIMSK |= ( 1 << INT0 );  // Reactivate
}

int main(void)
{
	DDRB |= ( 1 << PB0 );
	DDRD &= ~( 1 << PD2 );
	PORTD |= ( 1 << PD2 );
	
	cli();
	EICRA = ( 1 << ISC01 );  // Falling edge
	EIMSK = ( 1 << INT0 );
	sei();
	
	while ( 1 );
	
	return ( 0 );
}
