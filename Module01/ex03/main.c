/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:13:04 by x03phy            #+#    #+#             */
/*   Updated: 2025/10/30 17:42:13 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define DEBOUNCE_DELAY 20 // Delay (ms) to filter button bouncing

int main( void )
{
	DDRB |= ( 1 << PB1 ); // Configure PB1 as an output

	// Set PD2 and PD4 as inputs (buttons)
	DDRD &= ~ ( ( 1 << PD2 ) | ( 1 << PD4 ) );

	// Enable internal pull-up resistors for PD2 and PD4
	PORTD |= ( 1 << PD2 ) | ( 1 << PD4 );

	TCCR1A |= ( 1 << COM1A1 ); // For duty cycle
	TCCR1A |= ( 1 << WGM11 );
	TCCR1B |= ( 1 << WGM12 ) | ( 1 << WGM13 ); // mode 14 Fast PWM

	TCCR1B |= ( 1 << CS12 ) | ( 1 << CS10 ); // Prescaler = 1024
	ICR1 = 15625; // 1 cycle
	OCR1A = 7813; // Duty cycle 50 to start%

	while ( 1 )
	{
		// If SW1 (PD2) is pressed
		if ( ! ( PIND & ( 1 << PD2 ) ) )
		{
			if ( OCR1A <= 14063 ) // 90 %
				OCR1A += ( ( OCR1A % 5 == 0 ) ? 1563 : 1562 );
			while ( ! ( PIND & ( 1 << PD2 ) ) ); // Wait until button is released
			_delay_ms( DEBOUNCE_DELAY );  // Delay after release to avoid bouncing
		}
		// If SW2 (PD4) is pressed
		else if ( ! ( PIND & ( 1 << PD4 ) ) )
		{
			if ( OCR1A >= 1563 ) // 10 %
				OCR1A -= ( ( OCR1A % 5 == 0 ) ? 1562 : 1563 );
			while ( ! ( PIND & ( 1 << PD4 ) ) ); // Wait until button is released
			_delay_ms( DEBOUNCE_DELAY );  // Delay after release to avoid bouncing
		}
	}

	return ( 0 );
}
