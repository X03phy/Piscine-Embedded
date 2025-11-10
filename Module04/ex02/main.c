/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:19:58 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/10 09:12:16 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t counter = 0;

static void display_counter( void )
{
	PORTB = ( PORTB & ~ ( ( 1 << PB0 ) | ( 1 << PB1 ) | ( 1 << PB2 ) | ( 1 << PB4 ) ) ) // Clear LED bits
		      | ( ( ( counter >> 3 ) & 1 ) << PB0 )  // bit 3 → PB0
		      | ( ( ( counter >> 2 ) & 1 ) << PB1 )  // bit 2 → PB1
		      | ( ( ( counter >> 1 ) & 1 ) << PB2 )  // bit 1 → PB2
		      | ( ( counter & 1 ) << PB4 );          // bit 0 → PB4
}

// INT0 Interrupt
void INT0_vect( void ) __attribute__( ( signal, used ) );

void INT0_vect( void )
{
	// If SW1 (PD2) is pressed
	if ( ! ( PIND & ( 1 << PD2 ) ) )
	{
		if ( counter < 15 )
			counter += 1;
		while ( ! ( PIND & ( 1 << PD2 ) ) ); // Wait until button is released
		_delay_ms( 20 );  // Delay after release to avoid bouncing
	}
	
	display_counter();
}

// INT1 Interrupt
// void INT1_vect( void ) __attribute__( ( signal, used ) );

void PCINT2_vect( void ) __attribute__( ( signal, used ) );

void PCINT2_vect( void )
{
	
	if ( ! ( PIND & ( 1 << PD4 ) ) )
	{
		if ( counter > 0 )
			counter -= 1;
		while ( ! ( PIND & ( 1 << PD4 ) ) ); // Wait until button is released
		_delay_ms( 20 );  // Delay after release to avoid bouncing
	}

	display_counter();
}
	
int main( void )
{
	// Set PB0, PB1, PB2, PB4 as outputs (LEDs)
	DDRB |= ( 1 << PB0 ) | ( 1 << PB1 ) | ( 1 << PB2 ) | ( 1 << PB4 );

	// Set PD2 and PD4 as inputs (buttons)
	DDRD &= ~ ( ( 1 << PD2 ) | ( 1 << PD4 ) );

	// Enable internal pull-up resistors for PD2 and PD4
	PORTD |= ( 1 << PD2 ) | ( 1 << PD4 );

	// Disable global interruptions
	cli();

	// Set INT0 to Falling edge
	EICRA &= ~ ( 1 << ISC00 );
	EICRA |=  ( 1 << ISC01 );

	// Enable INT0
	EIMSK |= ( 1 << INT0 );

	// Same for INT1
	PCICR |= ( 1 << PCIE2 );

	// Enable pin change mask 20
	PCMSK2 |= ( 1 << PCINT20 );


	// Enable global interruptions
	sei();

	while ( 1 );

	return ( 0 );
}
